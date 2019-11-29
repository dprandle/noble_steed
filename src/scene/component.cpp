#include <noble_steed/scene/component.h>
#include <noble_steed/container/string.h>
#include <noble_steed/core/logger.h>

namespace noble_steed
{
Component::Component() : id_(0), owner_id_(0)
{}

Component::~Component()
{}

void Component::initialize()
{
    ilog("Initializing base component with derived type {} and id {}",
         String(get_derived_info().m_type.get_name()), id_);
}

void Component::terminate()
{
    ilog("Terminating base component with derived type {} and id {}",
         String(get_derived_info().m_type.get_name()), id_);
}

void Component::set_id(uint32_t id)
{
    uint32_t old_id = id_;
    id_ = id;
    if (old_id && (old_id != id_))
    {
        ilog("Re-assigning component id from {} to {}", old_id, id_);
        id_change(glm::ivec2(old_id, id_));
    }
}

uint32_t Component::get_id()
{
    return id_;
}

void Component::set_owner_id(uint32_t owner_id)
{
    uint32_t old_id = owner_id_;
    owner_id_ = owner_id;
    if (old_id && (old_id != owner_id_))
    {
        ilog("Re-assigning component owner_id from {} to {}", old_id, owner_id_);
        owner_id_change(glm::ivec2(old_id, owner_id_));
    }
}

uint32_t Component::get_owner_id()
{
    return owner_id_;
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Component>("Component")
        .constructor<>()
        .method("initialize", &Component::initialize, registration::public_access)
        .method("terminate", &Component::terminate, registration::public_access)
        .property("id", &Component::get_id, &Component::set_id)
        .property("owner_id", &Component::get_owner_id, &Component::set_owner_id);
}
