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

template nsvec2<float> operator*(const float &lhs_, const nsvec2<float> &rhs_);
template nsvec2<float> operator/(const float &lhs_, const nsvec2<float> &rhs_);
template nsvec2<float> to_cartesian(const nsvec2<float> &polar);
template nsvec2<float> to_polar(const nsvec2<float> &cartesian);
template nsvec2<float> scaling_vec2(const nsmat2<float> &tform);
template nsvec2<float> scaling_vec2(const nsmat3<float> &tform);
template nsvec2<float> translation_vec2(const nsmat2<float> &tform);
template nsvec2<float> translation_vec2(const nsmat3<float> &tform);
template std::ostream &operator<<(std::ostream &os, const nsvec2<float> &vec);