#pragma once

#include "../core/basic_types.h"

namespace noble_steed
{
#define PI 3.14159265359f
#define FLOAT_EPS 0.00001f

float lerp(i32 low_, i32 high_, i32 middle_);
float lerp(u32 low_, u32 high_, u32 middle_);
float lerp(float low_, float high_, float middle_);
double lerp(double low_, double high_, double middle_);

template<class T>
T degrees(const T & val_)
{
	return (180 / PI) * val_;
}

template<class T>
T radians(const T & val_)
{
	return (PI / 180) * val_;
}

float random_float(float high_ = 1.0f, float low_ = 0.0f);

}