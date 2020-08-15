#include <noble_steed/io/filesystem.h>
#include <noble_steed/core/context.h>
#include <fstream>

const noble_steed::u32 MAX_PATH = 200;

#if defined(PLATFORM_POSIX) || defined(__linux__)
#elif defined(_WIN32)
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace std
{
namespace experimental
{
namespace filesystem
{
bool read_file_to_string(const noble_steed::String & fname, noble_steed::String & read_into)
{
    std::ifstream ifs;
    ifs.open(fname,std::ifstream::in);
    if (ifs.is_open())
    {
        read_into.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        return true;
    }
    return false;
}

noble_steed::String executable_name()
{
    using namespace noble_steed;
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

} // namespace filesystem
} // namespace experimental
} // namespace std