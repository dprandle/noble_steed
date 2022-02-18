#include <unistd.h>
#include "filesystem.h"
#include "../core/context.h"

#if defined(PLATFORM_POSIX) || defined(__linux__)
#include <sys/wait.h>
#elif defined(_WIN32)
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace noble_steed::fs
{
const u32 MAX_PATH = 200;

bool read_file_to_buffer(const String &fname, i8_vector &buffer)
{
    std::ifstream ifs;
    ifs.open(fname, std::ifstream::in);
    if (ifs.is_open())
    {
        ifs.seekg(0, std::ios::end);
        sizet sz = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        buffer.resize(sz);
        ifs.read((char *)buffer.data(), sz);
        ifs.close();
        return true;
    }
    return false;
}

bool read_file_to_string(const String &fname, String &read_into)
{
    std::ifstream ifs;
    ifs.open(fname, std::ifstream::in);
    if (ifs.is_open())
    {
        read_into.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
        ifs.close();
        return true;
    }
    return false;
}

bool write_string_to_file(const String &fname, const String &string)
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

void fork_and_wait(const String &exec_path, const vector<String> &exec_args, String &prog_output)
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

        vector<char *> args;
        args.resize(exec_args.size() + 2);
        args[0] = (char *)exec_path.c_str();
        args[args.size() - 1] = nullptr;

        for (int i = 0; i < exec_args.size(); ++i)
            args[i + 1] = (char *)exec_args[i].c_str();

        execv(exec_path.c_str(), args.data());
        _exit(0);
    }
    else
    {
        char buff[10000];
        int status;
        waitpid(pid, &status, 0);
        int read_cnt = read(fd[0], buff, 10000);
        prog_output.resize(read_cnt);
        for (int i = 0; i < read_cnt; ++i)
            prog_output[i] = buff[i];
    }
}

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
    u32 buf_size(MAX_PATH);
    char buffer[MAX_PATH];
    assert(_NSGetExecutablePath(buffer, &buf_size));
    ret = buffer;
    size_t ind = ret.find_last_of('/');
    if (ind != String::npos)
        ret = ret.substr(ind);
#endif
    return ret;
}

Path absolute(const Path &p)
{
    return std::filesystem::absolute(p);
}

Path absolute(const Path &p, Error_Code &ec)
{
    return std::filesystem::absolute(p, ec);
}

Path canonical(const Path &p)
{
    return std::filesystem::canonical(p);
}

Path canonical(const Path &p, Error_Code &ec)
{
    return std::filesystem::canonical(p, ec);
}

void copy(const Path &from, const Path &to)
{
    return std::filesystem::copy(from, to);
}
void copy(const Path &from, const Path &to, Error_Code &ec)
{
    return std::filesystem::copy(from, to, ec);
}
void copy(const Path &from, const Path &to, Copy_Opts options)
{
    return std::filesystem::copy(from, to, options);
}
void copy(const Path &from, const Path &to, Copy_Opts options, Error_Code &ec)
{
    return std::filesystem::copy(from, to, options, ec);
}

bool copy_file(const Path &from, const Path &to)
{
    return std::filesystem::copy_file(from, to);
}

bool copy_file(const Path &from, const Path &to, Error_Code &ec)
{
    return std::filesystem::copy_file(from, to, ec);
}

bool copy_file(const Path &from, const Path &to, Copy_Opts option)
{
    return std::filesystem::copy_file(from, to, option);
}

bool copy_file(const Path &from, const Path &to, Copy_Opts option, Error_Code &ec)
{
    return std::filesystem::copy_file(from, to, option, ec);
}

void copy_symlink(const Path &existing_symlink, const Path &new_symlink)
{
    return std::filesystem::copy_symlink(existing_symlink, new_symlink);
}

void copy_symlink(const Path &existing_symlink, const Path &new_symlink, Error_Code &ec) noexcept
{
    return std::filesystem::copy_symlink(existing_symlink, new_symlink, ec);
}

bool create_directory(const Path &p)
{
    return std::filesystem::create_directory(p);
}

bool create_directory(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::create_directory(p, ec);
}

bool create_directory(const Path &p, const Path &existing_p)
{
    return std::filesystem::create_directory(p, existing_p);
}

bool create_directory(const Path &p, const Path &existing_p, Error_Code &ec) noexcept
{
    return std::filesystem::create_directory(p, existing_p, ec);
}

bool create_directories(const Path &p)
{
    return std::filesystem::create_directories(p);
}

bool create_directories(const Path &p, Error_Code &ec)
{
    return std::filesystem::create_directories(p, ec);
}

void create_directory_symlink(const Path &to, const Path &new_symlink)
{
    return std::filesystem::create_directory_symlink(to, new_symlink);
}
void create_directory_symlink(const Path &to, const Path &new_symlink, Error_Code &ec) noexcept
{
    return std::filesystem::create_directory_symlink(to, new_symlink, ec);
}

void create_hard_link(const Path &to, const Path &new_hard_link)
{
    return std::filesystem::create_hard_link(to, new_hard_link);
}
void create_hard_link(const Path &to, const Path &new_hard_link, Error_Code &ec) noexcept
{
    return std::filesystem::create_hard_link(to, new_hard_link, ec);
}

void create_symlink(const Path &to, const Path &new_symlink)
{
    return std::filesystem::create_symlink(to, new_symlink);
}
void create_symlink(const Path &to, const Path &new_symlink, Error_Code &ec) noexcept
{
    return std::filesystem::create_symlink(to, new_symlink, ec);
}

Path current_path()
{
    return std::filesystem::current_path();
}

Path current_path(Error_Code &ec)
{
    return std::filesystem::current_path(ec);
}

void current_path(const Path &p)
{
    return std::filesystem::current_path(p);
}

void current_path(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::current_path(p, ec);
}

bool equivalent(const Path &p1, const Path &p2)
{
    return std::filesystem::equivalent(p1, p2);
}
bool equivalent(const Path &p1, const Path &p2, Error_Code &ec) noexcept
{
    return std::filesystem::equivalent(p1, p2, ec);
}

bool exists(File_Status s) noexcept
{
    return std::filesystem::exists(s);
}
bool exists(const Path &p)
{
    return std::filesystem::exists(p);
}
bool exists(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::exists(p, ec);
}

uintmax_t file_size(const Path &p)
{
    return std::filesystem::file_size(p);
}
uintmax_t file_size(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::file_size(p, ec);
}

uintmax_t hard_link_count(const Path &p)
{
    return std::filesystem::hard_link_count(p);
}
uintmax_t hard_link_count(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::hard_link_count(p, ec);
}

bool is_block_file(File_Status s) noexcept
{
    return std::filesystem::is_block_file(s);
}
bool is_block_file(const Path &p)
{
    return std::filesystem::is_block_file(p);
}
bool is_block_file(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_block_file(p, ec);
}

bool is_character_file(File_Status s) noexcept
{
    return std::filesystem::is_character_file(s);
}
bool is_character_file(const Path &p)
{
    return std::filesystem::is_character_file(p);
}
bool is_character_file(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_character_file(p, ec);
}

bool is_directory(File_Status s) noexcept
{
    return std::filesystem::is_directory(s);
}
bool is_directory(const Path &p)
{
    return std::filesystem::is_directory(p);
}
bool is_directory(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_directory(p, ec);
}

bool is_empty(const Path &p)
{
    return std::filesystem::is_empty(p);
}
bool is_empty(const Path &p, Error_Code &ec)
{
    return std::filesystem::is_empty(p, ec);
}

bool is_fifo(File_Status s) noexcept
{
    return std::filesystem::is_fifo(s);
}
bool is_fifo(const Path &p)
{
    return std::filesystem::is_fifo(p);
}
bool is_fifo(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_fifo(p, ec);
}

bool is_other(File_Status s) noexcept
{
    return std::filesystem::is_other(s);
}
bool is_other(const Path &p)
{
    return std::filesystem::is_other(p);
}
bool is_other(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_other(p, ec);
}

bool is_regular_file(File_Status s) noexcept
{
    return std::filesystem::is_regular_file(s);
}
bool is_regular_file(const Path &p)
{
    return std::filesystem::is_regular_file(p);
}
bool is_regular_file(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_regular_file(p, ec);
}

bool is_socket(File_Status s) noexcept
{
    return std::filesystem::is_socket(s);
}
bool is_socket(const Path &p)
{
    return std::filesystem::is_socket(p);
}
bool is_socket(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_socket(p, ec);
}

bool is_symlink(File_Status s) noexcept
{
    return std::filesystem::is_symlink(s);
}
bool is_symlink(const Path &p)
{
    return std::filesystem::is_symlink(p);
}
bool is_symlink(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::is_symlink(p, ec);
}

File_Time_Type last_write_time(const Path &p)
{
    return std::filesystem::last_write_time(p);
}
File_Time_Type last_write_time(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::last_write_time(p, ec);
}
void last_write_time(const Path &p, File_Time_Type new_time)
{
    return std::filesystem::last_write_time(p, new_time);
}
void last_write_time(const Path &p, File_Time_Type new_time, Error_Code &ec) noexcept
{
    return std::filesystem::last_write_time(p, new_time, ec);
}

void permissions(const Path &p, Perms prms, Perm_Options opts)
{
    return std::filesystem::permissions(p, prms, opts);
}
void permissions(const Path &p, Perms prms, Error_Code &ec) noexcept
{
    return std::filesystem::permissions(p, prms, ec);
}
void permissions(const Path &p, Perms prms, Perm_Options opts, Error_Code &ec)
{
    return std::filesystem::permissions(p, prms, opts, ec);
}

Path proximate(const Path &p, Error_Code &ec)
{
    return std::filesystem::proximate(p, ec);
}
Path proximate(const Path &p, const Path &base)
{
    return std::filesystem::proximate(p, base);
}
Path proximate(const Path &p, const Path &base, Error_Code &ec)
{
    return std::filesystem::proximate(p, base, ec);
}

Path read_symlink(const Path &p)
{
    return std::filesystem::read_symlink(p);
}
Path read_symlink(const Path &p, Error_Code &ec)
{
    return std::filesystem::read_symlink(p, ec);
}

Path relative(const Path &p, Error_Code &ec)
{
    return std::filesystem::relative(p, ec);
}
Path relative(const Path &p, const Path &base)
{
    return std::filesystem::relative(p, base);
}
Path relative(const Path &p, const Path &base, Error_Code &ec)
{
    return std::filesystem::relative(p, base, ec);
}

bool remove(const Path &p)
{
    return std::filesystem::remove(p);
}
bool remove(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::remove(p, ec);
}

uintmax_t remove_all(const Path &p)
{
    return std::filesystem::remove_all(p);
}
uintmax_t remove_all(const Path &p, Error_Code &ec)
{
    return std::filesystem::remove_all(p, ec);
}

void rename(const Path &from, const Path &to)
{
    return std::filesystem::rename(from, to);
}
void rename(const Path &from, const Path &to, Error_Code &ec) noexcept
{
    return std::filesystem::rename(from, to, ec);
}

void resize_file(const Path &p, uintmax_t size)
{
    return std::filesystem::resize_file(p, size);
}
void resize_file(const Path &p, uintmax_t size, Error_Code &ec) noexcept
{
    return std::filesystem::resize_file(p, size, ec);
}

Space_Info space(const Path &p)
{
    return std::filesystem::space(p);
}
Space_Info space(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::space(p, ec);
}

File_Status status(const Path &p)
{
    return std::filesystem::status(p);
}
File_Status status(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::status(p, ec);
}

bool status_known(File_Status s) noexcept
{
    return std::filesystem::status_known(s);
}

File_Status symlink_status(const Path &p)
{
    return std::filesystem::symlink_status(p);
}
File_Status symlink_status(const Path &p, Error_Code &ec) noexcept
{
    return std::filesystem::symlink_status(p, ec);
}

Path temp_directory_path()
{
    return std::filesystem::temp_directory_path();
}
Path temp_directory_path(Error_Code &ec)
{
    return std::filesystem::temp_directory_path(ec);
}

Path weakly_canonical(const Path &p)
{
    return std::filesystem::weakly_canonical(p);
}
Path weakly_canonical(const Path &p, Error_Code &ec)
{
    return std::filesystem::weakly_canonical(p, ec);
}

} // namespace noble_steed::io::fs