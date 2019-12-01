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

Context::Context()
    : comp_allocators_(),
      comp_ptrs_(),
      ent_allocator_(nullptr),
      ent_ptrs(),
      mem_free_list_(100 * MB_SIZE, FreeListAllocator::FIND_FIRST),
      ent_id_stack_(),
      ent_current_id_(0)
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
    u16 ent_alloc = DEFAULT_ENTITY_ALLOC;

    sizet ent_byte_size = sizeof(Entity);
    if (ent_byte_size < MIN_CHUNK_ALLOC_SIZE)
        ent_byte_size = MIN_CHUNK_ALLOC_SIZE;

    // Create the entity allocator based on settings
    auto fiter = init_params.find(ENTITY_ALLOC_KEY);
    if (fiter != init_params.end() && fiter->second.is_type<u16>())
        ent_alloc = fiter->second.get_value<u16>();

    ilog("Creating {} byte pool allocator for entities", ent_alloc * ent_byte_size);
    ent_allocator_ = malloc<PoolAllocator>(ent_alloc * ent_byte_size, ent_byte_size);
    ent_allocator_->Init();
    ent_ptrs.reserve(ent_alloc);
}

void Context::create_component_allocators_(const Variant_Map & init_params)
{
    // Auto get component types and register allocators for them
    auto t = rttr::type::get<Component>();
    auto derived = t.get_derived_classes();
    auto type_iter = derived.begin();
    sizet total_comp_count = 0;
    while (type_iter != derived.end())
    {
        u16 comp_alloc = DEFAULT_COMP_ALLOC;
        auto fiter = init_params.find(String(type_iter->get_name()) + "_Alloc");
        if (fiter != init_params.end() && fiter->second.is_type<u16>())
            comp_alloc = fiter->second.get_value<u16>();
        register_component_type_(*type_iter, comp_alloc);
        total_comp_count += comp_alloc;
        ++type_iter;
    }
    comp_ptrs_.reserve(total_comp_count);
}

void Context::terminate()
{
    world_->terminate();

    // Not really job of the context... but just in case delete any leftover entities and components
    // Terminating the world should destroy all entities, destroying all entities should destroy all components
    // We track them here just in case
    while (!ent_ptrs.empty())
        destroy_entity(ent_ptrs[0]);

    free(ent_allocator_);
    while (comp_allocators_.begin() != comp_allocators_.end())
    {
        free(comp_allocators_.begin()->second);
        comp_allocators_.erase(comp_allocators_.begin());
    }
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

void Context::on_entity_id_change(Pair<u32> id)
{
    dlog("Context dealing with entity id change - old id: {} and new id {}", id.x, id.y);
}

void Context::on_entity_owner_id_change(Pair<sizet> owner_id)
{
    dlog("Context dealing with entity owner change - old id: {} and new id {}",
         owner_id.x,
         owner_id.y);
}

void Context::register_component_type_(const rttr::type & type, u16 expected_component_count)
{
    sizet type_size = type.get_sizeof();

    if (type_size < MIN_CHUNK_ALLOC_SIZE)
        type_size = MIN_CHUNK_ALLOC_SIZE;

    PoolAllocator * comp_alloc =
        malloc<PoolAllocator>(expected_component_count * type_size, type_size);

    comp_alloc->Init();
    comp_allocators_.emplace(type.get_id(), comp_alloc);
    String str(type.get_name());
    ilog("Creating {0} byte pool allocator for component type {1}",
         expected_component_count * type_size,
         str);
}

void Context::raw_free(void * to_free)
{
    mem_free_list_.Free(to_free);
}

Entity * Context::create_entity(const Entity * copy)
{
    sizet ent_size = sizeof(Entity);
    ilog("Allocating {0} bytes for entity", ent_size);
    Entity * ent = static_cast<Entity *>(ent_allocator_->Allocate(ent_size));

    // If crashing here means not enough mem for entities - allocate more
    assert(ent != nullptr);

    ent_ptrs.push_back(ent);
    if (copy != nullptr)
        new (ent) Entity(*copy);
    else
        new (ent) Entity();

    // Assign id to entity
    if (!ent_id_stack_.empty())
    {
        ent->set_id(ent_id_stack_.top());
        ent_id_stack_.pop();
    }
    else
    {
        ++ent_current_id_;
        ent->set_id(ent_current_id_);
    }

    ent->initialize();

    // Any time the entity has it's id or owner changed we can propagate those changes correctly here
    sig_connect(ent->id_change, Context::on_entity_id_change);
    sig_connect(ent->owner_id_change, Context::on_entity_owner_id_change);

    return ent;
}

void Context::destroy_entity(Entity * ent)
{
    ent->terminate();

    // Take the id back
    ent_id_stack_.push(ent->get_id());

    // This should also call the signal to change the comp id
    ent->set_id(0);

    // Calling the destructor of the ent will automatically disconnect its signals so no
    // need to do that here
    ent->~Entity();

    ilog("De-allocating {} bytes for entity {}", sizeof(Entity), ent->get_name());
    ent_allocator_->Free(ent);

    if (ent_ptrs.empty())
    {
        elog("There was no pointer in ent_ptrs vector which means the context has lost track of "
             "it somehow");
        return;
    }

    // Find the pointer - copy the last element of the vector to overwrite the pointer's value
    // and decrease the vector size by one
    for (sizet i = 0; i < ent_ptrs.size(); ++i)
    {
        if (ent == ent_ptrs[i])
        {
            ent_ptrs[i] = ent_ptrs[ent_ptrs.size() - 1];
            break;
        }
    }
    ent_ptrs.pop_back();    
}

PoolAllocator * Context::get_comp_allocator(const rttr::type & type)
{
    auto fiter = comp_allocators_.find(type.get_id());
    if (fiter != comp_allocators_.end())
        return fiter->second;
    return nullptr;
}

} // namespace noble_steed


#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Context>("Context")
        .constructor<>();
        // .property("id_", &Component::initialize, registration::public_access)
        // .property("terminate", &Component::terminate, registration::public_access)
        // .property("owner_id", &Component::owner_id_, registration::private_access);
}
