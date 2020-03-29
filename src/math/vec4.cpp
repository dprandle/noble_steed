#include <noble_steed/math/vec4.h>

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace noble_steed;

    registration::class_<vec4>("noble_steed::vec4")
        .constructor<>()
        .property("x",&vec4::x,registration::public_access)
        .property("y",&vec4::y,registration::public_access)
        .property("z",&vec4::z,registration::public_access)
        .property("w",&vec4::w,registration::public_access);
}