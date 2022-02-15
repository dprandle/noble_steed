#pragma once

#include "nsmat3.h"

namespace noble_steed
{
template<class T>
struct matrix4
{
    // Identity
    matrix4() : data{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    {}

    matrix4(const T &val_) : data{{val_}, {val_}, {val_}, {val_}}
    {}

    //
    matrix4(const vector4<T> &row1_, const vector4<T> &row2_, const vector4<T> &row3_, const vector4<T> &row4_)
        : row1(row1_), row2(row2_), row3(row3_), row4(row4_)
    {}

    matrix4(const nsmat3<T> &basis) : row1(basis[0], 0), row2(basis[1], 0), row3(basis[2], 0), row4(basis[3], 1)
    {}

    COMMON_OPERATORS(matrix4<T>, 4, vector4<T>)

    vector4<T> operator()(sizet ind) const
    {
        return {data[0][ind], data[1][ind], data[2][ind], data[3][ind]};
    }

    union
    {
        vector4<T> data[size_];
        struct
        {
            vector4<T> row1;
            vector4<T> row2;
            vector4<T> row3;
            vector4<T> row4;
        };
    };
};

template<class T>
vector4<T> operator*(const matrix4<T> &lhs, const vector4<T> &rhs)
{
    using namespace math;
    return {dot(lhs[0], rhs), dot(lhs[1], rhs), dot(lhs[2], rhs), dot(lhs[3] * rhs)};
}

template<class T>
vector4<T> operator*(const vector4<T> &lhs, const matrix4<T> &rhs)
{
    vector4<T> ret;
    ret[0] = lhs[0] * rhs[0][0] + lhs[1] * rhs[1][0] + lhs[2] * rhs[2][0] + lhs[3] * rhs[3][0];
    ret[1] = lhs[0] * rhs[0][1] + lhs[1] * rhs[1][1] + lhs[2] * rhs[2][1] + lhs[3] * rhs[3][1];
    ret[2] = lhs[0] * rhs[0][2] + lhs[1] * rhs[1][2] + lhs[2] * rhs[2][2] + lhs[3] * rhs[3][2];
    ret[3] = lhs[0] * rhs[0][3] + lhs[1] * rhs[1][3] + lhs[2] * rhs[2][3] + lhs[3] * rhs[3][3];
    return ret;
}

template<floating_pt T>
vector4<T> operator/(const matrix4<T> &lhs, const vector4<T> &rhs)
{
    using namespace math;
    T mult = 1 / dot(rhs, rhs);
    return {dot(lhs[0], rhs), math::dot(lhs[1], rhs), dot(lhs[2], rhs), dot(lhs[3] * rhs)};
}

template<integral T>
vector4<T> operator/(const matrix4<T> &lhs, const vector4<T> &rhs)
{
    using namespace math;
    T lensq = dot(rhs, rhs);
    return {dot(lhs[0], rhs) / lensq, dot(lhs[1], rhs) / lensq, dot(lhs[2], rhs) / lensq, dot(lhs[3] * rhs) / lensq};
}

template<class T>
matrix4<T> operator*(const matrix4<T> &lhs, const matrix4<T> &rhs)
{
    matrix4<T> ret;
    ret[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0] + lhs[0][3] * rhs[3][0];
    ret[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1] + lhs[0][3] * rhs[3][1];
    ret[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2] + lhs[0][3] * rhs[3][2];
    ret[0][3] = lhs[0][0] * rhs[0][3] + lhs[0][1] * rhs[1][3] + lhs[0][2] * rhs[2][3] + lhs[0][3] * rhs[3][3];

    ret[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0] + lhs[1][3] * rhs[3][0];
    ret[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1] + lhs[1][3] * rhs[3][1];
    ret[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2] + lhs[1][3] * rhs[3][2];
    ret[1][3] = lhs[1][0] * rhs[0][3] + lhs[1][1] * rhs[1][3] + lhs[1][2] * rhs[2][3] + lhs[1][3] * rhs[3][3];

    ret[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0] + lhs[2][3] * rhs[3][0];
    ret[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1] + lhs[2][3] * rhs[3][1];
    ret[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2] + lhs[2][3] * rhs[3][2];
    ret[2][3] = lhs[2][0] * rhs[0][3] + lhs[2][1] * rhs[1][3] + lhs[2][2] * rhs[2][3] + lhs[2][3] * rhs[3][3];

    ret[3][0] = lhs[3][0] * rhs[0][0] + lhs[3][1] * rhs[1][0] + lhs[3][2] * rhs[2][0] + lhs[3][3] * rhs[3][0];
    ret[3][1] = lhs[3][0] * rhs[0][1] + lhs[3][1] * rhs[1][1] + lhs[3][2] * rhs[2][1] + lhs[3][3] * rhs[3][1];
    ret[3][2] = lhs[3][0] * rhs[0][2] + lhs[3][1] * rhs[1][2] + lhs[3][2] * rhs[2][2] + lhs[3][3] * rhs[3][2];
    ret[3][3] = lhs[3][0] * rhs[0][3] + lhs[3][1] * rhs[1][3] + lhs[3][2] * rhs[2][3] + lhs[3][3] * rhs[3][3];
    return ret;
}

template<class T>
matrix4<T> operator/(const matrix4<T> &lhs, const matrix4<T> &rhs)
{
    return lhs * inverse(rhs);
}

namespace math
{

template<class T>
nsmat3<T> basis(const matrix4<T> &mat)
{
    return {mat.row1.xyz, mat.row2.xyz, mat.row3.xyz};
}

template<class T>
void transpose(matrix4<T> *mat)
{
    T tmp = (*mat)[1][0];
    (*mat)[1][0] = (*mat)[0][1];
    (*mat)[0][1] = tmp;
    tmp = (*mat)[2][0];
    (*mat)[2][0] = (*mat)[0][2];
    (*mat)[0][2] = tmp;
    tmp = (*mat)[2][1];
    (*mat)[2][1] = (*mat)[1][2];
    (*mat)[1][2] = tmp;

    tmp = (*mat)[3][0];
    (*mat)[3][0] = (*mat)[0][3];
    (*mat)[0][3] = tmp;
    tmp = (*mat)[3][1];
    (*mat)[3][1] = (*mat)[1][3];
    (*mat)[1][3] = tmp;
    tmp = (*mat)[3][2];
    (*mat)[3][2] = (*mat)[2][3];
    (*mat)[2][3] = tmp;
}

template<class T>
matrix4<T> transpose(matrix4<T> mat)
{
    transpose(&mat);
    return mat;
}

template<class T>
T determinant(const matrix4<T> &mat)
{
    return mat[0][3] * mat[1][2] * mat[2][1] * mat[3][0] - mat[0][2] * mat[1][3] * mat[2][1] * mat[3][0] -
           mat[0][3] * mat[1][1] * mat[2][2] * mat[3][0] + mat[0][1] * mat[1][3] * mat[2][2] * mat[3][0] +
           mat[0][2] * mat[1][1] * mat[2][3] * mat[3][0] - mat[0][1] * mat[1][2] * mat[2][3] * mat[3][0] -
           mat[0][3] * mat[1][2] * mat[2][0] * mat[3][1] + mat[0][2] * mat[1][3] * mat[2][0] * mat[3][1] +
           mat[0][3] * mat[1][0] * mat[2][2] * mat[3][1] - mat[0][0] * mat[1][3] * mat[2][2] * mat[3][1] -
           mat[0][2] * mat[1][0] * mat[2][3] * mat[3][1] + mat[0][0] * mat[1][2] * mat[2][3] * mat[3][1] +
           mat[0][3] * mat[1][1] * mat[2][0] * mat[3][2] - mat[0][1] * mat[1][3] * mat[2][0] * mat[3][2] -
           mat[0][3] * mat[1][0] * mat[2][1] * mat[3][2] + mat[0][0] * mat[1][3] * mat[2][1] * mat[3][2] +
           mat[0][1] * mat[1][0] * mat[2][3] * mat[3][2] - mat[0][0] * mat[1][1] * mat[2][3] * mat[3][2] -
           mat[0][2] * mat[1][1] * mat[2][0] * mat[3][3] + mat[0][1] * mat[1][2] * mat[2][0] * mat[3][3] +
           mat[0][2] * mat[1][0] * mat[2][1] * mat[3][3] - mat[0][0] * mat[1][2] * mat[2][1] * mat[3][3] -
           mat[0][1] * mat[1][0] * mat[2][2] * mat[3][3] + mat[0][0] * mat[1][1] * mat[2][2] * mat[3][3];
}

template<class T>
matrix4<T> inverse(const matrix4<T> &mat)
{
    T det = determinant(mat);
    if constexpr (std::is_floating_point_v<T>)
    {
        if (fequals(det, 0))
            return matrix4<T>();
    }
    else
    {
        if (det == 0)
            return matrix4<T>();
    }

    matrix4<T> ret;
    ret[0][0] = mat[1][2] * mat[2][3] * mat[3][1] - mat[1][3] * mat[2][2] * mat[3][1] + mat[1][3] * mat[2][1] * mat[3][2] -
                mat[1][1] * mat[2][3] * mat[3][2] - mat[1][2] * mat[2][1] * mat[3][3] + mat[1][1] * mat[2][2] * mat[3][3];
    ret[0][1] = mat[0][3] * mat[2][2] * mat[3][1] - mat[0][2] * mat[2][3] * mat[3][1] - mat[0][3] * mat[2][1] * mat[3][2] +
                mat[0][1] * mat[2][3] * mat[3][2] + mat[0][2] * mat[2][1] * mat[3][3] - mat[0][1] * mat[2][2] * mat[3][3];
    ret[0][2] = mat[0][2] * mat[1][3] * mat[3][1] - mat[0][3] * mat[1][2] * mat[3][1] + mat[0][3] * mat[1][1] * mat[3][2] -
                mat[0][1] * mat[1][3] * mat[3][2] - mat[0][2] * mat[1][1] * mat[3][3] + mat[0][1] * mat[1][2] * mat[3][3];
    ret[0][3] = mat[0][3] * mat[1][2] * mat[2][1] - mat[0][2] * mat[1][3] * mat[2][1] - mat[0][3] * mat[1][1] * mat[2][2] +
                mat[0][1] * mat[1][3] * mat[2][2] + mat[0][2] * mat[1][1] * mat[2][3] - mat[0][1] * mat[1][2] * mat[2][3];
    ret[1][0] = mat[1][3] * mat[2][2] * mat[3][0] - mat[1][2] * mat[2][3] * mat[3][0] - mat[1][3] * mat[2][0] * mat[3][2] +
                mat[1][0] * mat[2][3] * mat[3][2] + mat[1][2] * mat[2][0] * mat[3][3] - mat[1][0] * mat[2][2] * mat[3][3];
    ret[1][1] = mat[0][2] * mat[2][3] * mat[3][0] - mat[0][3] * mat[2][2] * mat[3][0] + mat[0][3] * mat[2][0] * mat[3][2] -
                mat[0][0] * mat[2][3] * mat[3][2] - mat[0][2] * mat[2][0] * mat[3][3] + mat[0][0] * mat[2][2] * mat[3][3];
    ret[1][2] = mat[0][3] * mat[1][2] * mat[3][0] - mat[0][2] * mat[1][3] * mat[3][0] - mat[0][3] * mat[1][0] * mat[3][2] +
                mat[0][0] * mat[1][3] * mat[3][2] + mat[0][2] * mat[1][0] * mat[3][3] - mat[0][0] * mat[1][2] * mat[3][3];
    ret[1][3] = mat[0][2] * mat[1][3] * mat[2][0] - mat[0][3] * mat[1][2] * mat[2][0] + mat[0][3] * mat[1][0] * mat[2][2] -
                mat[0][0] * mat[1][3] * mat[2][2] - mat[0][2] * mat[1][0] * mat[2][3] + mat[0][0] * mat[1][2] * mat[2][3];
    ret[2][0] = mat[1][1] * mat[2][3] * mat[3][0] - mat[1][3] * mat[2][1] * mat[3][0] + mat[1][3] * mat[2][0] * mat[3][1] -
                mat[1][0] * mat[2][3] * mat[3][1] - mat[1][1] * mat[2][0] * mat[3][3] + mat[1][0] * mat[2][1] * mat[3][3];
    ret[2][1] = mat[0][3] * mat[2][1] * mat[3][0] - mat[0][1] * mat[2][3] * mat[3][0] - mat[0][3] * mat[2][0] * mat[3][1] +
                mat[0][0] * mat[2][3] * mat[3][1] + mat[0][1] * mat[2][0] * mat[3][3] - mat[0][0] * mat[2][1] * mat[3][3];
    ret[2][2] = mat[0][1] * mat[1][3] * mat[3][0] - mat[0][3] * mat[1][1] * mat[3][0] + mat[0][3] * mat[1][0] * mat[3][1] -
                mat[0][0] * mat[1][3] * mat[3][1] - mat[0][1] * mat[1][0] * mat[3][3] + mat[0][0] * mat[1][1] * mat[3][3];
    ret[2][3] = mat[0][3] * mat[1][1] * mat[2][0] - mat[0][1] * mat[1][3] * mat[2][0] - mat[0][3] * mat[1][0] * mat[2][1] +
                mat[0][0] * mat[1][3] * mat[2][1] + mat[0][1] * mat[1][0] * mat[2][3] - mat[0][0] * mat[1][1] * mat[2][3];
    ret[3][0] = mat[1][2] * mat[2][1] * mat[3][0] - mat[1][1] * mat[2][2] * mat[3][0] - mat[1][2] * mat[2][0] * mat[3][1] +
                mat[1][0] * mat[2][2] * mat[3][1] + mat[1][1] * mat[2][0] * mat[3][2] - mat[1][0] * mat[2][1] * mat[3][2];
    ret[3][1] = mat[0][1] * mat[2][2] * mat[3][0] - mat[0][2] * mat[2][1] * mat[3][0] + mat[0][2] * mat[2][0] * mat[3][1] -
                mat[0][0] * mat[2][2] * mat[3][1] - mat[0][1] * mat[2][0] * mat[3][2] + mat[0][0] * mat[2][1] * mat[3][2];
    ret[3][2] = mat[0][2] * mat[1][1] * mat[3][0] - mat[0][1] * mat[1][2] * mat[3][0] - mat[0][2] * mat[1][0] * mat[3][1] +
                mat[0][0] * mat[1][2] * mat[3][1] + mat[0][1] * mat[1][0] * mat[3][2] - mat[0][0] * mat[1][1] * mat[3][2];
    ret[3][3] = mat[0][1] * mat[1][2] * mat[2][0] - mat[0][2] * mat[1][1] * mat[2][0] + mat[0][2] * mat[1][0] * mat[2][1] -
                mat[0][0] * mat[1][2] * mat[2][1] - mat[0][1] * mat[1][0] * mat[2][2] + mat[0][0] * mat[1][1] * mat[2][2];

    ret /= det;
    return ret;
}

template<floating_pt T>
matrix4<T> ortho(T left, T right, T top, T bottom, T near, T far)
{
    T w = right - left;
    T h = top - bottom;
    T p = far - near;

    T x = (right + left) / w;
    T y = (top + bottom) / h;
    T z = (far + near) / p;

    matrix4<T> ret;
    set_mat_column(&ret, 3, {-x, -y, -z, 1});
    ret[0][0] = 2 / w;
    ret[1][1] = 2 / h;
    ret[2][2] = -2 / p;
    return ret;
}

template<floating_pt T>
matrix4<T> perspective(T fov, T aspect_ratio, T z_near, T z_far)
{
    T z_range = z_near - z_far;
    T useful_num = math::tan((fov / 2) * TO_RADS);
    matrix4<T> ret;
    ret[0][0] = 1 / (aspect_ratio * useful_num);
    ret[1][1] = 1 / useful_num;
    ret[2][2] = (-z_near - z_far) / z_range;
    ret[2][3] = 2 * z_far * z_near / z_range;
    ret[3][2] = 1;
    ret[3][3] = 0;
    return ret;
}

template<class T>
matrix4<T> &rotation_from(const nsmat3<T> &transform)
{
    matrix4<T> ret(transform);
    normalize(&ret[0]);
    normalize(&ret[1]);
    normalize(&ret[2]);
    return ret;
}

template<class T>
matrix4<T> rotation_mat4(const matrix4<T> &transform)
{
    matrix4<T> ret(basis(transform));
    normalize(&ret[0]);
    normalize(&ret[1]);
    normalize(&ret[2]);
    return ret;
}

template<class T>
matrix4<T> &scaling_mat4(const vector3<T> &scale)
{
    matrix4<T> ret;
    ret[0][0] = scale.x;
    ret[1][1] = scale.y;
    ret[2][2] = scale.z;
    return ret;
}

template<class T>
matrix4<T> scaling_mat4(const nsmat3<T> &transform)
{
    matrix4<T> ret;
    ret[0][0] = length(transform[0]);
    ret[1][1] = length(transform[1]);
    ret[2][2] = length(transform[2]);
    return ret;
}

template<class T>
matrix4<T> scaling_mat4(const matrix4<T> &transform)
{
    matrix4<T> ret;
    ret[0][0] = length(transform[0].xyz);
    ret[1][1] = length(transform[1].xyz);
    ret[2][2] = length(transform[2].xyz);
    return ret;
}

template<class T>
vector3<T> right(const matrix4<T> &mat)
{
    return normalize(mat(0).xyz);
}

template<class T>
vector3<T> target(const matrix4<T> &mat)
{
    return normalize(mat(2).xyz);
}

template<class T>
vector3<T> up(const matrix4<T> &mat)
{
    return normalize(mat(1).xyz);
}

template<class T>
matrix4<T> translation_mat4(const vector3<T> &v3)
{
    matrix4<T> ret;
    set_mat_column(&ret, 3, {v3, 1});
    return ret;
}

template<class T>
matrix4<T> translation_mat4(const vector4<T> &v4)
{
    matrix4<T> ret;
    set_mat_column(&ret, 3, {v4.xyz, 1});
    return ret;
}

template<class T>
matrix4<T> translation_mat4(const matrix4<T> &transform)
{
    matrix4<T> ret;
    set_mat_column(&ret, 3, transform(3));
    ret[3][3] = 1;
    return ret;
}

} // namespace math

// Enable type trait
template<class U>
struct is_mat<matrix4<U>>
{
    static constexpr bool value = true;
};

using i8mat4 = matrix4<i8>;
using i16mat4 = matrix4<i16>;
using imat4 = matrix4<i32>;
using i64mat4 = matrix4<i64>;
using u8mat4 = matrix4<u8>;
using u16mat4 = matrix4<u16>;
using umat4 = matrix4<u32>;
using u64mat4 = matrix4<u64>;
using mat4 = matrix4<float>;
using dmat4 = matrix4<double>;
using ldmat4 = matrix4<ldouble>;
} // namespace noble_steed