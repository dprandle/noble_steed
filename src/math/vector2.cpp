#include "algorithm.h"
#include "vector2.h"
#include "nsmat2.h"
#include "nsmat3.h"

namespace noble_steed
{
template<class T>
vector2<T>::vector2(const T &init_) : data{init_, init_}
{}

template<class T>
vector2<T>::vector2(const T &x_, const T &y_) : data{x_, y_}
{}

template<class T>
vector2<T> vector2<T>::minmax() const
{
    if (x <= y)
        return {x, y};
    else
        return {y, x};
}

template<class T>
T vector2<T>::angle() const
{
    return math::angle(*this, {1, 0});
}

template<class T>
T vector2<T>::length() const
{
    return sqrt(length_sq());
}

template<class T>
T vector2<T>::length_sq() const
{
    return x * x + y * y;
}

template<class T>
vector2<T> &vector2<T>::set_length(const T &len_)
{
    T l = length();
    if (l < math::FLOAT_EPS)
        return *this;
    return (*this) *= (len_ / l);
}

template<class T>
vector2<T> &vector2<T>::rotate(T angle_)
{
    T new_angle = angle_ + angle();
    x = length() * std::cos(new_angle);
    y = length() * std::sin(new_angle);
    return *this;
}

template<class T>
vector2<T> &vector2<T>::project(const vector2<T> &vec_)
{
    T denom = math::dot(vec_, vec_);

    if constexpr (std::is_floating_point_v<T>)
    {
        if (math::fequals(denom, T(0.0), T(0.0001)))
            return *this;
    }
    else
    {
        if (denom == 0)
            return *this;
    }
    return *this = T(math::dot(*this, vec_) / denom) * vec_;
}

template<class T>
vector2<T> &vector2<T>::project_plane(const vector2<T> &normal)
{
    return *this -= math::project(*this, normal);
}

template<class T>
vector2<T> &vector2<T>::reflect(const vector2<T> &normal)
{
    return *this -= T(2 * math::dot(*this, normal)) * normal;
}

template<class T>
vector2<T> &vector2<T>::normalize()
{
    return set_length(1);
}

template<class T>
vector2<T> &vector2<T>::abs()
{
    if constexpr (std::is_signed_v<T>)
    {
        x = std::abs(x);
        y = std::abs(y);
    }
    return *this;
}

template<class T>
vector2<T> &vector2<T>::ceil()
{
    x = std::ceil(x);
    y = std::ceil(y);
    return *this;
}

template<class T>
vector2<T> &vector2<T>::floor()
{
    x = std::floor(x);
    y = std::floor(y);
    return *this;
}

template<class T>
vector2<T> &vector2<T>::round(i8 decimal_places)
{
    x = math::round_decimal(x, decimal_places);
    y = math::round_decimal(y, decimal_places);
    return *this;
}

template<class T>
vector2<T> &vector2<T>::round()
{
    x = std::round(x);
    y = std::round(y);
    return *this;
}

template<class T>
vector2<T> vector2<T>::operator+(const vector2<T> &rhs_) const
{
    return {x + rhs_.x, y + rhs_.y};
}

template<class T>
vector2<T> vector2<T>::operator-(const vector2<T> &rhs_) const
{
    return {x - rhs_.x, y - rhs_.y};
}

template<class T>
vector2<T> vector2<T>::operator*(const vector2<T> &rhs_) const
{
    return {x * rhs_.x, y * rhs_.y};
}

template<class T>
vector2<T> vector2<T>::operator/(const vector2<T> &rhs_) const
{
    return {x / rhs_.x, y / rhs_.y};
}

template<class T>
vector2<T> vector2<T>::operator*(const T &rhs_) const
{
    return {x * rhs_, y * rhs_};
}

template<class T>
vector2<T> vector2<T>::operator/(const T &rhs_) const
{
    if constexpr (std::is_floating_point_v<T>)
        return *this * (1.0 / rhs_);
    else
        return {x / rhs_, y / rhs_};
}

template<class T>
vector2<T> vector2<T>::operator++(i32)
{
    vector2<T> ret(*this);
    ++(*this);
    return ret;
}

template<class T>
vector2<T> vector2<T>::operator--(i32)
{
    vector2<T> ret(*this);
    --(*this);
    return ret;
}

template<class T>
vector2<T> &vector2<T>::operator++()
{
    ++x;
    ++y;
    return *this;
}

template<class T>
vector2<T> &vector2<T>::operator--()
{
    --x;
    --y;
    return *this;
}

template<class T>
vector2<T> &vector2<T>::operator+=(const vector2<T> &rhs_)
{
    return *this = *this + rhs_;
}

template<class T>
vector2<T> &vector2<T>::operator-=(const vector2<T> &rhs_)
{
    return *this = *this - rhs_;
}

template<class T>
vector2<T> &vector2<T>::operator*=(const vector2<T> &rhs_)
{
    return *this = *this * rhs_;
}

template<class T>
vector2<T> &vector2<T>::operator/=(const vector2<T> &rhs_)
{
    return *this = *this / rhs_;
}

template<class T>
vector2<T> &vector2<T>::operator*=(const T &rhs_)
{
    return *this = *this * rhs_;
}

template<class T>
vector2<T> &vector2<T>::operator/=(const T &rhs_)
{
    return *this = *this / rhs_;
}

template<class T>
bool vector2<T>::operator==(const vector2<T> &rhs_) const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return (math::fequals(x, rhs_.x) && math::fequals(y, rhs_.y));
    }
    else
        return ((x == rhs_.x) && (y == rhs_.y));
}

template<class T>
bool vector2<T>::operator<(const vector2<T> &rhs_) const
{
    return (x < rhs_.x && y < rhs_.y);
}

template<class T>
bool vector2<T>::operator<=(const vector2<T> &rhs_) const
{
    return !(*this > rhs_);
}

template<class T>
bool vector2<T>::operator>(const vector2<T> &rhs_) const
{
    return !(*this == rhs_ || *this < rhs_);
}

template<class T>
bool vector2<T>::operator>=(const vector2<T> &rhs_) const
{
    return !(*this < rhs_);
}

template<class T>
bool vector2<T>::operator!=(const vector2<T> &rhs_) const
{
    return !(*this == rhs_);
}

template<class T>
const T &vector2<T>::operator[](sizet val_) const
{
    assert(val_ < 2 && "Index out of bounds!");
    return data[val_];
}

template<class T>
T &vector2<T>::operator[](sizet val_)
{
    assert(val_ < 2 && "Index out of bounds!");
    return data[val_];
}

template<class T>
vector2<T> operator*(const T &lhs_, const vector2<T> &rhs_)
{
    return rhs_ * lhs_;
}

template<class T>
vector2<T> operator/(const T &lhs_, const vector2<T> &rhs_)
{
    return {lhs_ / rhs_.x, lhs_ / rhs_.y};
}

template<class T>
std::ostream &operator<<(std::ostream &os, const vector2<T> &vec)
{
    using namespace math;
    os << STR_ALIGN << PRINT_START_VEC << vec.x << PRINT_VEC_DELIMITER << vec.y << PRINT_END_VEC;
    return os;
}

namespace math
{
template<class T>
noble_steed::vector2<T> to_polar(const noble_steed::vector2<T> &cartesian)
{
    return {cartesian.length(), cartesian.angle()};
}

template<class T>
noble_steed::vector2<T> to_cartesian(const noble_steed::vector2<T> &polar)
{
    return {polar.x * std::cos(polar.y), polar.x * std::sin(polar.y)};
}

template<class T>
noble_steed::vector2<T> scaling_vec2(const noble_steed::nsmat2<T> &tform)
{
    return {tform[0].length(), tform[1].length()};
}

template<class T>
noble_steed::vector2<T> scaling_vec2(const noble_steed::nsmat3<T> &tform)
{
    return {tform[0].vec2().length(), tform[1].vec2().length()};
}

template<class T>
noble_steed::vector2<T> translation_vec2(const noble_steed::nsmat2<T> &tform)
{
    return tform(2);
}

template<class T>
noble_steed::vector2<T> translation_vec2(const noble_steed::nsmat3<T> &tform)
{
    return tform(2).vec2();
}
} // namespace math
} // namespace noble_steed