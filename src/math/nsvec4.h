#pragma once

#include "nsvec3.h"

template<class T>
struct nsvec4
{
    nsvec4(const nsvec4<T> &copy_) : x(copy_.x), y(copy_.y), z(copy_.z), w(copy_.w)
    {}
    nsvec4(const T &val = static_cast<T>(0)) : x(val), y(val), z(val), w(val)
    {}
    nsvec4(const T &x_, const T &y_, const T &z_ = static_cast<T>(0), const T &w_ = static_cast<T>(0)) : x(x_), y(y_), z(z_), w(w_)
    {}
    nsvec4(const nsvec3<T> &xyz, const T &w_ = static_cast<T>(1)) : x(xyz.x), y(xyz.y), z(xyz.z), w(w_)
    {}
    nsvec4(const T &x_, const nsvec3<T> &yzw) : x(x_), y(yzw.x), z(yzw.y), w(yzw.z)
    {}
    nsvec4(const nsvec2<T> &xy, const nsvec2<T> &zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y)
    {}
    nsvec4(const nsvec2<T> &xy, const T &z_ = static_cast<T>(0), const T &w_ = static_cast<T>(0)) : x(xy.x), y(xy.y), z(z_), w(w_)
    {}
    nsvec4(const T &x_, const nsvec2<T> &yz, const T &w_ = static_cast<T>(0)) : x(x_), y(yz.x), z(yz.z), w(w_)
    {}
    nsvec4(const T &x_, const T &y_, const nsvec2<T> &zw) : x(x_), y(y_), z(zw.x), w(zw.y)
    {}

    nsvec3<T> &abs()
    {
        x = static_cast<T>(std::abs(x));
        y = static_cast<T>(std::abs(y));
        z = static_cast<T>(std::abs(z));
        w = static_cast<T>(std::abs(w));
        return *this;
    }

    nsvec4<T> &axis_angle_from(const nsvec3<T> &euler_, typename nsvec3<T>::EulerOrder order_, bool rads_ = false)
    {
        return axis_angle_from(nsquat<T>().from(euler_, order_, rads_), rads_);
    }

    nsvec4<T> &axis_angle_from(const nsquat<T> &orientation_, bool rads_ = false)
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
        w = static_cast<T>(2 * std::acos(orientation_.w));

        if (!rads_)
            w = degrees(w);

        T den = static_cast<T>(std::sqrt(1 - orientation_.w * orientation_.w));
        if (den < FLOAT_EPS)
        {
            x = orientation_.x;
            y = orientation_.y;
            z = orientation_.z;
        }
        else
        {
            x = orientation_.x / den;
            y = orientation_.y / den;
            z = orientation_.z / den;
        }
        return *this;
    }

    nsvec4<T> &axis_angle_from(const nsmat3<T> &rotation_mat3_, bool rads_ = false)
    {
        return axis_angle_from(nsquat<T>().from(rotation_mat3_), rads_);
    }

    nsvec4<T> &axis_angle_from(const nsmat4<T> &transform_, bool rads_ = false)
    {
        return axis_angle_from(nsquat<T>().from(transform_), rads_);
    }

    nsvec4<T> &axis_angle_from(const nsvec3<T> &vec_, const nsvec3<T> &to_vec_, bool rads_ = false)
    {
        return axis_angle_from(nsquat<T>().from(vec_, to_vec_), rads_);
    }

    nsvec4<T> &ceil()
    {
        x = static_cast<T>(std::ceil(x));
        y = static_cast<T>(std::ceil(y));
        z = static_cast<T>(std::ceil(z));
        w = static_cast<T>(std::ceil(w));
        return *this;
    }

    nsvec4<T> &clamp(const T &min_ = static_cast<T>(0), const T &max_ = static_cast<T>(0))
    {
        x = std::clamp(x, min_, max_);
        y = std::clamp(x, min_, max_);
        z = std::clamp(x, min_, max_);
        w = std::clamp(x, min_, max_);
        return *this;
    }


    nsvec3<T> vec3() const
    {
        return nsvec3<T>(x,y,z);
    }

    T distance_to(const nsvec4<T> &vec_) const
    {
        return ((vec_ - *this).length());
    }

    nsvec4<T> &floor()
    {
        x = static_cast<T>(std::floor(x));
        y = static_cast<T>(std::floor(y));
        z = static_cast<T>(std::floor(z));
        w = static_cast<T>(std::floor(w));
        return *this;
    }

    nsvec4<T> &fract()
    {
        for (u32 i = 0; i < 4; ++i)
        {
            if (data[i] >= 0)
                data[i] -= static_cast<T>(std::floor(data[i]));
            else
                data[i] -= static_cast<T>(std::ceil(data[i]));
        }
        return *this;
    }

    T length() const
    {
        return static_cast<T>(sqrt(x * x + y * y + z * z + w * w));
    }

    T length_sq() const
    {
        return x * x + y * y + z * z + w * w;
    }

    template<class T2>
    nsvec4<T> &lerp(const nsvec4<T> &vec_, const T2 &scaling_factor_)
    {
        x += static_cast<T>((vec_.x - x) * scaling_factor_);
        y += static_cast<T>((vec_.y - y) * scaling_factor_);
        z += static_cast<T>((vec_.z - z) * scaling_factor_);
        w += static_cast<T>((vec_.w - w) * scaling_factor_);
        return *this;
    }

    T min()
    {
        return std::minmax({x,y,z,w}).first;
        // T m1 = xyz().min();
        // if (m1 < w)
        //     return m1;
        // return w;
    }

    nsvec4<T> &minimize(const nsvec4<T> &rhs_)
    {
        if (x > rhs_.x)
            x = rhs_.x;
        if (y > rhs_.y)
            y = rhs_.y;
        if (z > rhs_.z)
            z = rhs_.z;
        if (w > rhs_.w)
            w = rhs_.w;
        return *this;
    }

    T max()
    {
        return std::minmax({x,y,z,w}).second;
        // T m1 = xyz().max();
        // if (m1 > w)
        //     return m1;
        // return w;
    }

    nsvec4<T> &maximize(const nsvec4<T> &rhs_)
    {
        if (x < rhs_.x)
            x = rhs_.x;
        if (y < rhs_.y)
            y = rhs_.y;
        if (z < rhs_.z)
            z = rhs_.z;
        if (w < rhs_.w)
            w = rhs_.w;
        return *this;
    }

    nsvec4<T> &normalize()
    {
        T l = length();
        if (l == 0)
            return *this;
        return *this *= (1 / l);
    }

    nsvec4<T> &round()
    {
        x = static_cast<T>(std::round(x));
        y = static_cast<T>(std::round(y));
        z = static_cast<T>(std::round(z));
        w = static_cast<T>(std::round(w));
        return *this;
    }

    nsvec4<T> &round_to_zero()
    {
        if (std::abs(x) < EPS_VEC_0)
            x = 0;
        if (std::abs(y) < EPS_VEC_0)
            y = 0;
        if (std::abs(z) < EPS_VEC_0)
            z = 0;
        if (std::abs(w) < EPS_VEC_0)
            w = 0;
        return *this;
    }

    nsvec3<T> &scaling_from(const nsmat3<T> &transform_)
    {
        x = transform_[0].length();
        y = transform_[1].length();
        z = transform_[2].length();
        w = static_cast<T>(1);
        return *this;
    }

    nsvec3<T> &scaling_from(const nsmat4<T> &transform_)
    {
        x = sqrt(transform_[0][0] * transform_[0][0] + transform_[0][1] * transform_[0][1] + transform_[0][2] * transform_[0][2]);
        y = sqrt(transform_[1][0] * transform_[1][0] + transform_[1][1] * transform_[1][1] + transform_[1][2] * transform_[1][2]);
        z = sqrt(transform_[2][0] * transform_[2][0] + transform_[2][1] * transform_[2][1] + transform_[2][2] * transform_[2][2]);
        w = static_cast<T>(1);
        return *this;
    }

    nsvec4<T> &set(const T &val_)
    {
        x = y = z = w = val_;
        return *this;
    }

    nsvec4<T> &set(const T &x_, const T &y_, const T &z_, const T &w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
        return *this;
    }

    nsvec4<T> &set(const nsvec3<T> &xyz, const T &w_)
    {
        x = xyz.x;
        y = xyz.y;
        z = xyz.z;
        w = w_;
        return *this;
    }

    nsvec4<T> &set(const T &x_, const nsvec3<T> &yzw)
    {
        x = x_;
        y = yzw.x;
        z = yzw.y;
        w = yzw.z;
        return *this;
    }

    nsvec4<T> &set(const nsvec2<T> &xy, const T &z_, const T &w_)
    {
        x = xy.x;
        y = xy.y;
        z = z_;
        w = w_;
        return *this;
    }

    nsvec4<T> &set(const T &x_, const nsvec2<T> &yz, const T &w_)
    {
        x = x_;
        y = yz.x;
        z = yz.y;
        w = w_;
        return *this;
    }

    nsvec4<T> &set(const T &x_, const T &y_, const nsvec2<T> &zw)
    {
        x = x_;
        y = y_;
        z = zw.x;
        w = zw.y;
        return *this;
    }

    nsvec4<T> &set_length(const T &len_)
    {
        T l = length();

        if (l == static_cast<T>(0))
            return *this;

        T mult = len_ / l;
        (*this) *= mult;
        return *this;
    }

    // string to_string()
    // {
    //     nsstringstream ss;
    //     ss << "[" << x << " " << y << " " << z << " " << w << "]";
    //     return ss.str();
    // }

    nsvec4<T> &translation_from(const nsmat4<T> &transform_)
    {
        *this = transform_(3);
        w = static_cast<T>(1);
        return *this;
    }

    // overloaded operators
    nsvec4<T> operator+(const nsvec4<T> &rhs_) const
    {
        return nsvec4<T>(x + rhs_.x, y + rhs_.y, z + rhs_.z, w + rhs_.w);
    }

    nsvec4<T> operator-(const nsvec4<T> &rhs_) const
    {
        return nsvec4<T>(x - rhs_.x, y - rhs_.y, z - rhs_.z, w - rhs_.w);
    }

    T operator*(const nsvec4<T> &rhs_) const // dot product
    {
        return x * rhs_.x + y * rhs_.y + z * rhs_.z + w * rhs_.w;
    }

    nsmat4<T> operator^(const nsvec4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret[0] = x * rhs_;
        ret[1] = y * rhs_;
        ret[2] = z * rhs_;
        ret[3] = w * rhs_;
        return ret;
    }

    nsvec4<T> operator%(const nsvec4<T> &rhs_) const // component wise scalar product
    {
        return nsvec4<T>(x * rhs_.x, y * rhs_.y, z * rhs_.z, w * rhs_.w);
    }

    nsvec4<T> operator/(const nsvec4<T> &rhs_) const
    {
        return nsvec4<T>(x / rhs_.x, y / rhs_.y, z / rhs_.z, w / rhs_.w);
    }

    nsvec4<T> operator*(const T &rhs_) const
    {
        return nsvec4<T>(x * rhs_, y * rhs_, z * rhs_, w * rhs_);
    }

    nsvec4<T> operator/(const T &rhs_) const
    {
        return nsvec4<T>(x / rhs_, y / rhs_, z / rhs_, w / rhs_);
    }

    nsvec4<T> &operator=(const nsvec4<T> &rhs_)
    {
        if (this == &rhs_)
            return *this;
        x = rhs_.x;
        y = rhs_.y;
        z = rhs_.z;
        w = rhs_.w;
        return *this;
    }

    nsvec4<T> operator++(i32)
    {
        nsvec4<T> ret(*this);
        ++(*this);
        return ret;
    }

    nsvec4<T> operator--(i32)
    {
        nsvec4<T> ret(*this);
        --(*this);
        return ret;
    }

    nsvec4<T> &operator++()
    {
        ++x;
        ++y;
        ++z;
        ++w;
        return *this;
    }

    nsvec4<T> &operator--()
    {
        --x;
        --y;
        --z;
        --w;
        return *this;
    }

    nsvec4<T> &operator+=(const nsvec4<T> &rhs_)
    {
        x += rhs_.x;
        y += rhs_.y;
        z += rhs_.z;
        w += rhs_.w;
        return *this;
    }

    nsvec4<T> &operator-=(const nsvec4<T> &rhs_)
    {
        x -= rhs_.x;
        y -= rhs_.y;
        z -= rhs_.z;
        w -= rhs_.w;
        return *this;
    }

    nsvec4<T> &operator%=(const nsvec4<T> &rhs_)
    {
        x *= rhs_.x;
        y *= rhs_.y;
        z *= rhs_.z;
        w *= rhs_.w;
        return *this;
    }

    nsvec4<T> &operator/=(const nsvec4<T> &rhs_)
    {
        x /= rhs_.x;
        y /= rhs_.y;
        z /= rhs_.z;
        w /= rhs_.w;
        return *this;
    }

    nsvec4<T> &operator*=(const T &rhs_)
    {
        x *= rhs_;
        y *= rhs_;
        z *= rhs_;
        w *= rhs_;
        return *this;
    }

    nsvec4<T> &operator/=(const T &rhs_)
    {
        x /= rhs_;
        y /= rhs_;
        z /= rhs_;
        w /= rhs_;
        return *this;
    }

    bool operator==(const nsvec4<T> &rhs_) const
    {
        return ((x == rhs_.x) && (y == rhs_.y) && (z == rhs_.z) && (w == rhs_.w));
    }

    bool operator!=(const nsvec4<T> &rhs_) const
    {
        return !(*this == rhs_);
    }

    bool operator==(const T &rhs_) const
    {
        return ((x == rhs_) && (y == rhs_) && (z == rhs_) && (w == rhs_));
    }

    bool operator!=(const T &rhs_) const
    {
        return !(*this == rhs_);
    }

    const T &operator[](sizet val_) const
    {
        assert(val_ < 4 && "Index out of range!");
        return data[val_];
    }

    T &operator[](sizet val_)
    {
        assert(val_ < 4 && "Index out of range!");
        return data[val_];
    }

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
    };
};

template<class T>
nsvec4<T> operator*(const i32 &lhs_, const nsvec4<T> &rhs_)
{
    return rhs_ * static_cast<T>(lhs_);
}

template<class T>
nsvec4<T> operator*(const float &lhs_, const nsvec4<T> &rhs_)
{
    return rhs_ * static_cast<T>(lhs_);
}

template<class T>
nsvec4<T> operator*(const double &lhs_, const nsvec4<T> &rhs_)
{
    return rhs_ * static_cast<T>(lhs_);
}

template<class T>
nsvec4<T> operator/(const i32 &lhs_, const nsvec4<T> &rhs_)
{
    return nsvec4<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z, lhs_ / rhs_.w);
}

template<class T>
nsvec4<T> operator/(const float &lhs_, const nsvec4<T> &rhs_)
{
    return nsvec4<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z, lhs_ / rhs_.w);
}

template<class T>
nsvec4<T> operator/(const double &lhs_, const nsvec4<T> &rhs_)
{
    return nsvec4<T>(lhs_ / rhs_.x, lhs_ / rhs_.y, lhs_ / rhs_.z, lhs_ / rhs_.w);
}

template<class T>
nsvec4<T> abs(const nsvec4<T> &vec_)
{
    return nsvec4<T>(vec_).abs();
}

template<class T>
nsvec4<T> axis_angle(const nsvec3<T> &euler_, typename nsvec3<T>::EulerOrder order_, bool rads_)
{
    return nsvec4<T>().axis_angle_from(nsquat<T>().from(euler_, order_, rads_), rads_);
}

template<class T>
nsvec4<T> axis_angle(const nsquat<T> &orientation_, bool rads_)
{
    return nsvec4<T>().axis_angle_from(orientation_, rads_);
}

template<class T>
nsvec4<T> axis_angle(const nsmat3<T> &rotation_mat3_, bool rads_)
{
    return nsvec4<T>().axis_angle_from(nsquat<T>().from(rotation_mat3_), rads_);
}

template<class T>
nsvec4<T> axis_angle(const nsmat4<T> &transform_, bool rads_)
{
    return nsvec4<T>().axis_angle_from(nsquat<T>().from(transform_), rads_);
}

template<class T>
nsvec4<T> axis_angle(const nsvec3<T> &vec_, const nsvec3<T> &to_vec_, bool rads_)
{
    return nsvec4<T>().axis_angle_from(nsquat<T>().from(vec_, to_vec_), rads_);
}

template<class T>
nsvec4<T> ceil(const nsvec4<T> &vec_)
{
    nsvec4<T> ret(vec_);
    ret.ceiling();
    return ret;
}

template<class T>
nsvec4<T> clamp(const nsvec4<T> &vec_, const T &min_, const T &max_)
{
    nsvec4<T> ret(vec_);
    ret.clamp(min_, max_);
    return ret;
}

template<class T>
T distance(const nsvec4<T> &lvec_, const nsvec4<T> &rvec_)
{
    return lvec_.distanceTo(rvec_);
}

template<class T>
T dot(const nsvec4<T> &lhs_, const nsvec4<T> &rhs_)
{
    return lhs_ * rhs_;
}

template<class T>
nsvec4<T> floor(const nsvec4<T> &vec_)
{
    nsvec4<T> ret(vec_);
    ret.floor();
    return ret;
}

template<class T>
nsvec4<T> fract(const nsvec4<T> &vec_)
{
    return nsvec4<T>(vec_).fract();
}

template<class T>
T length(const nsvec4<T> &vec_)
{
    return vec_.length();
}

template<class T, class T2>
nsvec4<T> lerp(const nsvec4<T> &lhs_, const nsvec4<T> &rhs_, T2 scaling_factor_)
{
    nsvec4<T> ret(lhs_);
    ret.lerp(rhs_, scaling_factor_);
    return ret;
}

template<class T>
nsvec4<T> min(const nsvec4<T> &lhs_, const nsvec4<T> &rhs_)
{
    nsvec4<T> ret(lhs_);
    ret.minimize(rhs_);
    return ret;
}

template<class T>
nsvec4<T> max(const nsvec4<T> &lhs_, const nsvec4<T> &rhs_)
{
    nsvec4<T> ret(lhs_);
    ret.maximize(rhs_);
    return ret;
}

template<class T>
nsvec4<T> normalize(const nsvec4<T> &rhs_)
{
    nsvec4<T> ret(rhs_);
    ret.normalize();
    return ret;
}

template<class T>
nsvec4<T> round(const nsvec4<T> &vec_)
{
    nsvec4<T> ret(vec_);
    ret.round();
    return ret;
}

using cvec4 = nsvec4<char>;
using c16vec4 = nsvec4<char16>;
using c32vec4 = nsvec4<char32>;
using wcvec4 = nsvec4<wchar>;
using i8vec4 = nsvec4<i8>;
using i16vec4 = nsvec4<i16>;
using ivec4 = nsvec4<i32>;
using i64vec4 = nsvec4<i64>;
using u8vec4 = nsvec4<u8>;
using u16vec4 = nsvec4<u16>;
using uvec4 = nsvec4<u32>;
using u64vec4 = nsvec4<u64>;
using vec4 = nsvec4<float>;
using dvec4 = nsvec4<double>;
using ldvec4 = nsvec4<ldouble>;