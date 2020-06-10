#include <noble_steed/io/filesystem.h>
#include <noble_steed/core/context.h>

const noble_steed::u32 MAX_PATH = 200;

#if defined(PLATFORM_POSIX) || defined(__linux__)
#elif defined(_WIN32)
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace noble_steed
{
String executable_name()
{
    String ret;
#if defined(PLATFORM_POSIX) || defined(__linux__)
    std::ifstream("/proc/self/comm") >> ret;
#elif defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    ret = buffer;
#elif defined(__APPLE__)
    u32 buf_size = MAX_PATH;
    char * buffer = ns_ctxt.malloc_array<char>(buf_size);
    if (_NSGetExecutablePath(buffer, &buf_size) != 0)
    {
        ns_ctxt.free_array(buffer);
        buffer = ns_ctxt.malloc_array<char>(buf_size);
        _NSGetExecutablePath(buffer, &buf_size);
    }
    ret = buffer;
    size_t ind = ret.find_last_of('/');
    if (ind != String::npos)
        ret = ret.substr(ind);
#endif
    return ret;
}

}