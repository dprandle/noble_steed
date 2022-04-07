#pragma once
#include "basic_types.h"

namespace noble_steed
{
struct string
{
    int cap {0};
    int size {0};
    char *data {};
};

template<sizet N>
struct fixed_str
{
    char buf[N] {};
    string str{N, 0, buf};
};

using small_str = fixed_str<32>;

void copy_str(string *dest, const string &source);
void copy_str(string * dest, const char *source);

template<sizet N>
void copy_str(fixed_str<N> *dest, const string &source)
{
    copy_str(dest->str, source);
}

template<sizet N>
void copy_str(fixed_str<N> *dest, const char *source)
{
    copy_str(dest->str, source);
}

template<sizet N, sizet M>
void copy_str(fixed_str<N> *dest, const fixed_str<M> &source)
{
    static_assert(N >= M);
    copy_str(dest->str, source.str);
}

} // namespace noble_steed
