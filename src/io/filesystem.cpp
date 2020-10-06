#include <noble_steed/io/filesystem.h>
#include <noble_steed/core/context.h>
#include <fstream>

#include <unistd.h>

const noble_steed::u32 MAX_PATH = 200;

#if defined(PLATFORM_POSIX) || defined(__linux__)
#include <sys/wait.h>
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

bool read_file_to_buffer(const noble_steed::String & fname, noble_steed::i8_Vector & buffer)
{
    std::ifstream ifs;
    ifs.open(fname,std::ifstream::in);
    if (ifs.is_open())
    {
        ifs.seekg(0,std::ios::end);
        noble_steed::sizet sz = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        buffer.resize(sz);
        ifs.read((char*)buffer.data(),sz);
        ifs.close();
        return true;
    }
    return false;
}

bool read_file_to_string(const noble_steed::String & fname, noble_steed::String & read_into)
{
    std::ifstream ifs;
    ifs.open(fname,std::ifstream::in);
    if (ifs.is_open())
    {
        read_into.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        ifs.close();
        return true;
    }
    return false;
}

bool write_string_to_file(const noble_steed::String & fname, const noble_steed::String & string)
{
    std::ofstream ofs;
    ofs.open(fname, std::ofstream::out);
    if (ofs.is_open())
    {
        ofs << string;
        ofs.close();
        return true;
    }
    return false;
}

void fork_and_wait(const noble_steed::String & exec_path, const noble_steed::Vector<noble_steed::String> & exec_args, noble_steed::String & prog_output)
{
    int fd[2];
    pipe(fd);
    int pid = fork();
    if (pid == 0)
    {
        // child process
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        noble_steed::Vector<char*> args;
        args.resize(exec_args.size()+2);
        args[0] = (char*)exec_path.c_str();
        args[args.size()-1] = nullptr;

        for (int i = 0; i < exec_args.size(); ++i)
            args[i+1] = (char*)exec_args[i].c_str();

        execv(exec_path.c_str(), args.data());
        dlog("Error executing {} with {} args - error: {}",exec_path,args.size(),strerror(errno));
        _exit(0);
    }
    else
    {
        char buff[10000];
        int status;
        waitpid(pid,&status,0);
        int read_cnt = read(fd[0],buff,10000);
        prog_output.resize(read_cnt);
        for (int i = 0; i < read_cnt; ++i)
            prog_output[i] = buff[i];
    }
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