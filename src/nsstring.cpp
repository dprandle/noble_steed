#include "nsdebug.h"
#include "nsstring.h"

namespace noble_steed
{

void copy_str(string *dest, const string &source)
{
    nsassert(source.size <= dest->cap);
    for (sizet ind{0}; ind < source.size; ++ind)
    {
        dest->data[ind] = source.data[ind];
        ++ind;
    }
}

void copy_str(string *dest, const char *source)
{
    int ind = 0;
    while (ind < dest->cap && source[ind] != '\0')
    {
        dest->data[ind] = source[ind];
        ++ind;
    }
}

} // namespace noble_steed