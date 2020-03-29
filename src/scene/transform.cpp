#include <noble_steed/scene/transform.h>
#include <noble_steed/core/logger.h>

namespace noble_steed
{
Transform::Transform():dummy_(10)
{}

Transform::~Transform()
{
}

} // namespace noble_steed

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<Transform>("noble_steed::Transform").constructor<>()
    .property("position",&Transform::position_,registration::public_access)
    .property("dummy",&Transform::dummy_,registration::public_access);
}
