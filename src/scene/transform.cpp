#include <noble_steed/scene/transform.h>
#include <noble_steed/core/logger.h>

namespace noble_steed
{
Transform::Transform()
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

    registration::class_<Transform>("Transform").constructor<>();
}
