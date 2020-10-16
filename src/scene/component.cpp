#include <noble_steed/scene/component.h>
#include <noble_steed/container/string.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/core/context.h>

namespace noble_steed
{
Component::Component() : Context_Obj(), owner_id_(0)
{}

Component::Component(const Component & copy): Context_Obj(copy), owner_id_(0)
{}

Component & Component::operator=(Component rhs)
{
    swap(rhs);
    return *this;
}

void Component::swap(Component & rhs)
{
    Context_Obj::swap(rhs);
    std::swap(owner_id_,rhs.owner_id_);
}

Component::~Component()
{}

void Component::initialize(const Variant_Hash & init_params)
{
    ilog("Initializing component type {} and owner id {}", String(get_derived_info().m_type.get_name()), owner_id_);
}

void Component::terminate()
{
    ilog("Terminating component type {} and owner id {}", String(get_derived_info().m_type.get_name()), owner_id_);
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Component>("noble_steed::Component").property("owner_id", &Component::owner_id_, registration::public_access);
}
