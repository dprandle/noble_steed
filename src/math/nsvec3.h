#pragma once

#include "algorithm.h"
#include "vector2.h"

namespace noble_steed
{
template<class T>
struct nsvec4;

template<class T>
struct nsquat;

template<class T>
struct nsmat4;

template<class T>
struct nsvec3
{
    enum CoordSys
    {
        Cartesian,
        Cylindrical,
        Spherical
    };

    enum EulerOrder
    {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    nsvec3(const nsvec3<T> &copy_) : x(copy_.x), y(copy_.y), z(copy_.z)
    {}

    nsvec3(const T &val_ = 0) : x(val_), y(val_), z(val_)
    {}
    nsvec3(const vector2<T> &xy, const T &z_) : x(xy.x), y(xy.y), z(z_)
    {}
    nsvec3(const T &x_, const vector2<T> &yz) : x(x_), y(yz.y), z(yz.z)
    {}
    nsvec3(const T &x_, const T &y_, const T &z_ = 0) : x(x_), y(y_), z(z_)
    {}

    nsvec3<T> &abs()
    {
        x = std::abs(x);
        y = std::abs(y);
        z = std::abs(z);
        return *this;
    }

    T angle_to(const nsvec3<T> vec_, bool rads_ = false) const
    {
        T dotP = (*this) * vec_;
        T l = length() * vec_.length();

        if (l == 0)
            return 0;

        dotP /= l;

        if (rads_)
            return T(acos(dotP));
        else
            return T(degrees(acos(dotP)));
    }

    nsvec3<T> &ceil()
    {
        x = std::ceil(x);
        y = std::ceil(y);
        z = std::ceil(z);
        return *this;
    }

    nsvec3<T> &cross(const nsvec3<T> &cross_with_)
    {
        T tmpx = x, tmpy = y;
        x = y * cross_with_.z - z * cross_with_.y;
        y = z * cross_with_.x - tmpx * cross_with_.z;
        z = tmpx * cross_with_.y - tmpy * cross_with_.x;
        return *this;
    }

    nsvec3<T> cylindrical(bool rads_ = false) const
    {
        nsvec3<T> ret;
        ret.x = sqrt(x * x + y * y);

        if (x == 0)
        {
            ret.y = math::PI / 2;
            if (y == 0)
                ret.y = 0;
        }
        else
            ret.y = atan2(y, x);

        ret.z = z;

        if (!rads_)
            ret.y = degrees(ret.y);

        return ret;
    }

    T distance_to(const nsvec3<T> &vec_) const
    {
        return ((vec_ - *this).length());
    }

    nsvec3<T> &euler_from(const nsvec4<T> &axis_angle_, EulerOrder order_ = XYZ, bool rads_ = false)
    {
        return euler_from(nsquat<T>().from(axis_angle_, rads_), order_, rads_);
    }

    nsvec3<T> &euler_from(const nsquat<T> &orientation_, EulerOrder order_, bool rads_)
    {
        return euler_from(nsmat3<T>().rotationFrom(orientation_), order_, rads_);
    }

    nsvec3<T> &euler_from(const nsmat3<T> &rotation_mat3_, EulerOrder order_ = XYZ, bool rads_ = false)
    {
        // https://github.com/mrdoob/three.js/blob/master/src/math/Euler.js
        T ep = 1 - math::FLOAT_EPS;
        switch (order_)
        {
        case (XYZ):
            y = std::asin(rotation_mat3_[0][2]);
            if (std::abs(rotation_mat3_[0][2]) < ep)
            {
                x = std::atan2(-rotation_mat3_[1][2], rotation_mat3_[2][2]);
                z = std::atan2(-rotation_mat3_[0][1], rotation_mat3_[0][0]);
            }
            else
            {
                x = std::atan2(rotation_mat3_[2][1], rotation_mat3_[1][1]);
                z = 0;
            }
            break;
        case (XZY):
            z = std::asin(rotation_mat3_[0][1]);
            if (std::abs(rotation_mat3_[0][1]) < ep)
            {
                x = std::atan2(rotation_mat3_[2][1], rotation_mat3_[1][1]);
                y = std::atan2(rotation_mat3_[0][2], rotation_mat3_[0][0]);
            }
            else
            {
                x = std::atan2(-rotation_mat3_[1][2], rotation_mat3_[2][2]);
                y = 0;
            }
            break;
        case (YXZ):
            x = std::asin(rotation_mat3_[1][2]);
            if (std::abs(rotation_mat3_[1][2]) < ep)
            {
                y = std::atan2(rotation_mat3_[0][2], rotation_mat3_[2][2]);
                z = std::atan2(rotation_mat3_[1][0], rotation_mat3_[1][1]);
            }
            else
            {
                y = std::atan2(-rotation_mat3_[2][0], rotation_mat3_[0][0]);
                z = 0;
            }
            break;
        case (YZX):
            z = std::asin(rotation_mat3_[1][0]);
            if (std::abs(rotation_mat3_[1][0]) < ep)
            {
                x = std::atan2(-rotation_mat3_[1][2], rotation_mat3_[1][1]);
                y = std::atan2(-rotation_mat3_[2][0], rotation_mat3_[0][0]);
            }
            else
            {
                x = 0;
                y = std::atan2(rotation_mat3_[0][2], rotation_mat3_[2][2]);
            }
            break;
        case (ZXY):
            x = std::asin(rotation_mat3_[2][1]);
            if (std::abs(rotation_mat3_[2][1]) < ep)
            {
                y = std::atan2(-rotation_mat3_[2][0], rotation_mat3_[2][2]);
                z = std::atan2(-rotation_mat3_[0][1], rotation_mat3_[1][1]);
            }
            else
            {
                y = 0;
                z = std::atan2(rotation_mat3_[1][0], rotation_mat3_[0][0]);
            }
            break;
        case (ZYX):
            y = std::asin(rotation_mat3_[2][0]);
            if (std::abs(rotation_mat3_[2][0]) < ep)
            {
                x = std::atan2(rotation_mat3_[2][1], rotation_mat3_[2][2]);
                z = std::atan2(rotation_mat3_[1][0], rotation_mat3_[0][0]);
            }
            else
            {
                x = 0;
                z = std::atan2(-rotation_mat3_[0][1], rotation_mat3_[1][1]);
            }
            break;
        }
        if (!rads_)
            *this = degrees(*this);
        return *this;
    }

    nsvec3<T> &euler_from(const nsmat4<T> &transform_, EulerOrder order_ = XYZ, bool rads_ = false)
    {
        return euler_from(rotation_mat3_(transform_), order_, rads_);
    }

    nsvec3<T> &euler_from(const nsvec3<T> &vec_, const nsvec3<T> &to_vec_, EulerOrder order_ = XYZ, bool rads_ = false)
    {
        return euler_from(nsquat<T>().from(vec_, to_vec_), order_, rads_);
    }

    nsvec3<T> &floor()
    {
        x = std::floor(x);
        y = std::floor(y);
        z = std::floor(z);
        return *this;
    }

    nsvec3<T> &fract()
    {
        for (u32 i = 0; i < 3; ++i)
        {
            if (data[i] >= 0)
                data[i] -= std::floor(data[i]);
            else
                data[i] -= std::ceil(data[i]);
        }
        return *this;
    }

    nsvec3<T> &from(CoordSys coord_sys_, const nsvec3<T> &vec_, bool rads_ = false)
    {
        switch (coord_sys_)
        {
        case (Cylindrical): {
            T theta = vec_.y;
            if (!rads_)
                theta = radians(theta);
            x = vec_.x * cos(theta);
            y = vec_.x * sin(theta);
            z = vec_.z;
            break;
        }
        case (Spherical): {
            T theta = vec_.y;
            T phi = vec_.z;
            if (!rads_)
            {
                theta = radians(theta);
                phi = radians(phi);
            }
            x = vec_.x * cos(theta) * sin(phi);
            y = vec_.x * sin(theta) * sin(phi);
            z = vec_.x * cos(phi);
            break;
        }
        default:
            *this = vec_;
        }
        return *this;
    }

    T length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    T length_sq() const
    {
        return x * x + y * y + z * z;
    }

    template<class T2>
    nsvec3<T> &lerp(const nsvec3<T> &vec_, const T2 &scaling_factor_)
    {
        x += (vec_.x - x * scaling_factor_);
        y += (vec_.y - y * scaling_factor_);
        z += (vec_.z - z * scaling_factor_);
        return *this;
    }

    vector2<T> minmax() const
    {
        auto pr = std::minmax({x,y,z,});
        return {pr.first, pr.second};
    }

    nsvec3<T> &minimize(const nsvec3<T> &rhs_)
    {
        if (x > rhs_.x)
            x = rhs_.x;
        if (y > rhs_.y)
            y = rhs_.y;
        if (z > rhs_.z)
            z = rhs_.z;
        return *this;
    }

    nsvec3<T> &maximize(const nsvec3<T> &rhs_)
    {
        if (x < rhs_.x)
            x = rhs_.x;
        if (y < rhs_.y)
            y = rhs_.y;
        if (z < rhs_.z)
            z = rhs_.z;
        return *this;
    }

    nsvec3<T> &normalize()
    {
        T l = length();
        if (l <= math::FLOAT_EPS)
            return *this;
        return *this *= (1 / l);
    }

    nsvec3<T> &project(const nsvec3<T> &vec_)
    {
        T denom = vec_ * vec_;
        if (denom == 0)
            return *this;
        (*this) = ((*this * vec_) / denom) * vec_;
        return *this;
    }

    nsvec3<T> &project_plane(const nsvec3<T> &plane_normal_)
    {
        nsvec3<T> aonb(*this);
        aonb.project(plane_normal_);
        (*this) -= aonb;
        return *this;
    }

    nsvec3<T> &reflect(const nsvec3<T> &normal_)
    {
        (*this) = (*this) - (2 * (normal_ * *this)) * normal_;
        return *this;
    }

    nsvec3<T> &round()
    {
        x = std::round(x);
        y = std::round(y);
        z = std::round(z);
        return *this;
    }

    nsvec3<T> &scaling_from(const nsmat3<T> &transform_)
    {
        x = transform_[0].length();
        y = transform_[1].length();
        z = transform_[2].length();
        return *this;
    }

    nsvec3<T> &scaling_from(const nsmat4<T> &transform_)
    {
        x = sqrt(transform_[0][0] * transform_[0][0] + transform_[0][1] * transform_[0][1] + transform_[0][2] * transform_[0][2]);
        y = sqrt(transform_[1][0] * transform_[1][0] + transform_[1][1] * transform_[1][1] + transform_[1][2] * transform_[1][2]);
        z = sqrt(transform_[2][0] * transform_[2][0] + transform_[2][1] * transform_[2][1] + transform_[2][2] * transform_[2][2]);
        return *this;
    }

    nsvec3<T> &translation_from(const nsmat4<T> &transform_)
    {
        return *this = transform_(3).xyz();
    }

    nsvec3<T> &set_length(const T &len_)
    {
        T l = length();

        if (l == 0)
            return *this;

        T mult = len_ / l;
        (*this) *= mult;
        return *this;
    }

    nsvec3<T> spherical(bool rads_ = false) const
    {
        nsvec3<T> ret;
        ret.x = length();

        if (x == 0)
        {
            ret.y = math::PI / 2;
            if (y == 0)
                ret.y = 0;
        }
        else
            ret.y = atan2(y, x);

        if (ret.x == 0)
            ret.z = 0;
        else
            ret.z = acos(z / ret.x);

        if (!rads_)
        {
            ret.y = degrees(ret.y);
            ret.z = degrees(ret.z);
        }

        return ret;
    }

    vector2<T> vec2() const
    {
        return vector2<T>(x, y);
    }

    // overloaded operators
    nsvec3<T> operator+(const nsvec3<T> &rhs_) const
    {
        return nsvec3<T>(x + rhs_.x, y + rhs_.y, z + rhs_.z);
    }

    nsvec3<T> operator-(const nsvec3<T> &rhs_) const
    {
        return nsvec3<T>(x - rhs_.x, y - rhs_.y, z - rhs_.z);
    }

    T operator*(const nsvec3<T> &rhs_) const // dot product
    {
        return x * rhs_.x + y * rhs_.y + z * rhs_.z;
    }

    nsmat3<T> operator^(const nsvec3<T> &rhs_) const
    {
        nsmat3<T> ret;
        ret[0] = x * rhs_;
        ret[1] = y * rhs_;
        ret[2] = z * rhs_;
        return ret;
    }

    nsvec3<T> operator%(const nsvec3<T> &rhs_) const // component wise scalar product
    {
        return nsvec3<T>(x * rhs_.x, y * rhs_.y, z * rhs_.z);
    }

    nsvec3<T> operator/(const nsvec3<T> &rhs_) const
    {
        return nsvec3<T>(x / rhs_.x, y / rhs_.y, z / rhs_.z);
    }

    nsvec3<T> operator*(const T &rhs_) const
    {
        return nsvec3<T>(x * rhs_, y * rhs_, z * rhs_);
    }

    nsvec3<T> operator/(const T &rhs_) const
    {
        return nsvec3<T>(x / rhs_, y / rhs_, z / rhs_);
    }

    nsvec3<T> &operator=(const nsvec3<T> &rhs_)
    {
        if (this == &rhs_)
            return *this;
        x = rhs_.x;
        y = rhs_.y;
        z = rhs_.z;
        return *this;
    }

    nsvec3<T> operator++(i32)
    {
        nsvec3<T> ret(*this);
        ++(*this);
        return ret;
    }

    nsvec3<T> operator--(i32)
    {
        nsvec3<T> ret(*this);
        --(*this);
        return ret;
    }

    nsvec3<T> &operator++()
    {
        ++x;
        ++y;
        ++z;
        return *this;
    }

    nsvec3<T> &operator--()
    {
        --x;
        --y;
        --z;
        return *this;
    }

    nsvec3<T> &operator+=(const nsvec3<T> &rhs_)
    {
        x += rhs_.x;
        y += rhs_.y;
        z += rhs_.z;
        return *this;
    }

    nsvec3<T> &operator-=(const nsvec3<T> &rhs_)
    {
        x -= rhs_.x;
        y -= rhs_.y;
        z -= rhs_.z;
        return *this;
    }

    nsvec3<T> &operator%=(const nsvec3<T> &rhs_)
    {
        x *= rhs_.x;
        y *= rhs_.y;
        z *= rhs_.z;
        return *this;
    }

    nsvec3<T> &operator/=(const nsvec3<T> &rhs_)
    {
        x /= rhs_.x;
        y /= rhs_.y;
        z /= rhs_.z;
        return *this;
    }

    nsvec3<T> &operator*=(const T &rhs_)
    {
        x *= rhs_;
        y *= rhs_;
        z *= rhs_;
        return *this;
    }

    nsvec3<T> &operator/=(const T &rhs_)
    {
        x /= rhs_;
        y /= rhs_;
        z /= rhs_;
        return *this;
    }

    bool operator==(const nsvec3<T> &rhs_) const
    {
        return ((x == rhs_.x) && (y == rhs_.y) && (z == rhs_.z));
    }

    bool operator!=(const nsvec3<T> &rhs_) const
    {
        return !(*this == rhs_);
    }

    bool operator==(const T &rhs_) const
    {
        return ((x == rhs_) && (y == rhs_) && (z == rhs_));
    }

    bool operator!=(const T &rhs_) const
    {
        return !(*this == rhs_);
    }

    const T &operator[](sizet val_) const
    {
        assert(val_ < 3 && "Index out of range!");
        return data[val_];
    }

    T &operator[](sizet val_)
    {
        assert(val_ < 3 && "Index out of range!");
        return data[val_];
    }

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
    };
};

// template<class T>
// nsvec3<T> operator*(const i32 &lhs_, const nsvec3<T> &rhs_)
// {
//     return rhs_ * lhs_;
// }

// template<class T>
// nsvec3<T> operator*(const float &lhs_, const nsvec3<T> &rhs_)
// {
//     return rhs_ * lhs_;
// }

// template<class T>
// nsvec3<T> operator*(const double &lhs_, const nsvec3<T> &rhs_)
// {
//     return rhs_ * lhs_;
// }

// template<class T>
// nsvec3<T> operator/(const i32 &lhs_, const nsvec3<T> &rhs_)
// {
//     return nsvec3<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z);
// }

// template<class T>
// nsvec3<T> operator/(const float &lhs_, const nsvec3<T> &rhs_)
// {
//     return nsvec3<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z);
// }

// template<class T>
// nsvec3<T> operator/(const double &lhs_, const nsvec3<T> &rhs_)
// {
//     return nsvec3<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z);
// }

// template<class T>
// nsvec3<T> abs(const nsvec3<T> &vec_)
// {
//     return nsvec3<T>(vec_).abs();
// }

// template<class T>
// nsvec3<T> ceil(const nsvec3<T> &vec_)
// {
//     nsvec3<T> ret(vec_);
//     ret.ceiling();
//     return ret;
// }

// template<class T>
// nsvec3<T> cross(const nsvec3<T> &lhs_, const nsvec3<T> &rhs_)
// {
//     nsvec3<T> ret;
//     ret.x = lhs_.y * rhs_.z - lhs_.z * rhs_.y;
//     ret.y = lhs_.z * rhs_.x - lhs_.x * rhs_.z;
//     ret.z = lhs_.x * rhs_.y - lhs_.y * rhs_.x;
//     return ret;
// }

// template<class T>
// T distance(const nsvec3<T> &lvec_, const nsvec3<T> &rvec_)
// {
//     return lvec_.distanceTo(rvec_);
// }

// template<class T>
// T dot(const nsvec3<T> &lhs_, const nsvec3<T> &rhs_)
// {
//     return lhs_ * rhs_;
// }

// template<class T>
// nsvec3<T> euler(const nsvec4<T> &axis_angle_, typename nsvec3<T>::EulerOrder order_, bool rads_)
// {
//     return nsvec3<T>().euler_from(nsquat<T>().from(axis_angle_, rads_), order_, rads_);
// }

// template<class T>
// nsvec3<T> euler(const nsquat<T> &orientation_, typename nsvec3<T>::EulerOrder order_, bool rads_)
// {
//     return nsvec3<T>().euler_from(nsmat3<T>().rotation_from(orientation_), order_, rads_);
// }

// template<class T>
// nsvec3<T> euler(const nsmat3<T> &rotation_mat3_, typename nsvec3<T>::EulerOrder order_, bool rads_)
// {
//     return nsvec3<T>().euler_from(rotation_mat3_, order_, rads_);
// }

// template<class T>
// nsvec3<T> euler(const nsmat4<T> &transform_, typename nsvec3<T>::EulerOrder order_, bool rads_)
// {
//     return nsvec3<T>().euler_from(transform_, order_, rads_);
// }

// template<class T>
// nsvec3<T> euler(const nsvec3<T> &vec_, const nsvec3<T> &to_vec_, typename nsvec3<T>::EulerOrder order_, bool rads_)
// {
//     return nsvec3<T>().euler_from(nsquat<T>().from(vec_, to_vec_), order_, rads_);
// }

// template<class T>
// nsvec3<T> floor(const nsvec3<T> &vec_)
// {
//     nsvec3<T> ret(vec_);
//     ret.floor();
//     return ret;
// }

// template<class T>
// nsvec3<T> fract(const nsvec3<T> &vec_)
// {
//     return nsvec3<T>(vec_).fract();
// }

// template<class T>
// T length(const nsvec3<T> &vec_)
// {
//     return vec_.length();
// }

// template<class T, class T2>
// nsvec3<T> lerp(const nsvec3<T> &lhs_, const nsvec3<T> &rhs_, T2 scaling_factor_)
// {
//     nsvec3<T> ret(lhs_);
//     ret.lerp(rhs_, scaling_factor_);
//     return ret;
// }

// template<class T>
// nsvec3<T> min(const nsvec3<T> &lhs_, const nsvec3<T> &rhs_)
// {
//     nsvec3<T> ret(lhs_);
//     ret.minimize(rhs_);
//     return ret;
// }

// template<class T>
// nsvec3<T> max(const nsvec3<T> &lhs_, const nsvec3<T> &rhs_)
// {
//     nsvec3<T> ret(lhs_);
//     ret.maximize(rhs_);
//     return ret;
// }

// template<class T>
// nsvec3<T> normalize(const nsvec3<T> &rhs_)
// {
//     nsvec3<T> ret(rhs_);
//     ret.normalize();
//     return ret;
// }

// template<class T>
// nsvec3<T> project(const nsvec3<T> &a, const nsvec3<T> &b)
// {
//     nsvec3<T> ret(a);
//     ret.project(b);
//     return ret;
// }

// template<class T>
// nsvec3<T> project_plane(const nsvec3<T> &a, const nsvec3<T> &normal_)
// {
//     nsvec3<T> ret(a);
//     ret.project_plane(normal_);
//     return ret;
// }

// template<class T>
// nsvec3<T> reflect(const nsvec3<T> &incident_, const nsvec3<T> &normal_)
// {
//     nsvec3<T> ret(incident_);
//     ret.reflect(normal_);
//     return ret;
// }

// template<class T>
// nsvec3<T> round(const nsvec3<T> &vec_)
// {
//     nsvec3<T> ret(vec_);
//     ret.round();
//     return ret;
// }

// template<class T>
// nsvec3<T> scaling_vec(const nsmat3<T> &transform_)
// {
//     return nsvec3<T>().scaling_from(transform_);
// }

// template<class T>
// nsvec3<T> scaling_vec(const nsmat4<T> &transform_)
// {
//     return nsvec3<T>().scaling_from(transform_);
// }

// template<class T>
// nsvec3<T> translation_vec(const nsmat4<T> &transform_)
// {
//     return nsvec3<T>().translation_from(transform_);
// }

template<class T>
std::ostream& operator<<(std::ostream& os, const nsvec3<T>& vec)
{
    using namespace math;
    os << STR_ALIGN << PRINT_START_VEC << vec.x << PRINT_VEC_DELIMITER << vec.y << PRINT_VEC_DELIMITER << vec.z << PRINT_END_VEC;
    return os;
}


using i8vec3 = nsvec3<i8>;
using i16vec3 = nsvec3<i16>;
using ivec3 = nsvec3<i32>;
using i64vec3 = nsvec3<i64>;
using u8vec3 = nsvec3<u8>;
using u16vec3 = nsvec3<u16>;
using uvec3 = nsvec3<u32>;
using u64vec3 = nsvec3<u64>;
using vec3 = nsvec3<float>;
using dvec3 = nsvec3<double>;
using ldvec3 = nsvec3<ldouble>;
}