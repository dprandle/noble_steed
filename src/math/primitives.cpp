#include "primitives.h"
#include "nsvec3.h"

namespace noble_steed
{

template<class T>
rectangle<T>::rectangle(const nsvec4<T> &center_wh)
{}

template<class T>
rectangle<T>::rectangle(T x, T y, T width, T height)
{}

template<class T>
cube_base<T>::cube_base(const nsvec3<T> &min_, const nsvec3<T> &max_) : min(min_), max(max_)
{}

template<class T>
cube_base<T> cube_base<T>::operator+(const cube_base<T> &rhs)
{
    return cube_base<T>(min + rhs.min, max + rhs.max);
}

template<class T>
cube_base<T> cube_base<T>::operator-(const cube_base<T> &rhs)
{
    return cube_base<T>(min - rhs.min, max - rhs.max);
}

template<class T>
cube_base<T> &cube_base<T>::operator+=(const cube_base<T> &rhs)
{
    min += rhs.min;
    max += rhs.max;
    return *this;
}

template<class T>
cube_base<T> &cube_base<T>::operator-=(const cube_base<T> &rhs)
{
    min -= rhs.min;
    max -= rhs.max;
    return *this;
}

}