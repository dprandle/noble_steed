#include "nsvec2.h"
#include "nsvec2.cpp"

template struct nsvec2<i8>;
template struct nsvec2<i16>;
template struct nsvec2<i32>;
template struct nsvec2<i64>;
template struct nsvec2<u8>;
template struct nsvec2<u16>;
template struct nsvec2<u32>;
template struct nsvec2<u64>;
template struct nsvec2<float>;
template struct nsvec2<double>;
template struct nsvec2<ldouble>;

#define vec2funcs(type) template nsvec2<type> operator*(const type &lhs_, const nsvec2<type> &rhs_); \
template nsvec2<type> operator/(const type &lhs_, const nsvec2<type> &rhs_); \
template nsvec2<type> to_cartesian(const nsvec2<type> &polar); \
template nsvec2<type> to_polar(const nsvec2<type> &cartesian); \
template nsvec2<type> scaling_vec2(const nsmat2<type> &tform); \
template nsvec2<type> scaling_vec2(const nsmat3<type> &tform); \
template nsvec2<type> translation_vec2(const nsmat2<type> &tform); \
template nsvec2<type> translation_vec2(const nsmat3<type> &tform); \
template std::ostream &operator<<(std::ostream &os, const nsvec2<type> &vec);

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
