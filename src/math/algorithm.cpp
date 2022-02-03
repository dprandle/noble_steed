#include <cmath>
#include "algorithm.h"

namespace noble_steed
{

float lerp(float low, float high, float middle)
{
    return (middle - low) / (high - low);
}

double lerp(double low, double high, double middle)
{
    return (middle - low) / (high - low);
}

float lerp(i32 low, i32 high, i32 middle)
{
    return float(middle - low) / float(high - low);
}

float lerp(u32 low, u32 high, u32 middle)
{
    return float(middle - low) / float(high - low);
}

float random_float(float high, float low)
{
    return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

} // namespace noble_steed
