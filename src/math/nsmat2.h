#pragma once

#include "../core/basic_types.h"
#include "vector2.h"
#include "nsmat3.h"
namespace noble_steed
{
template<class T>
struct nsmat2
{
    nsmat2():data{{1,0},{0,1}}
    {}

    nsmat2(const T &val_)
    {
        data[0].x = val_;
        data[0].y = val_;
        data[1].x = val_;
        data[1].y = val_;
    }

    nsmat2(const nsmat2 &copy_)
    {
        set(copy_.data[0], copy_.data[1]);
    }

    nsmat2(const T &a, const T &b, const T &c, const T &d)
    {
        data[0].x = a;
        data[0].y = b;
        data[1].x = c;
        data[1].y = d;
    }

    nsmat2(const vector2<T> &row1, const vector2<T> &row2)
    {
        data[0] = row1;
        data[1] = row2;
    }

    T *data_ptr()
    {
        return &data[0][0];
    }

    T determinant() const
    {
        return data[0].x * data[1].y - data[1].x * data[0].y;
    }

    vector2<T> minmax() const
    {
        auto pr = std::minmax({data[0][0], data[0][1], data[1][0], data[1][1]});
        return {pr.first, pr.second};
    }

    nsmat2<T> &abs()
    {
        data[0].abs();
        data[1].abs();
        return *this;
    }

    nsmat2<T> &invert()
    {
        T det = determinant();

        if (det == 0)
            return nsmat2<T>();

        T tmp = data[0][0];
        data[0][0] = data[1][1] / det;
        data[0][1] = -data[0][1] / det;
        data[1][0] = -data[1][0] / det;
        data[1][1] = tmp / det;
        return *this;
    }

    nsmat2<T> &rotation_from(T angle_, bool rads_ = false)
    {
        if (!rads_)
            angle_ = radians(angle_);

        data[0][0] = math::cos(angle_);
        data[0][1] = math::sin(angle_);
        data[1][0] = -math::sin(angle_);
        data[1][1] = math::cos(angle_);
        return *this;
    }

    nsmat2<T> &round_to_zero()
    {
        data[0].round_to_zero();
        data[1].round_to_zero();
        return *this;
    }

    nsmat2<T> &scaling_from(const vector2<T> &scale_)
    {
        data[0][0] = scale_.x;
        data[1][1] = scale_.y;
        data[0][1] = data[1][0] = 0;
        return *this;
    }

    nsmat2<T> &scaling_from(const nsmat2<T> &transform2d_)
    {
        vector2<T> scalingVec(transform2d_[0].length(), transform2d_[1].length());
        return scaling_from(scalingVec);
    }

    nsmat2<T> &scaling_from(const nsmat3<T> &transform2d_)
    {
        vector2<T> scalingVec;
        scalingVec.x = math::sqrt(transform2d_[0][0] * transform2d_[0][0] + transform2d_[0][1] * transform2d_[0][1]);
        scalingVec.y = math::sqrt(transform2d_[1][0] * transform2d_[1][0] + transform2d_[1][1] * transform2d_[1][1]);
        return scaling_from(scalingVec);
    }

    nsmat2<T> &set_column(sizet i, const T &x, const T &y)
    {
        (*this)[0][i] = x;
        (*this)[1][i] = y;
        return *this;
    }

    nsmat2<T> &set_column(sizet i, const vector2<T> &col)
    {
        (*this)[i][0] = col.x;
        (*this)[i][1] = col.y;
        return *this;
    }

    nsmat2<T> &transpose()
    {
        T tmp = data[1][0];
        data[1][0] = data[0][1];
        data[0][1] = tmp;
        return *this;
    }

    // string to_string(bool newline_ = true) const
    // {
    // 	nsstringstream ss;
    // 	string lc = ";";

    // 	if (newline_)
    // 		lc += '\n';

    // 	ss << std::left
    // 	   << "|" << std::setw(6) << data[0][0]
    // 	   << " " << std::setw(6) << data[0][1] << lc
    // 	   << "|" << std::setw(6) << data[1][0]
    // 	   << " " << std::setw(6) << data[1][1] << " |";
    // 	return ss.str();
    // }

    // Overloaded operators
    nsmat2<T> operator*(const nsmat2<T> &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0][0] = data[0][0] * rhs_.data[0][0] + data[0][1] * rhs_.data[1][0];
        ret.data[0][1] = data[0][0] * rhs_.data[0][1] + data[0][1] * rhs_.data[1][1];
        ret.data[1][0] = data[1][0] * rhs_.data[0][0] + data[1][1] * rhs_.data[1][0];
        ret.data[1][1] = data[1][0] * rhs_.data[0][1] + data[1][1] * rhs_.data[1][1];
        return ret;
    }

    nsmat2<T> operator/(const nsmat2<T> &rhs_) const
    {
        return *this * inverse(rhs_);
    }

    nsmat2<T> operator%(const nsmat2<T> &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] % rhs_.data[0];
        ret.data[1] = data[1] % rhs_.data[1];
        return ret;
    }

    vector2<T> operator*(const vector2<T> &rhs_) const
    {
        return vector2<T>(data[0] * rhs_, data[1] * rhs_);
    }

    nsmat2<T> operator%(const vector2<T> &rhs_) const
    {
        return nsmat2<T>(data[0] % rhs_, data[1] % rhs_);
    }

    nsmat2<T> operator/(const vector2<T> &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] / rhs_[0];
        ret.data[1] = data[1] / rhs_[1];
        return ret;
    }

    nsmat2<T> operator*(const T &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] * rhs_;
        ret.data[1] = data[1] * rhs_;
        return ret;
    }

    nsmat2<T> operator/(const T &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] / rhs_;
        ret.data[1] = data[1] / rhs_;
        return ret;
    }

    nsmat2<T> operator+(const nsmat2<T> &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] + rhs_.data[0];
        ret.data[1] = data[1] + rhs_.data[1];
        return ret;
    }

    nsmat2<T> operator-(const nsmat2<T> &rhs_) const
    {
        nsmat2<T> ret;
        ret.data[0] = data[0] - rhs_.data[0];
        ret.data[1] = data[1] - rhs_.data[1];
        return ret;
    }

    bool operator==(const nsmat2<T> &rhs_) const
    {
        return (data[0] == rhs_.data[0] && data[1] == rhs_.data[1]);
    }

    bool operator!=(const nsmat2<T> &rhs_) const
    {
        return !(*this == rhs_);
    }

    nsmat2<T> &operator=(const nsmat2<T> &rhs_)
    {
        if (this == &rhs_)
            return *this;

        data[0] = rhs_.data[0];
        data[1] = rhs_.data[1];
        return *this;
    }

    nsmat2<T> &operator*=(const nsmat2<T> &rhs_)
    {
        *this = *this * rhs_;
        return *this;
    }

    nsmat2<T> &operator%=(const nsmat2<T> &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat2<T> &operator/=(const nsmat2<T> &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat2<T> &operator%=(const vector2<T> &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat2<T> &operator/=(const vector2<T> &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat2<T> &operator*=(const T &rhs_)
    {
        *this = *this % rhs_;
        return *this;
    }

    nsmat2<T> &operator/=(const T &rhs_)
    {
        *this = *this / rhs_;
        return *this;
    }

    nsmat2<T> operator++(i32)
    {
        nsmat2<T> ret(*this);
        ++(*this);
        return ret;
    }

    nsmat2<T> operator--(i32)
    {
        nsmat2<T> ret(*this);
        --(*this);
        return ret;
    }

    nsmat2<T> &operator++()
    {
        ++data[0];
        ++data[1];
        return *this;
    }

    nsmat2<T> &operator--()
    {
        --data[0];
        --data[1];
        return *this;
    }

    nsmat2<T> &operator+=(const nsmat2<T> &rhs_)
    {
        *this = *this + rhs_;
        return *this;
    }

    nsmat2<T> &operator-=(const nsmat2<T> &rhs_)
    {
        *this = *this - rhs_;
        return *this;
    }

    const vector2<T> &operator[](sizet val_) const
    {
        return data[val_];
    }

    vector2<T> &operator[](sizet val_)
    {
        return data[val_];
    }

    vector2<T> operator()(sizet val_) const
    {
        return vector2<T>(data[0][val_], data[1][val_]);
    }

  private:
    vector2<T> data[2];
};

template<class T>
nsmat2<T> operator*(const i32 &lhs_, const nsmat2<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat2<T> operator*(const float &lhs_, const nsmat2<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat2<T> operator*(const double &lhs_, const nsmat2<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
nsmat2<T> operator/(const i32 &lhs_, const nsmat2<T> &rhs_)
{
    return nsmat2<T>(lhs_ / rhs_[0], lhs_ / rhs_[1]);
}

template<class T>
nsmat2<T> operator/(const float &lhs_, const nsmat2<T> &rhs_)
{
    return nsmat2<T>(lhs_ / rhs_[0], lhs_ / rhs_[1]);
}

template<class T>
nsmat2<T> operator/(const double &lhs_, const nsmat2<T> &rhs_)
{
    return nsmat2<T>(lhs_ / rhs_[0], lhs_ / rhs_[1]);
}

template<class T>
vector2<T> operator*(const vector2<T> &lhs_, const nsmat2<T> &rhs_)
{
    return vector2<T>(lhs_[0] * rhs_[0][0] + lhs_[1] * rhs_[1][0], lhs_[0] * rhs_[0][1] + lhs_[1] * rhs_[1][1]);
}

template<class T>
vector2<T> operator/(const vector2<T> &lhs_, const nsmat2<T> &rhs_)
{
    return lhs_ * inverse(rhs_);
}

template<class T>
nsmat2<T> operator%(const vector2<T> &lhs_, const nsmat2<T> &rhs_)
{
    return nsmat2<T>(rhs_[0] * lhs_[0], rhs_[1] * lhs_[1]);
}

template<class T>
T determinant(const nsmat2<T> &mat_)
{
    return mat_.determinant();
}

template<class T>
nsmat2<T> abs(const nsmat2<T> &mat_)
{
    auto cpy{mat_};
    return cpy.abs();
}

template<class T>
nsmat2<T> rotation2d_mat2(const T &angle_, bool rads_)
{
    return nsmat2<T>().rotation_from(angle_, rads_);
}

template<class T>
nsmat2<T> rotation2d_mat2(const nsmat3<T> &transform2d_)
{
    return nsmat2<T>().rotation_from(transform2d_);
}

template<class T>
nsmat2<T> rotation2d_mat2(const nsmat2<T> &transform2d_)
{
    return nsmat2<T>().rotation_from(transform2d_);
}

template<class T>
nsmat2<T> scaling2d_mat2(const vector2<T> &scale_)
{
    return nsmat2<T>().scaling_from(scale_);
}

template<class T>
nsmat2<T> scaling2d_mat2(const nsmat2<T> &transform2d_)
{
    return nsmat2<T>().scaling_from(transform2d_);
}

template<class T>
nsmat2<T> scaling2d_mat2(const nsmat3<T> &transform2d_)
{
    return nsmat2<T>().scaling_from(transform2d_);
}

template<class T>
nsmat2<T> transpose(nsmat2<T> mat_)
{
    return mat_.transpose();
}

template<class T>
nsmat2<T> inverse(nsmat2<T> mat_)
{
    return mat_.invert();
}

template<class T>
std::ostream& operator<<(std::ostream& os, const nsmat2<T>& mat)
{
    os << math::PRINT_MAT_START << mat[0] << math::PRINT_MAT_DELIMITER << mat[1] << math::PRINT_MAT_END;
    return os;
}

using i8mat2 = nsmat2<i8>;
using i16mat2 = nsmat2<i16>;
using imat2 = nsmat2<i32>;
using i64mat2 = nsmat2<i64>;
using u8mat2 = nsmat2<u8>;
using u16mat2 = nsmat2<u16>;
using umat2 = nsmat2<u32>;
using u64mat2 = nsmat2<u64>;
using mat2 = nsmat2<float>;
using dmat2 = nsmat2<double>;
using ldmat2 = nsmat2<ldouble>;
}