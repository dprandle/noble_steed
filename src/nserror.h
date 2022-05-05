#pragma once
#include "nsstring.h"

namespace noble_steed
{

namespace err_code
{
constexpr const inline int NO_ERROR = 0;
}

struct err_desc
{
    int type{0};
    int code{0};
    int flags{0};
};

template<class T>
struct result_err_desc
{
    err_desc err;
    T val;
};

bool error_occured(const err_desc &err);

void error_to_string(const err_desc &err, string *str);


template<class T>
bool error_occured(const result_err_desc<T> &err)
{
    return error_occured(err.err);
}

template<class T>
void error_to_string(const result_err_desc<T> &err, string *str)
{
    error_to_string(err.err, str);
}

} // namespace noble_steed
