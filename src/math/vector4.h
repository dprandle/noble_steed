#pragma once

#include "vector3.h"

namespace noble_steed
{
template<class T>
struct vector4
{
    vector4(T val = 0) : x(val), y(val), z(val), w(val)
    {}
    vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_)
    {}
    vector4(const vector3<T> &xyz_, T w_ = 1) : xyz(xyz_), _wvec3(w_)
    {}
    vector4(T x_, const vector3<T> &yzw_) : _xvec3(x_), yzw(yzw_)
    {}
    vector4(const vector2<T> &xy_, const vector2<T> &zw_) : xy(xy_), zw(zw_)
    {}
    vector4(T x_, const vector2<T> &yz_, T w_) : _xvec2(x_), yz(yz_), _wvec2(w_)
    {}

    COMMON_OPERATORS(vector4<T>, 4)

    union
    {
        T data[4];

        struct
        {
            T x;
            T y;
            T z;
            T w;
        };

        struct
        {
            T r;
            T g;
            T b;
            T a;
        };

        struct
        {
            T s;
            T t;
            T p;
            T q;
        };

        struct
        {
            vector3<T> xyz;
            T _wvec3;
        };

        struct
        {
            T _xvec3;
            vector3<T> yzw;
        };

        struct
        {
            vector2<T> xy;
            vector2<T> zw;
        };

        struct
        {
            T _xvec2;
            vector2<T> yz;
            T _wvec2;
        };
    };
};

namespace math
{

template<floating_pt T>
vector4<T> axis_angle(const quaternion<T> &orientation)
{
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
    vector4<T> ret;
    ret.w = 2 * math::acos(orientation.w);
    T den = math::sqrt(1 - orientation.w * orientation.w);
    if (den < math::FLOAT_EPS)
    {
        ret.x = orientation.x;
        ret.y = orientation.y;
        ret.z = orientation.z;
    }
    else
    {
        ret.x = orientation.x / den;
        ret.y = orientation.y / den;
        ret.z = orientation.z / den;
    }
    return ret;
}

} // namespace math

// Enable type trait
template<class U>
struct is_vec<vector4<U>>
{
    static constexpr bool value = true;
};


using i8vec4 = vector4<i8>;
using i16vec4 = vector4<i16>;
using ivec4 = vector4<i32>;
using i64vec4 = vector4<i64>;
using u8vec4 = vector4<u8>;
using u16vec4 = vector4<u16>;
using uvec4 = vector4<u32>;
using u64vec4 = vector4<u64>;
using vec4 = vector4<float>;
using dvec4 = vector4<double>;
using ldvec4 = vector4<ldouble>;
} // namespace noble_steed