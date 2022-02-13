#pragma once

#include "vector2.h"

namespace noble_steed
{
template<class T>
struct vector4;

template<floating_pt T>
struct quaternion;

template<class T>
struct nsmat4;

template<class T>
struct vector3
{
    enum coord_sys
    {
        Cartesian,
        Cylindrical,
        Spherical
    };

    enum euler_order
    {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    explicit vector3(T val_ = 0) : x(val_), y(val_), z(val_)
    {}
    vector3(const vector2<T> &xy_, T z_) : xy(xy_), _z(z_)
    {}
    vector3(T x_, const vector2<T> &yz_) : _x(x_), yz(yz_)
    {}
    vector3(T x_, T y_, T z_ = 0) : x(x_), y(y_), z(z_)
    {}

    COMMON_OPERATORS(vector3<T>, 3)

    union
    {
        T data[3];

        struct
        {
            T x;
            T y;
            T z;
        };

        struct
        {
            T r;
            T g;
            T b;
        };

        struct
        {
            T s;
            T t;
            T p;
        };

        struct
        {
            T rad;
            T theta;
            T phi;
        };

        struct
        {
            vector2<T> xy;
            T _z;
        };

        struct
        {
            T _x;
            vector2<T> yz;
        };
    };
};

namespace math
{

template<basic_number T>
void cross(vector3<T> *srcvec, const vector3<T> &cross_with_)
{
    T tmpx = srcvec->x, tmpy = srcvec->y;
    srcvec->x = srcvec->y * cross_with_.z - srcvec->z * cross_with_.y;
    srcvec->y = srcvec->z * cross_with_.x - tmpx * cross_with_.z;
    srcvec->z = tmpx * cross_with_.y - tmpy * cross_with_.x;
}

template<basic_number T>
vector3<T> cross(vector3<T> lhs, const vector3<T> &rhs)
{
    cross(&lhs, rhs);
    return lhs;
}

template<floating_pt T>
vector3<T> spherical_to_cartesian(const vector3<T> &spherical)
{
    vector3<T> ret;
    ret.x = spherical.rad * math::cos(spherical.theta) * math::sin(spherical.phi);
    ret.y = spherical.rad * math::sin(spherical.theta) * math::sin(spherical.phi);
    ret.z = spherical.rad * math::cos(spherical.phi);
    return ret;
}

template<floating_pt T>
vector3<T> cylindrical_to_cartesian(const vector3<T> &cylindrical)
{
    vector3<T> ret;
    ret.x = cylindrical.rad * math::cos(cylindrical.theta);
    ret.y = cylindrical.rad * math::sin(cylindrical.theta);
    ret.z = cylindrical.z;
    return ret;
}

template<floating_pt T>
vector3<T> cartesian_to_cylindrical(const vector3<T> &cartesian)
{
    vector3<T> ret;
    ret.x = math::sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y);

    if (fequals(cartesian.x, 0))
    {
        ret.y = math::PI / 2;
        if (fequals(cartesian.y, 0))
            ret.y = 0;
    }
    else
        ret.y = math::atan(cartesian.y, cartesian.x);

    ret.z = cartesian.z;
    return ret;
}

template<floating_pt T>
vector3<T> cartesian_to_spherical(const vector3<T> &cartesian)
{
    vector3<T> ret;
    ret.x = length(cartesian);

    if (fequals(cartesian.x, 0))
    {
        ret.y = math::PI / 2;
        if (fequals(cartesian.y, 0))
            ret.y = 0;
    }
    else
        ret.y = math::atan(cartesian.y, cartesian.x);

    if (ret.x == 0)
        ret.z = 0;
    else
        ret.z = math::acos(cartesian.z / ret.x);
    return ret;
}

template<class T>
vector3<T> scaling_vec3(const nsmat3<T> &transform_)
{
    vector3<T> ret;
    ret.x = length(transform_[0]);
    ret.y = length(transform_[1]);
    ret.z = length(transform_[2]);
    return ret;
}

template<class T>
vector3<T> translation_vec3(const nsmat4<T> &transform_)
{
    return transform_(3).vec3();
}

template<floating_pt T>
vector3<T> euler_vec3(const nsmat3<T> &rotation_mat3, typename vector3<T>::euler_order order = vector3<T>::XYZ)
{
    // https://github.com/mrdoob/three.js/blob/master/src/math/Euler.js
    vector3<T> ret;
    T ep = 1 - math::FLOAT_EPS;
    switch (order)
    {
    case (vector2<T>::XYZ):
        ret.y = math::asin(rotation_mat3[0][2]);
        if (std::abs(rotation_mat3[0][2]) < ep)
        {
            ret.x = math::atan(-rotation_mat3[1][2], rotation_mat3[2][2]);
            ret.z = math::atan(-rotation_mat3[0][1], rotation_mat3[0][0]);
        }
        else
        {
            ret.x = math::atan(rotation_mat3[2][1], rotation_mat3[1][1]);
            ret.z = 0;
        }
        break;
    case (vector2<T>::XZY):
        ret.z = math::asin(rotation_mat3[0][1]);
        if (std::abs(rotation_mat3[0][1]) < ep)
        {
            ret.x = math::atan(rotation_mat3[2][1], rotation_mat3[1][1]);
            ret.y = math::atan(rotation_mat3[0][2], rotation_mat3[0][0]);
        }
        else
        {
            ret.x = math::atan(-rotation_mat3[1][2], rotation_mat3[2][2]);
            ret.y = 0;
        }
        break;
    case (vector2<T>::YXZ):
        ret.x = math::asin(rotation_mat3[1][2]);
        if (std::abs(rotation_mat3[1][2]) < ep)
        {
            ret.y = math::atan(rotation_mat3[0][2], rotation_mat3[2][2]);
            ret.z = math::atan(rotation_mat3[1][0], rotation_mat3[1][1]);
        }
        else
        {
            ret.y = math::atan(-rotation_mat3[2][0], rotation_mat3[0][0]);
            ret.z = 0;
        }
        break;
    case (vector2<T>::YZX):
        ret.z = math::asin(rotation_mat3[1][0]);
        if (std::abs(rotation_mat3[1][0]) < ep)
        {
            ret.x = math::atan(-rotation_mat3[1][2], rotation_mat3[1][1]);
            ret.y = math::atan(-rotation_mat3[2][0], rotation_mat3[0][0]);
        }
        else
        {
            ret.x = 0;
            ret.y = math::atan(rotation_mat3[0][2], rotation_mat3[2][2]);
        }
        break;
    case (vector2<T>::ZXY):
        ret.x = math::asin(rotation_mat3[2][1]);
        if (std::abs(rotation_mat3[2][1]) < ep)
        {
            ret.y = math::atan(-rotation_mat3[2][0], rotation_mat3[2][2]);
            ret.z = math::atan(-rotation_mat3[0][1], rotation_mat3[1][1]);
        }
        else
        {
            ret.y = 0;
            ret.z = math::atan(rotation_mat3[1][0], rotation_mat3[0][0]);
        }
        break;
    case (vector2<T>::ZYX):
        ret.y = math::asin(rotation_mat3[2][0]);
        if (std::abs(rotation_mat3[2][0]) < ep)
        {
            ret.x = math::atan(rotation_mat3[2][1], rotation_mat3[2][2]);
            ret.z = math::atan(rotation_mat3[1][0], rotation_mat3[0][0]);
        }
        else
        {
            ret.x = 0;
            ret.z = math::atan(-rotation_mat3[0][1], rotation_mat3[1][1]);
        }
        break;
    }
    return ret;
}
} // namespace math

// Enable type trait
template<class U>
struct is_vec<vector3<U>>
{
    static constexpr bool value = true;
};

using i8vec3 = vector3<i8>;
using i16vec3 = vector3<i16>;
using ivec3 = vector3<i32>;
using i64vec3 = vector3<i64>;
using u8vec3 = vector3<u8>;
using u16vec3 = vector3<u16>;
using uvec3 = vector3<u32>;
using u64vec3 = vector3<u64>;
using vec3 = vector3<float>;
using dvec3 = vector3<double>;
using ldvec3 = vector3<ldouble>;
} // namespace noble_steed