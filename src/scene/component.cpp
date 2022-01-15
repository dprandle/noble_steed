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

void Component::initialize(const Variant_Map & init_params)
{
}

void Component::terminate()
{
}

} // namespace noble_steed