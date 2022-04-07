#include "filesystem.h"

//#include <unistd.h>

// #if defined(PLATFORM_POSIX) || defined(__linux__)
// #include <sys/wait.h>
// #elif defined(_WIN32)
// #elif defined(__APPLE__)
// #include <mach-o/dyld.h>
// #endif

namespace noble_steed::fs
{

// bool read_file_to_buffer(const path_str &fname, u8 *buffer)
// {
//     std::ifstream ifs;
//     ifs.open(fname, std::ifstream::in);
//     if (ifs.is_open())
//     {
//         ifs.seekg(0, std::ios::end);
//         sizet sz = ifs.tellg();
//         ifs.seekg(0, std::ios::beg);
//         buffer.resize(sz);
//         ifs.read((char *)buffer.data(), sz);
//         ifs.close();
//         return true;
//     }
//     return false;
// }

// bool read_file_to_string(const String &fname, String &read_into)
// {
//     std::ifstream ifs;
//     ifs.open(fname, std::ifstream::in);
//     if (ifs.is_open())
//     {
//         read_into.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
//         ifs.close();
//         return true;
//     }
//     return false;
// }

// bool write_string_to_file(const String &fname, const String &string)
// {
//     std::ofstream ofs;
//     ofs.open(fname, std::ofstream::out);
//     if (ofs.is_open())
//     {
//         ofs << string;
//         ofs.close();
//         return true;
//     }
//     return false;
// }

// void fork_and_wait(const String &exec_path, const vector<String> &exec_args, String &prog_output)
// {
//     int fd[2];
//     pipe(fd);
//     int pid = fork();
//     if (pid == 0)
//     {
//         // child process
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[0]);
//         close(fd[1]);

//         vector<char *> args;
//         args.resize(exec_args.size() + 2);
//         args[0] = (char *)exec_path.c_str();
//         args[args.size() - 1] = nullptr;

//         for (int i = 0; i < exec_args.size(); ++i)
//             args[i + 1] = (char *)exec_args[i].c_str();

//         execv(exec_path.c_str(), args.data());
//         _exit(0);
//     }
//     else
//     {
//         char buff[10000];
//         int status;
//         waitpid(pid, &status, 0);
//         int read_cnt = read(fd[0], buff, 10000);
//         prog_output.resize(read_cnt);
//         for (int i = 0; i < read_cnt; ++i)
//             prog_output[i] = buff[i];
//     }
// }

// const string &executable_name()
// {
//     static fixed_str<32> ret;
// #if defined(PLATFORM_POSIX) || defined(__linux__)

//     std::ifstream("/proc/self/comm") >> ret;
// #elif defined(_WIN32)
//     char buffer[MAX_PATH];
//     GetModuleFileNameA(nullptr, buffer, MAX_PATH);
//     ret = buffer;
// #elif defined(__APPLE__)
//     u32 buf_size(MAX_PATH);
//     char buffer[MAX_PATH];
//     assert(_NSGetExecutablePath(buffer, &buf_size));
//     ret = buffer;
//     size_t ind = ret.find_last_of('/');
//     if (ind != String::npos)
//         ret = ret.substr(ind);
// #endif
//     return ret;
// }

} // namespace noble_steed::io::fs