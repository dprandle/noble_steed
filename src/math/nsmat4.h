#pragma once

#include "nsmat3.h"

namespace noble_steed
{
template<class T>
struct nsmat4
{
    //
    nsmat4() : data{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    {}

    //
    nsmat4(const nsmat4 &copy_) = default;

    nsmat4(const T &val_) : data{{val_}, {val_}, {val_}, {val_}}
    {}

    //
    nsmat4(const T &a,
           const T &b,
           const T &c,
           const T &d,
           const T &e,
           const T &f,
           const T &g,
           const T &h,
           const T &i,
           const T &j,
           const T &k,
           const T &l,
           const T &m,
           const T &n,
           const T &o,
           const T &p)
        : data{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {m, n, o, p}}
    {}

    //
    nsmat4(const vector4<T> &row1, const vector4<T> &row2, const vector4<T> &row3, const vector4<T> &row4)
    {
        data[0] = row1;
        data[1] = row2;
        data[2] = row3;
        data[3] = row4;
    }

    //
    nsmat4(const nsmat3<T> &basis_)
    {
        data[0][0] = basis_[0][0];
        data[0][1] = basis_[0][1];
        data[0][2] = basis_[0][2];
        data[1][0] = basis_[1][0];
        data[1][1] = basis_[1][1];
        data[1][2] = basis_[1][2];
        data[2][0] = basis_[2][0];
        data[2][1] = basis_[2][1];
        data[2][2] = basis_[2][2];
        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;
    }

    //
    nsmat3<T> basis() const
    {
        return nsmat3<T>(data[0][0], data[0][1], data[0][2], data[1][0], data[1][1], data[1][2], data[2][0], data[2][1], data[2][2]);
    }

    T *data_ptr()
    {
        return &data[0][0];
    }

    nsmat4<T> &abs()
    {
        data[0].abs();
        data[1].abs();
        data[2].abs();
        data[3].abs();
        return *this;
    }

    vector2<T> minmax() const
    {
        auto pr = std::minmax({data[0][0],
                               data[0][1],
                               data[0][2],
                               data[0][3],
                               data[1][0],
                               data[1][1],
                               data[1][2],
                               data[1][3],
                               data[2][0],
                               data[2][1],
                               data[2][2],
                               data[2][3],
                               data[3][0],
                               data[3][1],
                               data[3][2],
                               data[3][3]});
        return {pr.first, pr.second};
    }

    T determinant() const
    {
        return data[0][3] * data[1][2] * data[2][1] * data[3][0] - data[0][2] * data[1][3] * data[2][1] * data[3][0] -
               data[0][3] * data[1][1] * data[2][2] * data[3][0] + data[0][1] * data[1][3] * data[2][2] * data[3][0] +
               data[0][2] * data[1][1] * data[2][3] * data[3][0] - data[0][1] * data[1][2] * data[2][3] * data[3][0] -
               data[0][3] * data[1][2] * data[2][0] * data[3][1] + data[0][2] * data[1][3] * data[2][0] * data[3][1] +
               data[0][3] * data[1][0] * data[2][2] * data[3][1] - data[0][0] * data[1][3] * data[2][2] * data[3][1] -
               data[0][2] * data[1][0] * data[2][3] * data[3][1] + data[0][0] * data[1][2] * data[2][3] * data[3][1] +
               data[0][3] * data[1][1] * data[2][0] * data[3][2] - data[0][1] * data[1][3] * data[2][0] * data[3][2] -
               data[0][3] * data[1][0] * data[2][1] * data[3][2] + data[0][0] * data[1][3] * data[2][1] * data[3][2] +
               data[0][1] * data[1][0] * data[2][3] * data[3][2] - data[0][0] * data[1][1] * data[2][3] * data[3][2] -
               data[0][2] * data[1][1] * data[2][0] * data[3][3] + data[0][1] * data[1][2] * data[2][0] * data[3][3] +
               data[0][2] * data[1][0] * data[2][1] * data[3][3] - data[0][0] * data[1][2] * data[2][1] * data[3][3] -
               data[0][1] * data[1][0] * data[2][2] * data[3][3] + data[0][0] * data[1][1] * data[2][2] * data[3][3];
    }

    nsmat4<T> &invert()
    {
        T det = determinant();

        if (det == 0)
            return nsmat4<T>();

        nsmat4 ret;
        ret[0][0] = data[1][2] * data[2][3] * data[3][1] - data[1][3] * data[2][2] * data[3][1] + data[1][3] * data[2][1] * data[3][2] -
                    data[1][1] * data[2][3] * data[3][2] - data[1][2] * data[2][1] * data[3][3] + data[1][1] * data[2][2] * data[3][3];
        ret[0][1] = data[0][3] * data[2][2] * data[3][1] - data[0][2] * data[2][3] * data[3][1] - data[0][3] * data[2][1] * data[3][2] +
                    data[0][1] * data[2][3] * data[3][2] + data[0][2] * data[2][1] * data[3][3] - data[0][1] * data[2][2] * data[3][3];
        ret[0][2] = data[0][2] * data[1][3] * data[3][1] - data[0][3] * data[1][2] * data[3][1] + data[0][3] * data[1][1] * data[3][2] -
                    data[0][1] * data[1][3] * data[3][2] - data[0][2] * data[1][1] * data[3][3] + data[0][1] * data[1][2] * data[3][3];
        ret[0][3] = data[0][3] * data[1][2] * data[2][1] - data[0][2] * data[1][3] * data[2][1] - data[0][3] * data[1][1] * data[2][2] +
                    data[0][1] * data[1][3] * data[2][2] + data[0][2] * data[1][1] * data[2][3] - data[0][1] * data[1][2] * data[2][3];
        ret[1][0] = data[1][3] * data[2][2] * data[3][0] - data[1][2] * data[2][3] * data[3][0] - data[1][3] * data[2][0] * data[3][2] +
                    data[1][0] * data[2][3] * data[3][2] + data[1][2] * data[2][0] * data[3][3] - data[1][0] * data[2][2] * data[3][3];
        ret[1][1] = data[0][2] * data[2][3] * data[3][0] - data[0][3] * data[2][2] * data[3][0] + data[0][3] * data[2][0] * data[3][2] -
                    data[0][0] * data[2][3] * data[3][2] - data[0][2] * data[2][0] * data[3][3] + data[0][0] * data[2][2] * data[3][3];
        ret[1][2] = data[0][3] * data[1][2] * data[3][0] - data[0][2] * data[1][3] * data[3][0] - data[0][3] * data[1][0] * data[3][2] +
                    data[0][0] * data[1][3] * data[3][2] + data[0][2] * data[1][0] * data[3][3] - data[0][0] * data[1][2] * data[3][3];
        ret[1][3] = data[0][2] * data[1][3] * data[2][0] - data[0][3] * data[1][2] * data[2][0] + data[0][3] * data[1][0] * data[2][2] -
                    data[0][0] * data[1][3] * data[2][2] - data[0][2] * data[1][0] * data[2][3] + data[0][0] * data[1][2] * data[2][3];
        ret[2][0] = data[1][1] * data[2][3] * data[3][0] - data[1][3] * data[2][1] * data[3][0] + data[1][3] * data[2][0] * data[3][1] -
                    data[1][0] * data[2][3] * data[3][1] - data[1][1] * data[2][0] * data[3][3] + data[1][0] * data[2][1] * data[3][3];
        ret[2][1] = data[0][3] * data[2][1] * data[3][0] - data[0][1] * data[2][3] * data[3][0] - data[0][3] * data[2][0] * data[3][1] +
                    data[0][0] * data[2][3] * data[3][1] + data[0][1] * data[2][0] * data[3][3] - data[0][0] * data[2][1] * data[3][3];
        ret[2][2] = data[0][1] * data[1][3] * data[3][0] - data[0][3] * data[1][1] * data[3][0] + data[0][3] * data[1][0] * data[3][1] -
                    data[0][0] * data[1][3] * data[3][1] - data[0][1] * data[1][0] * data[3][3] + data[0][0] * data[1][1] * data[3][3];
        ret[2][3] = data[0][3] * data[1][1] * data[2][0] - data[0][1] * data[1][3] * data[2][0] - data[0][3] * data[1][0] * data[2][1] +
                    data[0][0] * data[1][3] * data[2][1] + data[0][1] * data[1][0] * data[2][3] - data[0][0] * data[1][1] * data[2][3];
        ret[3][0] = data[1][2] * data[2][1] * data[3][0] - data[1][1] * data[2][2] * data[3][0] - data[1][2] * data[2][0] * data[3][1] +
                    data[1][0] * data[2][2] * data[3][1] + data[1][1] * data[2][0] * data[3][2] - data[1][0] * data[2][1] * data[3][2];
        ret[3][1] = data[0][1] * data[2][2] * data[3][0] - data[0][2] * data[2][1] * data[3][0] + data[0][2] * data[2][0] * data[3][1] -
                    data[0][0] * data[2][2] * data[3][1] - data[0][1] * data[2][0] * data[3][2] + data[0][0] * data[2][1] * data[3][2];
        ret[3][2] = data[0][2] * data[1][1] * data[3][0] - data[0][1] * data[1][2] * data[3][0] - data[0][2] * data[1][0] * data[3][1] +
                    data[0][0] * data[1][2] * data[3][1] + data[0][1] * data[1][0] * data[3][2] - data[0][0] * data[1][1] * data[3][2];
        ret[3][3] = data[0][1] * data[1][2] * data[2][0] - data[0][2] * data[1][1] * data[2][0] + data[0][2] * data[1][0] * data[2][1] -
                    data[0][0] * data[1][2] * data[2][1] - data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];

        ret /= determinant();
        return (*this = ret);
    }

    nsmat4<T> &ortho_from(const T &left_, const T &right_, const T &top_, const T &bottom_, const T &near_, const T &far_)
    {
        T w = right_ - left_;
        T h = top_ - bottom_;
        T p = far_ - near_;

        T x = (right_ + left_) / w;
        T y = (top_ + bottom_) / h;
        T z = (far_ + near_) / p;

        *this = nsmat4<T>();
        set_column(3, -x, -y, -z, 1);
        data[0][0] = 2 / w;
        data[1][1] = 2 / h;
        data[2][2] = -2 / p;
        return *this;
    }

    nsmat4<T> &perspective_from(const T &fov_, const T &aspect_ratio_, const T &z_near_, const T &z_far_)
    {
        T zRange = z_near_ - z_far_;
        T usefulNum = math::tan(radians(fov_ / 2));
        *this = nsmat4<T>();
        data[0][0] = 1 / (aspect_ratio_ * usefulNum);
        data[1][1] = 1 / usefulNum;
        data[2][2] = (-z_near_ - z_far_) / zRange;
        data[2][3] = 2 * z_far_ * z_near_ / zRange;
        data[3][2] = 1;
        data[3][3] = 0;

        return *this;
    }

    vector3<T> right() const
    {
        return normalize(vector3<T>(data[0].x, data[1].x, data[2].x));
    }

    nsmat4<T> &rotation_from(const vector4<T> &axis_angle_, bool rads_ = false)
    {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm

        T angle = axis_angle_.a;

        if (!rads_)
            angle = radians(angle);

        T c = math::cos(angle);
        T s = math::sin(angle);
        T t = 1 - c;

        data[0][0] = c + axis_angle_.x * axis_angle_.x * t;
        data[1][1] = c + axis_angle_.y * axis_angle_.y * t;
        data[2][2] = c + axis_angle_.z * axis_angle_.z * t;

        T tmp1 = axis_angle_.x * axis_angle_.y * t;
        T tmp2 = axis_angle_.z * s;
        data[1][0] = tmp1 + tmp2;
        data[0][1] = tmp1 - tmp2;

        tmp1 = axis_angle_.x * axis_angle_.z * t;
        tmp2 = axis_angle_.y * s;
        data[2][0] = tmp1 - tmp2;
        data[0][2] = tmp1 + tmp2;

        tmp1 = axis_angle_.y * axis_angle_.z * t;
        tmp2 = axis_angle_.x * s;
        data[2][1] = tmp1 + tmp2;
        data[1][2] = tmp1 - tmp2;

        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;

        return *this;
    }

    nsmat4<T> &rotation_from(const vector3<T> &euler_, typename vector3<T>::euler_order order_, bool rads_ = false)
    {
        T cb, sb, ch, sh, ca, sa;
        if (!rads_)
        {
            vector3<T> eulRads(radians(euler_));
            cb = math::cos(eulRads.x);
            sb = math::sin(eulRads.x);
            ch = math::cos(eulRads.y);
            sh = math::sin(eulRads.y);
            ca = math::cos(eulRads.z);
            sa = math::sin(eulRads.z);
        }
        else
        {
            cb = math::cos(euler_.x);
            sb = math::sin(euler_.x);
            ch = math::cos(euler_.y);
            sh = math::sin(euler_.y);
            ca = math::cos(euler_.z);
            sa = math::sin(euler_.z);
        }

        switch (order_)
        {
        case (vector3<T>::XYZ): {
            data[0][0] = ch * ca;
            data[0][1] = -ch * sa;
            data[0][2] = sh;

            data[1][0] = cb * sa + sb * ca * sh;
            data[1][1] = cb * ca - sb * sa * sh;
            data[1][2] = -sb * ch;

            data[2][0] = sb * sa - cb * ca * sh;
            data[2][1] = sb * ca + cb * sa * sh;
            data[2][2] = cb * ch;
            break;
        }
        case (vector3<T>::XZY): {
            data[0][0] = ch * ca;
            data[0][1] = -sa;
            data[0][2] = sh * ca;

            data[1][0] = cb * ch * sa + sb * sh;
            data[1][1] = cb * ca;
            data[1][2] = cb * sh * sa - sb * ch;

            data[2][0] = sb * ch * sa - cb * sh;
            data[2][1] = sb * ca;
            data[2][2] = sb * sh * sa + cb * ch;
            break;
        }
        case (vector3<T>::YXZ): {
            data[0][0] = ch * ca + sh * sa * sb;
            data[0][1] = sh * ca * sb - ch * sa;
            data[0][2] = cb * sh;

            data[1][0] = cb * sa;
            data[1][1] = cb * ca;
            data[1][2] = -sb;

            data[2][0] = ch * sa * sb - sh * ca;
            data[2][1] = sh * sa + ch * ca * sb;
            data[2][2] = cb * ch;
            break;
        }
        case (vector3<T>::YZX): {
            data[0][0] = ch * ca;
            data[0][1] = sb * sh - cb * ch * sa;
            data[0][2] = sb * ch * sa + cb * sh;

            data[1][0] = sa;
            data[1][1] = cb * ca;
            data[1][2] = -sb * ca;

            data[2][0] = -sh * ca;
            data[2][1] = cb * sh * sa + sb * ch;
            data[2][2] = cb * ch - sb * sh * sa;
            break;
        }
        case (vector3<T>::ZXY): {
            data[0][0] = ch * ca - sh * sa * sb;
            data[0][1] = -cb * sa;
            data[0][2] = sh * ca + ch * sa * sb;

            data[1][0] = ch * sa + sh * ca * sb;
            data[1][1] = cb * ca;
            data[1][2] = sh * sa - ch * ca * sb;

            data[2][0] = -cb * sh;
            data[2][1] = sb;
            data[2][2] = cb * ch;
            break;
        }
        case (vector3<T>::ZYX): {
            data[0][0] = ch * ca;
            data[0][1] = sb * ca * sh - cb * sa;
            data[0][2] = cb * ca * sh + sb * sa;

            data[1][0] = ch * sa;
            data[1][1] = sb * sa * sh + cb * ca;
            data[1][2] = cb * sa * sh - sb * ca;

            data[2][0] = -sh;
            data[2][1] = sb * ch;
            data[2][2] = cb * ch;
            break;
        }
        }

        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;
        return round_to_zero();
    }

    nsmat4<T> &rotation_from(const quaternion<T> &ornt_)
    {
        data[0][0] = 1 - 2 * (ornt_.y * ornt_.y + ornt_.z * ornt_.z);
        data[0][1] = 2 * (ornt_.x * ornt_.y - ornt_.z * ornt_.w);
        data[0][2] = 2 * (ornt_.x * ornt_.z + ornt_.y * ornt_.w);

        data[1][0] = 2 * (ornt_.x * ornt_.y + ornt_.z * ornt_.w);
        data[1][1] = 1 - 2 * (ornt_.x * ornt_.x + ornt_.z * ornt_.z);
        data[1][2] = 2 * (ornt_.y * ornt_.z - ornt_.x * ornt_.w);

        data[2][0] = 2 * (ornt_.x * ornt_.z - ornt_.y * ornt_.w);
        data[2][1] = 2 * (ornt_.y * ornt_.z + ornt_.x * ornt_.w);
        data[2][2] = 1 - 2 * (ornt_.x * ornt_.x + ornt_.y * ornt_.y);

        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;
        return *this;
    }

    nsmat4<T> &rotation_from(const nsmat3<T> &transform_)
    {
        set(transform_);
        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;
        data[0].normalize();
        data[1].normalize();
        data[2].normalize();
        return *this;
    }

    nsmat4<T> &rotation_from(const nsmat4<T> &transform_)
    {
        *this = transform_;

        // Set remaining components to identity
        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;

        data[0].normalize();
        data[1].normalize();
        data[2].normalize();
        return *this;
    }

    nsmat4<T> &rotation_from(const vector3<T> &vec_, const vector3<T> &to_vec_)
    {
        /* http://www.euclideanspace.com/maths/algebra/vectors/angleBetween/index.htm */
        vector3<T> vs = cross(vec_, to_vec_);
        T ca = vec_ * to_vec_;
        vector3<T> v = normalize(vs);
        vector3<T> vt = v * (1 - ca);

        data[0][0] = vt.x * v.x + ca;
        data[1][1] = vt.y * v.y + ca;
        data[2][2] = vt.z * v.z + ca;
        vt.x *= v.y;
        vt.z *= v.x;
        vt.y *= v.z;
        data[0][1] = vt.x - vs.z;
        data[0][2] = vt.z + vs.y;
        data[1][0] = vt.x + vs.z;
        data[1][2] = vt.y - vs.x;
        data[2][0] = vt.z - vs.y;
        data[2][1] = vt.y + vs.x;

        // Set remaining components
        data[0][3] = 0;
        data[1][3] = 0;
        data[2][3] = 0;
        data[3][0] = 0;
        data[3][1] = 0;
        data[3][2] = 0;
        data[3][3] = 1;
        return *this;
    }

    nsmat4<T> &round_to_zero()
    {
        data[0].round_to_zero();
        data[1].round_to_zero();
        data[2].round_to_zero();
        data[3].round_to_zero();
        return *this;
    }

    nsmat4<T> &scaling_from(const vector3<T> &scale_)
    {
        *this = nsmat4<T>();
        data[0][0] = scale_.x;
        data[1][1] = scale_.y;
        data[2][2] = scale_.z;
        return *this;
    }

    nsmat4<T> &scaling_from(const nsmat3<T> &transform_)
    {
        vector3<T> scalingVec(transform_[0].length(), transform_[1].length(), transform_[2].length());
        return scaling_from(scalingVec);
    }

    nsmat4<T> &scaling_from(const nsmat4<T> &transform_)
    {
        vector3<T> scalingVec;
        scalingVec.x = math::sqrt(transform_[0][0] * transform_[0][0] + transform_[0][1] * transform_[0][1] + transform_[0][2] * transform_[0][2]);
        scalingVec.y = math::sqrt(transform_[1][0] * transform_[1][0] + transform_[1][1] * transform_[1][1] + transform_[1][2] * transform_[1][2]);
        scalingVec.z = math::sqrt(transform_[2][0] * transform_[2][0] + transform_[2][1] * transform_[2][1] + transform_[2][2] * transform_[2][2]);
        return scaling_from(scalingVec);
    }

    nsmat4<T> &set_column(const u32 &i, const T &x, const T &y, const T &z, const T &w)
    {
        (*this)[0][i] = x;
        (*this)[1][i] = y;
        (*this)[2][i] = z;
        (*this)[3][i] = w;
        return *this;
    }

    nsmat4<T> &set_column(const u32 &i, const vector4<T> &col)
    {
        (*this)[0][i] = col.x;
        (*this)[1][i] = col.y;
        (*this)[2][i] = col.z;
        (*this)[3][i] = col.w;
        return *this;
    }

    vector3<T> target() const
    {
        return normalize(vector3<T>(data[0].z, data[1].z, data[2].z));
    }

    nsmat4<T> &translation_from(const vector3<T> &pos_)
    {
        return set_column(3, pos_.x, pos_.y, pos_.z, 1);
    }

    nsmat4<T> &translation_from(const vector4<T> &posw_)
    {
        return set_column(3, posw_.x, posw_.y, posw_.z, 1);
    }

    nsmat4<T> &translation_from(const nsmat4<T> &transform_)
    {
        *this = transform_;
        data[3][3] = 1;
        return set(nsmat3<T>());
    }

    nsmat4<T> &transpose()
    {
        T tmp;
        tmp = data[1][0];
        data[1][0] = data[0][1];
        data[0][1] = tmp;
        tmp = data[2][0];
        data[2][0] = data[0][2];
        data[0][2] = tmp;
        tmp = data[2][1];
        data[2][1] = data[1][2];
        data[1][2] = tmp;

        tmp = data[3][0];
        data[3][0] = data[0][3];
        data[0][3] = tmp;
        tmp = data[3][1];
        data[3][1] = data[1][3];
        data[1][3] = tmp;
        tmp = data[3][2];
        data[3][2] = data[2][3];
        data[2][3] = tmp;
        return *this;
    }

    vector3<T> up() const
    {
        return normalize(vector3<T>(data[0].y, data[1].y, data[2].y));
    }

    //
    nsmat4<T> operator*(const nsmat4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret[0][0] = data[0][0] * rhs_[0][0] + data[0][1] * rhs_[1][0] + data[0][2] * rhs_[2][0] + data[0][3] * rhs_[3][0];
        ret[0][1] = data[0][0] * rhs_[0][1] + data[0][1] * rhs_[1][1] + data[0][2] * rhs_[2][1] + data[0][3] * rhs_[3][1];
        ret[0][2] = data[0][0] * rhs_[0][2] + data[0][1] * rhs_[1][2] + data[0][2] * rhs_[2][2] + data[0][3] * rhs_[3][2];
        ret[0][3] = data[0][0] * rhs_[0][3] + data[0][1] * rhs_[1][3] + data[0][2] * rhs_[2][3] + data[0][3] * rhs_[3][3];

        ret[1][0] = data[1][0] * rhs_[0][0] + data[1][1] * rhs_[1][0] + data[1][2] * rhs_[2][0] + data[1][3] * rhs_[3][0];
        ret[1][1] = data[1][0] * rhs_[0][1] + data[1][1] * rhs_[1][1] + data[1][2] * rhs_[2][1] + data[1][3] * rhs_[3][1];
        ret[1][2] = data[1][0] * rhs_[0][2] + data[1][1] * rhs_[1][2] + data[1][2] * rhs_[2][2] + data[1][3] * rhs_[3][2];
        ret[1][3] = data[1][0] * rhs_[0][3] + data[1][1] * rhs_[1][3] + data[1][2] * rhs_[2][3] + data[1][3] * rhs_[3][3];

        ret[2][0] = data[2][0] * rhs_[0][0] + data[2][1] * rhs_[1][0] + data[2][2] * rhs_[2][0] + data[2][3] * rhs_[3][0];
        ret[2][1] = data[2][0] * rhs_[0][1] + data[2][1] * rhs_[1][1] + data[2][2] * rhs_[2][1] + data[2][3] * rhs_[3][1];
        ret[2][2] = data[2][0] * rhs_[0][2] + data[2][1] * rhs_[1][2] + data[2][2] * rhs_[2][2] + data[2][3] * rhs_[3][2];
        ret[2][3] = data[2][0] * rhs_[0][3] + data[2][1] * rhs_[1][3] + data[2][2] * rhs_[2][3] + data[2][3] * rhs_[3][3];

        ret[3][0] = data[3][0] * rhs_[0][0] + data[3][1] * rhs_[1][0] + data[3][2] * rhs_[2][0] + data[3][3] * rhs_[3][0];
        ret[3][1] = data[3][0] * rhs_[0][1] + data[3][1] * rhs_[1][1] + data[3][2] * rhs_[2][1] + data[3][3] * rhs_[3][1];
        ret[3][2] = data[3][0] * rhs_[0][2] + data[3][1] * rhs_[1][2] + data[3][2] * rhs_[2][2] + data[3][3] * rhs_[3][2];
        ret[3][3] = data[3][0] * rhs_[0][3] + data[3][1] * rhs_[1][3] + data[3][2] * rhs_[2][3] + data[3][3] * rhs_[3][3];
        return ret;
    }

    nsmat4<T> operator/(const nsmat4<T> &rhs_) const
    {
        return *this * inverse(rhs_);
    }

    nsmat4<T> operator%(const nsmat4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] % rhs_.data[0];
        ret.data[1] = data[1] % rhs_.data[1];
        ret.data[2] = data[2] % rhs_.data[2];
        ret.data[3] = data[3] % rhs_.data[3];
        return ret;
    }

    vector4<T> operator*(const vector4<T> &rhs_) const
    {
        return vector4<T>(data[0] * rhs_, data[1] * rhs_, data[2] * rhs_, data[3] * rhs_);
    }

    nsmat4<T> operator%(const vector4<T> &rhs_) const
    {
        return nsmat4<T>(data[0] % rhs_, data[1] % rhs_, data[2] % rhs_, data[3] % rhs_);
    }

    nsmat4<T> operator/(const vector4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] / rhs_[0];
        ret.data[1] = data[1] / rhs_[1];
        ret.data[2] = data[2] / rhs_[2];
        ret.data[3] = data[3] / rhs_[3];
        return ret;
    }

    nsmat4<T> operator*(const T &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] * rhs_;
        ret.data[1] = data[1] * rhs_;
        ret.data[2] = data[2] * rhs_;
        ret.data[3] = data[3] * rhs_;
        return ret;
    }

    nsmat4<T> operator/(const T &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] / rhs_;
        ret.data[1] = data[1] / rhs_;
        ret.data[2] = data[2] / rhs_;
        ret.data[3] = data[3] / rhs_;
        return ret;
    }

    nsmat4<T> operator+(const nsmat4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] + rhs_.data[0];
        ret.data[1] = data[1] + rhs_.data[1];
        ret.data[2] = data[2] + rhs_.data[2];
        ret.data[3] = data[3] + rhs_.data[3];
        return ret;
    }

    nsmat4<T> operator-(const nsmat4<T> &rhs_) const
    {
        nsmat4<T> ret;
        ret.data[0] = data[0] - rhs_.data[0];
        ret.data[1] = data[1] - rhs_.data[1];
        ret.data[2] = data[2] - rhs_.data[2];
        ret.data[3] = data[3] - rhs_.data[3];
        return ret;
    }

    bool operator==(const nsmat4<T> &rhs_) const
    {
        return (data[0] == rhs_.data[0] && data[1] == rhs_.data[1] && data[2] == rhs_.data[2] && data[3] == rhs_.data[3]);
    }

    bool operator!=(const nsmat4<T> &rhs_) const
    {
        return !(*this == rhs_);
    }

    nsmat4<T> &operator=(const nsmat4<T> &rhs_)
    {
        if (this == &rhs_)
            return *this;

        data[0] = rhs_.data[0];
        data[1] = rhs_.data[1];
        data[2] = rhs_.data[2];
        data[3] = rhs_.data[3];
        return *this;
    }

    nsmat4<T> &operator*=(const nsmat4<T> &rhs_)
    {
        *this = *this * rhs_;
        return *this;
    }

    nsmat4<T> &operator/=(const nsmat4<T> &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat4<T> &operator%=(const nsmat4<T> &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat4<T> &operator%=(const vector4<T> &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat4<T> &operator/=(const vector4<T> &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat4<T> &operator*=(const T &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat4<T> &operator/=(const T &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat4<T> operator++(i32)
    {
        nsmat4<T> ret(*this);
        ++(*this);
        return ret;
    }

    nsmat4<T> operator--(i32)
    {
        nsmat4<T> ret(*this);
        --(*this);
        return ret;
    }

    nsmat4<T> &operator++()
    {
        ++data[0];
        ++data[1];
        ++data[2];
        ++data[3];
        return *this;
    }

    nsmat4<T> &operator--()
    {
        --data[0];
        --data[1];
        --data[2];
        --data[3];
        return *this;
    }

    nsmat4<T> &operator+=(const nsmat4<T> &rhs_)
    {
        *this = *this + rhs_;
        return *this;
    }

    nsmat4<T> &operator-=(const nsmat4<T> &rhs_)
    {
        *this = *this - rhs_;
        return *this;
    }

    //
    const vector4<T> &operator[](const u32 &pVal) const
    {
        if (pVal > 3)
            throw(std::out_of_range("mat4 index out of range"));
        return data[pVal];
    }

    //
    vector4<T> &operator[](const u32 &pVal)
    {
        if (pVal > 3)
            throw(std::out_of_range("mat4 index out of range"));
        return data[pVal];
    }

    vector4<T> operator()(const u32 &pVal) const
    {
        return vector4<T>(data[0][pVal], data[1][pVal], data[2][pVal], data[3][pVal]);
    }

  private:
    vector4<T> data[4];
};

template<class T>
nsmat4<T> operator*(const i32 &lhs_, const nsmat4<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat4<T> operator*(const float &lhs_, const nsmat4<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat4<T> operator*(const double &lhs_, const nsmat4<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat4<T> operator/(const i32 &lhs_, const nsmat4<T> &rhs_)
{
    return nsmat4<T>(lhs_ / rhs_[0], lhs_ / rhs_[1], lhs_ / rhs_[2]);
}

template<class T>
nsmat4<T> operator/(const float &lhs_, const nsmat4<T> &rhs_)
{
    return nsmat4<T>(lhs_ / rhs_[0], lhs_ / rhs_[1], lhs_ / rhs_[2]);
}

template<class T>
nsmat4<T> operator/(const double &lhs_, const nsmat4<T> &rhs_)
{
    return nsmat4<T>(lhs_ / rhs_[0], lhs_ / rhs_[1], lhs_ / rhs_[2]);
}

template<class T>
vector3<T> operator*(const vector3<T> &lhs_, const nsmat4<T> &rhs_)
{
    return vector3<T>(lhs_[0] * rhs_[0][0] + lhs_[1] * rhs_[1][0] + lhs_[2] * rhs_[2][0] + lhs_[3] * rhs_[3][0],
                     lhs_[0] * rhs_[0][1] + lhs_[1] * rhs_[1][1] + lhs_[2] * rhs_[2][1] + lhs_[3] * rhs_[3][1],
                     lhs_[0] * rhs_[0][2] + lhs_[1] * rhs_[1][2] + lhs_[2] * rhs_[2][2] + lhs_[3] * rhs_[3][2]);
}

template<class T>
vector3<T> operator/(const vector3<T> &lhs_, const nsmat4<T> &rhs_)
{
    return lhs_ * inverse(rhs_);
}

template<class T>
nsmat4<T> operator%(const vector3<T> &lhs_, const nsmat4<T> &rhs_)
{
    return nsmat4<T>(rhs_[0] * lhs_[0], rhs_[1] * lhs_[1], rhs_[2] * lhs_[2], rhs_[3] * lhs_[3]);
}

template<class T>
nsmat4<T> abs(const nsmat4<T> &mat_)
{
    auto cpy{mat_};
    return cpy.abs();
}

template<class T>
T determinant(const nsmat4<T> &mat_)
{
    return mat_.determinant();
}

template<class T>
nsmat4<T> ortho(const T &left_, const T &right_, const T &top_, const T &bottom_, const T &near_, const T &far_)
{
    return nsmat4<T>().ortho_from(left_, right_, top_, bottom_, near_, far_);
}

template<class T>
nsmat4<T> perspective(const T &fov_, const T &aspect_ratio_, const T &z_near_, const T &z_far_)
{
    return nsmat4<T>().perspective_from(fov_, aspect_ratio_, z_near_, z_far_);
}

template<class T>
nsmat4<T> rotation_mat4(const vector4<T> &axis_angle_, bool rads_)
{
    return nsmat4<T>().rotation_from(axis_angle_, rads_);
}

template<class T>
nsmat4<T> rotation_mat4(const vector3<T> &euler_, typename vector3<T>::euler_order order_, bool rads_)
{
    return nsmat4<T>().rotation_from(euler_, order_, rads_);
}

template<class T>
nsmat4<T> rotation_mat4(const quaternion<T> &orientation_)
{
    return nsmat4<T>().rotation_from(orientation_);
}

template<class T>
nsmat4<T> rotation_mat4(const vector3<T> &vec_, const vector3<T> &to_vec_)
{
    return nsmat4<T>().rotation_from(vec_, to_vec_);
}

template<class T>
nsmat4<T> rotationMat4(const nsmat3<T> &transform_)
{
    return nsmat4<T>().rotation_from(transform_);
}

template<class T>
nsmat4<T> rotation_mat4(const nsmat4<T> &transform_)
{
    return nsmat4<T>().rotation_from(transform_);
}

template<class T>
nsmat4<T> scaling_mat4(const vector3<T> &scale_)
{
    return nsmat4<T>().scaling_from(scale_);
}

template<class T>
nsmat4<T> scaling_mat4(const nsmat3<T> &transform_)
{
    return nsmat4<T>().scaling_from(transform_);
}

template<class T>
nsmat4<T> scaling_mat4(const nsmat4<T> &transform_)
{
    return nsmat4<T>().scaling_from(transform_);
}

template<class T>
nsmat4<T> translation_mat4(const vector3<T> &pos_)
{
    return nsmat4<T>().translation_from(pos_);
}

template<class T>
nsmat4<T> translation_mat4(const vector4<T> &posw_)
{
    return nsmat4<T>().translation_from(posw_);
}

template<class T>
nsmat4<T> translation_mat4(const nsmat4<T> &transform_)
{
    return nsmat4<T>().translation_from(transform_);
}

template<class T>
nsmat4<T> transpose(nsmat4<T> mat_)
{
    return mat_.transpose();
}

template<class T>
nsmat4<T> inverse(nsmat4<T> mat_)
{
    return mat_.invert();
}

template<class T>
std::ostream &operator<<(std::ostream &os, const nsmat4<T> &mat)
{
    using namespace math;
    os << PRINT_MAT_START << mat[0] << PRINT_MAT_DELIMITER << mat[1] << PRINT_MAT_DELIMITER << mat[2] << PRINT_MAT_DELIMITER << mat[3]
       << PRINT_MAT_END;
    return os;
}

using i8mat4 = nsmat4<i8>;
using i16mat4 = nsmat4<i16>;
using imat4 = nsmat4<i32>;
using i64mat4 = nsmat4<i64>;
using u8mat4 = nsmat4<u8>;
using u16mat4 = nsmat4<u16>;
using umat4 = nsmat4<u32>;
using u64mat4 = nsmat4<u64>;
using mat4 = nsmat4<float>;
using dmat4 = nsmat4<double>;
using ldmat4 = nsmat4<ldouble>;
}