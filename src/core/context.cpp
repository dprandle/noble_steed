#include <noble_steed/core/context.h>
#include <noble_steed/core/logger.h>
#include <noble_steed/core/filesystem.h>
#include <noble_steed/core/system.h>
#include <noble_steed/scene/entity.h>
#include <noble_steed/scene/world.h>
#include <noble_steed/core/resource_cache.h>

namespace noble_steed
{
Context * Context::s_this_ = nullptr;

Context::Context()
    : mem_free_list_(100 * MB_SIZE, FreeListAllocator::FIND_FIRST),
      comp_allocators_(),
      resource_allocators_(),
      type_factories_(),
      ent_allocator_(nullptr),
      logger_(nullptr),
      world_(nullptr),
      resource_cache_(nullptr)
{
    s_this_ = this;
}

Context::~Context()
{}

void Context::initialize(const Variant_Map & init_params)
{
    // Create log directory
    fs::create_directory("logs");

    // Allocate memories
    mem_free_list_.Init();

    // Initialize logger before other stuff so logging works
    logger_ = malloc<Logger>();
    logger_->initialize();
    resource_cache_ = malloc<Resource_Cache>();
    world_ = malloc<World>();
    create_entity_allocator_(init_params);

    // Do rest of initialization
    resource_cache_->initialize(init_params);
    world_->initialize(init_params);
}

void Context::terminate()
{
    world_->terminate();
    resource_cache_->terminate();

    // Free all component memory
    while (comp_allocators_.begin() != comp_allocators_.end())
    {
        comp_allocators_.begin()->second->Reset();
        free(comp_allocators_.begin()->second);
    }

    // Free all entity memory
    ent_allocator_->Reset();
    free(ent_allocator_);
    ent_allocator_ = nullptr;

    // Free all resource memory
    while (resource_allocators_.begin() != resource_allocators_.end())
    {
        resource_allocators_.begin()->second->Reset();
        free(resource_allocators_.begin()->second);
    }

    free(world_);
    world_ = nullptr;

    free(resource_cache_);
    resource_cache_ = nullptr;

    // Kill logger last so that we can log stuff when needed
    logger_->terminate();
    free(logger_);
    logger_ = nullptr;
    mem_free_list_.Reset();
}

Logger * Context::get_logger()
{
    return logger_;
}

World * Context::get_world()
{
    return world_;
}

Resource_Cache * Context::get_resource_cache()
{
    return resource_cache_;
}

Context & Context::inst()
{
    return *s_this_;
}

void Context::raw_free(void * to_free)
{
    mem_free_list_.Free(to_free);
}

void * Context::malloc_(const rttr::type & type)
{
    sizet type_size = type.get_sizeof();
    if (type_size < MIN_ALLOC_SIZE)
        type_size = MIN_ALLOC_SIZE;
    return mem_free_list_.Allocate(type_size, MIN_ALIGN_SIZE);
}

PoolAllocator * Context::create_entity_allocator_(const Variant_Map & init_params)
{
    sizet ent_byte_size = sizeof(Entity);
    if (ent_byte_size < MIN_CHUNK_ALLOC_SIZE)
        ent_byte_size = MIN_CHUNK_ALLOC_SIZE;

    // Create the entity allocator based on settings
    u16 ent_alloc = DEFAULT_ENTITY_ALLOC;
    auto fiter = init_params.find(ENTITY_ALLOC_KEY);
    if (fiter != init_params.end())
    {
        if (fiter->second.is_type<u16>())
            ent_alloc = fiter->second.get_value<u16>();
        else
            wlog("Passed in value for key {} but value was of incorrect type", ENTITY_ALLOC_KEY);
    }

    ilog("Creating {} byte pool allocator for entities - enough for {} instances", ent_alloc * ent_byte_size, ent_alloc);
    ent_allocator_ = ns_ctxt.malloc<PoolAllocator>(ent_alloc * ent_byte_size, ent_byte_size);
    ent_allocator_->Init();
    return ent_allocator_;
}

PoolAllocator * Context::create_component_allocator_(const rttr::type & type, const Variant_Map & init_params)
{
    sizet type_size = type.get_sizeof();

    if (type_size < MIN_CHUNK_ALLOC_SIZE)
        type_size = MIN_CHUNK_ALLOC_SIZE;

    // Grab the overriding alloc amount if its there
    u16 alloc_amount = DEFAULT_COMP_ALLOC;
    String type_str(type.get_name());
    type_str += "_Alloc";
    auto fiter = init_params.find(type_str);
    if (fiter != init_params.end())
    {
        if (fiter->second.is_type<u16>())
            alloc_amount = fiter->second.get_value<u16>();
        else
            wlog("Passed in value for key {} but value was of incorrect type", type_str);
    }

    PoolAllocator * comp_alloc = ns_ctxt.malloc<PoolAllocator>(alloc_amount * type_size, type_size);

    comp_alloc->Init();
    comp_allocators_.emplace(type.get_id(), comp_alloc);
    String str(type.get_name());
    ilog("Creating {0} byte pool allocator for component type {1} - enough for {2} instances", alloc_amount * type_size, str, alloc_amount);
    return comp_alloc;
}

PoolAllocator * Context::create_resource_allocator_(const rttr::type & type, const Variant_Map & init_params)
{
    sizet type_size = type.get_sizeof();

    if (type_size < MIN_CHUNK_ALLOC_SIZE)
        type_size = MIN_CHUNK_ALLOC_SIZE;

    // Grab the overriding alloc amount if its there
    u16 alloc_amount = DEFAULT_RES_ALLOC;
    String type_str(type.get_name());
    type_str += "_Alloc";
    auto fiter = init_params.find(type_str);
    if (fiter != init_params.end())
    {
        if (fiter->second.is_type<u16>())
            alloc_amount = fiter->second.get_value<u16>();
        else
            wlog("Passed in value for key {} but value was of incorrect type", type_str);
    }

    PoolAllocator * res_alloc = ns_ctxt.malloc<PoolAllocator>(alloc_amount * type_size, type_size);

    res_alloc->Init();
    resource_allocators_.emplace(type.get_id(), res_alloc);
    String str(type.get_name());
    ilog("Creating {0} byte pool allocator for resource type {1} - enough for {2} instances", alloc_amount * type_size, str, alloc_amount);
    return res_alloc;
}

PoolAllocator * Context::get_comp_allocator(const rttr::type & type)
{
    auto fiter = comp_allocators_.find(type.get_id());
    if (fiter != comp_allocators_.end())
        return fiter->second;
    return nullptr;
}

PoolAllocator * Context::get_resource_allocator(const rttr::type & type)
{
    auto fiter = resource_allocators_.find(type.get_id());
    if (fiter != resource_allocators_.end())
        return fiter->second;
    return nullptr;
}

PoolAllocator * Context::get_entity_allocator()
{
    return ent_allocator_;
}

Factory * Context::get_base_factory(const rttr::type & obj_type)
{
    auto fiter = type_factories_.find(obj_type.get_id());
    if (fiter != type_factories_.end())
        return fiter->second;
    return nullptr;
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Context>("Context").constructor<>();
    // .property("id_", &Component::initialize, registration::public_access)
    // .property("terminate", &Component::terminate, registration::public_access)
    // .property("owner_id", &Component::owner_id_, registration::private_access);
}
