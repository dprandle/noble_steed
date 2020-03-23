#include <noble_steed/scene/component.h>
#include <noble_steed/container/string.h>
#include <noble_steed/core/logger.h>

namespace noble_steed
{
Component::Component() : owner_id_(0)
{}

Component::~Component()
{}

void Component::initialize(const Variant_Map & init_params)
{
    ilog("Initializing component type {} and owner id {}",
         String(get_derived_info().m_type.get_name()),
         owner_id_);
}

void Component::terminate()
{
    ilog("Terminating component type {} and owner id {}",
         String(get_derived_info().m_type.get_name()),
         owner_id_);
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
        .property("owner_id", &Component::owner_id_, registration::private_access);
}
