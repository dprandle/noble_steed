#include "nsdebug.h"
#include "nsstring.h"

namespace noble_steed
{

void clear_str(string *str)
{
    for (sizet ind{0}; ind < str->cap; ++ind)
    {
        str->data[ind] = 0;
        ++ind;
    }
    str->size = 0;
}

void copy_str(string *dest, const string &source)
{
    nsassert(source.size <= dest->cap);
    for (sizet ind{0}; ind < dest->cap; ++ind)
    {
        if (ind < source.size)
        {
            dest->data[ind] = source.data[ind];
        }
        else
        {
            dest->data[ind] = 0;
        }
        ++ind;
    }
}

void copy_str(string *dest, const char *source)
{
    int ind = 0;
    bool reached_end_of_source = false;
    while (ind < dest->cap)
    {
        if (!reached_end_of_source && source[ind] != 0)
        {
            dest->data[ind] = source[ind];
        }
        else
        {
            reached_end_of_source = true;
            dest->data[ind] = 0;
        }
        ++ind;
    }
}

} // namespace noble_steed