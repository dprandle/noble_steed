#include <noble_steed/core/context.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/filesystem.h>

#include <noble_steed/scene/world.h>
#include <noble_steed/scene/component.h>
#include <noble_steed/scene/entity.h>

#include <PoolAllocator.h>
#include <Allocator.h>

namespace noble_steed
{
Context * Context::s_this_ = nullptr;

uint32_t Context::current_id_ = 0;

Context::Context()
    : comp_allocators_(),
    comp_ptrs_(),
      ent_allocator_(nullptr),
      ent_ptrs(),
      mem_free_list_(100 * MB_SIZE, FreeListAllocator::FIND_FIRST)
{
    mem_free_list_.Init();
    world_ = malloc<World>();
    logger_ = malloc<Logger>();
    s_this_ = this;
}

Context::~Context()
{
    free(world_);
    free(logger_);
    mem_free_list_.Reset();
}

void Context::initialize(const Variant_Map & init_params)
{
    fs::create_directory("logs");
    logger_->initialize();
    create_entity_allocator_(init_params);
    create_component_allocators_(init_params);
    world_->initialize();
}

void Context::create_entity_allocator_(const Variant_Map & init_params)
{
    uint16_t ent_alloc = DEFAULT_ENTITY_ALLOC;

    std::size_t ent_byte_size = sizeof(Entity);
    if (ent_byte_size < MIN_CHUNK_ALLOC_SIZE)
        ent_byte_size = MIN_CHUNK_ALLOC_SIZE;

    // Create the entity allocator based on settings
    auto fiter = init_params.find(ENTITY_ALLOC_KEY);
    if (fiter != init_params.end() && fiter->second.is_type<uint16_t>())
        ent_alloc = fiter->second.get_value<uint16_t>();

    ilog("Creating {} byte pool allocator for entities", ent_alloc*ent_byte_size);
    ent_allocator_ = malloc<PoolAllocator>(ent_alloc * ent_byte_size, ent_byte_size);
    ent_ptrs.reserve(ent_alloc);
}

void Context::create_component_allocators_(const Variant_Map & init_params)
{
    // Auto get component types and register allocators for them
    auto t = rttr::type::get<Component>();
    auto derived = t.get_derived_classes();
    auto type_iter = derived.begin();
    std::size_t total_comp_count = 0;
    while (type_iter != derived.end())
    {
        uint16_t comp_alloc = DEFAULT_COMP_ALLOC;
        auto fiter = init_params.find(String(type_iter->get_name()) + "_Alloc");
        if (fiter != init_params.end() && fiter->second.is_type<uint16_t>())
            comp_alloc = fiter->second.get_value<uint16_t>();
        register_component_type_(*type_iter, comp_alloc);
        total_comp_count += comp_alloc;
        ++type_iter;
    }
    comp_ptrs_.reserve(total_comp_count);
}

void Context::terminate()
{
    //ent_allocator_->Reset();
//    while (!ent_ptrs.empty())
        // destroy enttites
    free(ent_allocator_);

    while (!comp_ptrs_.empty())
        destroy_component(comp_ptrs_[0]);

    while (comp_allocators_.begin() != comp_allocators_.end())
    {
        //comp_allocators_.begin()->second->Reset();
        free(comp_allocators_.begin()->second);
        comp_allocators_.erase(comp_allocators_.begin());
    }
    world_->terminate();
    logger_->terminate();
}

World * Context::get_world()
{
    return world_;
}

Logger * Context::get_logger()
{
    return logger_;
}

Context & Context::inst()
{
    return *s_this_;
}

void Context::on_component_id_change(glm::uvec2 id)
{
    tlog("Context dealing with component id change - old id: {} and new id {}", id.x, id.y);
}

void Context::on_component_owner_id_change(glm::uvec2 owner_id)
{
    tlog("Context dealing with component owner change - old id: {} and new id {}",
         owner_id.x,
         owner_id.y);
}

void Context::register_component_type_(const rttr::type & type, uint16_t expected_component_count)
{
    std::size_t type_size = type.get_sizeof();

    if (type_size < MIN_CHUNK_ALLOC_SIZE)
        type_size = MIN_CHUNK_ALLOC_SIZE;

    PoolAllocator * comp_alloc =
        malloc<PoolAllocator>(expected_component_count * type_size, type_size);

    comp_alloc->Init();
    comp_allocators_.emplace(type.get_id(), comp_alloc);
    String str(type.get_name());
    ilog("Creating {0} byte pool allocator for component type {1}", expected_component_count * type_size, str);
}

void Context::raw_free(void * to_free)
{
    mem_free_list_.Free(to_free);
}

Component * Context::create_component_(const rttr::type & type)
{
    PoolAllocator * alloc = get_comp_allocator_(type);
    ilog("Allocating {0} bytes for {1}", type.get_sizeof(), String(type.get_name()));
    Component * comp_ptr = static_cast<Component *>(alloc->Allocate(type.get_sizeof()));
    comp_ptrs_.push_back(comp_ptr);
    return comp_ptr;
}

void Context::destroy_component(Component * comp)
{
    comp->terminate();

    // Take the id back
    id_stack_.push(comp->get_id());

    // This should also call the signal to change the comp id
    comp->set_id(0);

    // Must get the type before calling the destructor
    rttr::type type = comp->get_derived_info().m_type;

    // Calling the destructor of the comp will automatically disconnect its signals so no
    // need to do that here
    comp->~Component();

    PoolAllocator * alloc = get_comp_allocator_(type);
    ilog("De-allocating {0} bytes for {1}", type.get_sizeof(), String(type.get_name()));
    alloc->Free(comp);

    if (comp_ptrs_.empty())
    {
        elog("There was no pointer in comp_ptrs_ vector which means the context has lost track of it somehow");
        return;
    }

    // Find the pointer - copy the last element of the vector to overwrite the pointer's value
    // and decrease the vector size by one
    for (std::size_t i = 0; i < comp_ptrs_.size(); ++i)
    {
        if (comp == comp_ptrs_[i])
        {
            comp_ptrs_[i] = comp_ptrs_[comp_ptrs_.size()-1];
            break;
        }
    }
    comp_ptrs_.pop_back();
}

PoolAllocator * Context::get_comp_allocator_(const rttr::type & type)
{
    auto fiter = comp_allocators_.find(type.get_id());
    if (fiter != comp_allocators_.end())
        return fiter->second;
    return nullptr;
}

} // namespace noble_steed
