#pragma once

#include "../core/basic_types.h"
#include <iomanip>

#include <iostream>

namespace noble_steed
{
#define STR_W(val, width) std::setw(width) << val
#define STR_ALIGN std::right << std::setprecision(ROUND_TO_DEC) << std::fixed
inline constexpr float PI = 3.14159265359f;
inline constexpr float FLOAT_EPS = 0.00001f;
inline constexpr char PRINT_MAT_DELIMITER = '\n';
inline constexpr char PRINT_MAT_START = '\n';
inline constexpr char PRINT_MAT_END = '\n';
inline constexpr char PRINT_VEC_DELIMITER = ' ';
inline constexpr char PRINT_START_VEC = '[';
inline constexpr char PRINT_END_VEC = ']';
inline constexpr i8 ROUND_TO_DEC = 2;

inline constexpr float TO_DEGREES = (180.0 / PI);
inline constexpr float TO_RADS = (PI / 180.0);

i8 count_digits(i32 number);
template<typename T>
bool fequals(T left, T right, T eps = FLOAT_EPS)
{
    bool result = (left < (right + FLOAT_EPS)) && (left > (right - FLOAT_EPS));
    std::cout << "Result: " << result << std::endl;
    return result;
}

float random_float(float high_ = 1.0f, float low_ = 0.0f);

double round_decimal(double to_round, i8 decimal_places);

template<class T>
T project(const T &a, const T &b)
{
    return T(a).project(b);
}

template<class T>
T project_plane(const T &vec, const T &normal)
{
    return T(vec).project_plane(normal);
}

template<class T>
T reflect(const T &vec, const T &normal)
{
    return T(vec).reflect(normal);
}

template <class T>
auto distance(const T &a, const T &b)
{
    return (a - b).length();
}

template <class T>
auto dot(const T & veca, const T & vecb)
{
    auto val {veca.data[0] * vecb.data[0]};
    for (i8 i {1}; i < T::size; ++i)
        val += veca[i] * vecb[i];
    return val;
}

template <class T>
auto angle(const T &veca, const T &vecb)
{
    auto dot_p = dot(veca, vecb);
    auto l = sqrt(veca.length_sq() * vecb.length_sq());
    if (l < FLOAT_EPS)
        return l;
    return acos(dot_p / l);
}

template<class T>
T normalize(const T &vec_or_quat)
{
    return T(vec_or_quat).normalize();
}

template<class T>
T min(const T &lhs_, const T &rhs_)
{
    T ret {lhs_};
    for (i8 i {0}; i < T::size; ++i)
    {
        if (rhs_[i] < ret[i])
            ret[i] = rhs_[i];
    }
    return ret;
}

template<class T>
T max(const T &lhs_, const T &rhs_)
{
    T ret {lhs_};
    for (i8 i {0}; i < T::size; ++i)
    {
        if (rhs_[i] > ret[i])
            ret[i] = rhs_[i];
    }
    return ret;
}

template<class T>
T abs(const T &item)
{
    return T(item).abs();
}

template<class T>
T ceil(const T &item)
{
    return T(item).ceil();
}

template<class T>
T floor(const T &item)
{
    return T(item).floor();
}

template<class T>
T round(const T &item)
{
    return T(item).round();
}

template<class T>
T round(const T &item, i8 decimal_places)
{
    return T(item).round(decimal_places);
}

} // namespace noble_steed