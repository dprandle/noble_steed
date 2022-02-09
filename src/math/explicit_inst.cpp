#include "vector2.h"
#include "vector2.cpp"

namespace noble_steed
{
template struct vector2<i8>;
template struct vector2<i16>;
template struct vector2<i32>;
template struct vector2<i64>;
template struct vector2<u8>;
template struct vector2<u16>;
template struct vector2<u32>;
template struct vector2<u64>;
template struct vector2<float>;
template struct vector2<double>;
template struct vector2<ldouble>;

#define vec2global_operators(type)                                                                                                                   \
    template vector2<type> operator*(const type &lhs_, const vector2<type> &rhs_);                                                                   \
    template vector2<type> operator/(const type &lhs_, const vector2<type> &rhs_);                                                                   \
    template std::ostream &operator<<(std::ostream &os, const vector2<type> &vec)

vec2global_operators(i8);
vec2global_operators(i16);
vec2global_operators(i32);
vec2global_operators(i64);
vec2global_operators(u8);
vec2global_operators(u16);
vec2global_operators(u32);
vec2global_operators(u64);
vec2global_operators(float);
vec2global_operators(double);
vec2global_operators(ldouble);

namespace math
{
#define vec2funcs(type)                                                                                                                              \
    template noble_steed::vector2<type> to_cartesian(const noble_steed::vector2<type> &polar);                                                                                 \
    template noble_steed::vector2<type> to_polar(const noble_steed::vector2<type> &cartesian);                                                                                 \
    template noble_steed::vector2<type> scaling_vec2(const noble_steed::nsmat2<type> &tform);                                                                                  \
    template noble_steed::vector2<type> scaling_vec2(const noble_steed::nsmat3<type> &tform);                                                                                  \
    template noble_steed::vector2<type> translation_vec2(const noble_steed::nsmat2<type> &tform);                                                                              \
    template noble_steed::vector2<type> translation_vec2(const noble_steed::nsmat3<type> &tform)

vec2funcs(i8);
vec2funcs(i16);
vec2funcs(i32);
vec2funcs(i64);
vec2funcs(u8);
vec2funcs(u16);
vec2funcs(u32);
vec2funcs(u64);
vec2funcs(float);
vec2funcs(double);
vec2funcs(ldouble);

}
} // namespace noble_steed