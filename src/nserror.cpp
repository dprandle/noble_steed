#include <stdio.h>

#include "nserror.h"

namespace noble_steed
{

bool error_occured(const err_desc &err)
{
    return (err.type != err_code::NO_ERROR);
}

void error_to_string(const err_desc &err, string *str)
{
    clear_str(str);
    snprintf(str->data, str->cap, "General err type %d, code %d, flags %d", err.type, err.code, err.flags);
}

} // namespace noble_steed