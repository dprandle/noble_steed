#include <noble_steed/core/context.h>
#include <noble_steed/core/common.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/filesystem.h>

#include <noble_steed/scene/world.h>
#include <noble_steed/scene/component.h>

#include <PoolAllocator.h>
#include <Allocator.h>


namespace noble_steed
{
Context * Context::s_this_ = nullptr;

uint32_t Context::current_id_ = 0;

Context::Context()
    : comp_allocators_(), mem_free_list_(100 * MB_SIZE, FreeListAllocator::FIND_FIRST)
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

void Context::initialize()
{
    fs::create_directory("logs");
    logger_->initialize();

    // Auto get component types and register allocators for them
    auto t = rttr::type::get<Component>();
    auto derived = t.get_derived_classes();
    auto type_iter = derived.begin();
    while (type_iter != derived.end())
    {
        register_component_type_(*type_iter, DEFAULT_COMP_ALLOC);
        ++type_iter;
    }

    world_->initialize();
}

void Context::terminate()
{
    logger_->terminate();
    world_->terminate();
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
    ilog("Context dealing with component id change - old id: {} and new id {}",id.x,id.y);
}

void Context::on_component_owner_id_change(glm::uvec2 owner_id)
{
    ilog("Context dealing with component owner change - old id: {} and new id {}",owner_id.x,owner_id.y);
}

void Context::register_component_type_(const rttr::type & type, uint16_t expected_component_count)
{
    PoolAllocator * comp_alloc =
        malloc<PoolAllocator>(expected_component_count * type.get_sizeof(), type.get_sizeof());
    comp_alloc->Init();
    comp_allocators_.emplace(type.get_id(), comp_alloc);
    String str(type.get_name());
    ilog("Registered allocator for component type {}",str);
}

void Context::raw_free(void * to_free)
{
    mem_free_list_.Free(to_free);
}

Component * Context::create_component_(const rttr::type & type)
{
    PoolAllocator * alloc = get_comp_allocator_(type);
    ilog("Allocating {0} bytes for {1}",type.get_sizeof(),String(type.get_name()));
    Component * comp_ptr = static_cast<Component*>(alloc->Allocate(type.get_sizeof()));
    return comp_ptr;
}

void Context::destroy_component_(Component * comp, const rttr::type & type)
{
    PoolAllocator * alloc = get_comp_allocator_(type);
    ilog("De-allocating {0} bytes for {1}",type.get_sizeof(),String(type.get_name()));
    alloc->Free(comp);
}


PoolAllocator * Context::get_comp_allocator_(const rttr::type & type)
{
    auto fiter = comp_allocators_.find(type.get_id());
    if (fiter != comp_allocators_.end())
        return fiter->second;
    return nullptr;
}

} // namespace noble_steed
