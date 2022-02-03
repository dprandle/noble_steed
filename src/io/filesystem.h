#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../container/string.h"
#include "../container/vector.h"

namespace noble_steed::fs
{
using Path = std::filesystem::path;
using Error_Code = std::error_code;
using Dir_Entry = std::filesystem::directory_entry;
using Dir_Iter = std::filesystem::directory_iterator;
using Recursive_Dir_Iter = std::filesystem::recursive_directory_iterator;
using Copy_Opts = std::filesystem::copy_options;
using File_Status = std::filesystem::file_status;
using File_Time_Type = std::filesystem::file_time_type;
using Perms = std::filesystem::perms;
using Perm_Options = std::filesystem::perm_options;
using Space_Info = std::filesystem::space_info;

/// Read all contents from file fname in to vector buffer
bool read_file_to_buffer(const String &fname, i8_vector &buffer);

/// Read all contents from fname in to read_into as text
bool read_file_to_string(const String &fname, String &read_into);

/// Write string to fname as text
bool write_string_to_file(const String &fname, const String &string);

/// This function forks a new executable with Path/name exec_name, and args exec_args, and saves the command line output to prog_output
/// This function auto adds the self name to the first arg
void fork_and_wait(const String &exec_name, const vector<String> &exec_args, String &prog_output);

Path absolute(const Path &p);
Path absolute(const Path &p, Error_Code &ec);

Path canonical(const Path &p);
Path canonical(const Path &p, Error_Code &ec);

void copy(const Path &from, const Path &to);
void copy(const Path &from, const Path &to, Error_Code &ec);
void copy(const Path &from, const Path &to, Copy_Opts options);
void copy(const Path &from, const Path &to, Copy_Opts options, Error_Code &ec);

bool copy_file(const Path &from, const Path &to);
bool copy_file(const Path &from, const Path &to, Error_Code &ec);
bool copy_file(const Path &from, const Path &to, Copy_Opts option);
bool copy_file(const Path &from, const Path &to, Copy_Opts option, Error_Code &ec);

void copy_symlink(const Path &existing_symlink, const Path &new_symlink);
void copy_symlink(const Path &existing_symlink, const Path &new_symlink, Error_Code &ec) noexcept;

bool create_directories(const Path &p);
bool create_directories(const Path &p, Error_Code &ec);

bool create_directory(const Path &p);
bool create_directory(const Path &p, Error_Code &ec) noexcept;

bool create_directory(const Path &p, const Path &attributes);
bool create_directory(const Path &p, const Path &attributes, Error_Code &ec) noexcept;

void create_directory_symlink(const Path &to, const Path &new_symlink);
void create_directory_symlink(const Path &to, const Path &new_symlink, Error_Code &ec) noexcept;

void create_hard_link(const Path &to, const Path &new_hard_link);
void create_hard_link(const Path &to, const Path &new_hard_link, Error_Code &ec) noexcept;

void create_symlink(const Path &to, const Path &new_symlink);
void create_symlink(const Path &to, const Path &new_symlink, Error_Code &ec) noexcept;

Path current_path();
Path current_path(Error_Code &ec);
void current_path(const Path &p);
void current_path(const Path &p, Error_Code &ec) noexcept;

bool equivalent(const Path &p1, const Path &p2);
bool equivalent(const Path &p1, const Path &p2, Error_Code &ec) noexcept;

bool exists(File_Status s) noexcept;
bool exists(const Path &p);
bool exists(const Path &p, Error_Code &ec) noexcept;

uintmax_t file_size(const Path &p);
uintmax_t file_size(const Path &p, Error_Code &ec) noexcept;

uintmax_t hard_link_count(const Path &p);
uintmax_t hard_link_count(const Path &p, Error_Code &ec) noexcept;

bool is_block_file(File_Status s) noexcept;
bool is_block_file(const Path &p);
bool is_block_file(const Path &p, Error_Code &ec) noexcept;

bool is_character_file(File_Status s) noexcept;
bool is_character_file(const Path &p);
bool is_character_file(const Path &p, Error_Code &ec) noexcept;

bool is_directory(File_Status s) noexcept;
bool is_directory(const Path &p);
bool is_directory(const Path &p, Error_Code &ec) noexcept;

bool is_empty(const Path &p);
bool is_empty(const Path &p, Error_Code &ec);

bool is_fifo(File_Status s) noexcept;
bool is_fifo(const Path &p);
bool is_fifo(const Path &p, Error_Code &ec) noexcept;

bool is_other(File_Status s) noexcept;
bool is_other(const Path &p);
bool is_other(const Path &p, Error_Code &ec) noexcept;

bool is_regular_file(File_Status s) noexcept;
bool is_regular_file(const Path &p);
bool is_regular_file(const Path &p, Error_Code &ec) noexcept;

bool is_socket(File_Status s) noexcept;
bool is_socket(const Path &p);
bool is_socket(const Path &p, Error_Code &ec) noexcept;

bool is_symlink(File_Status s) noexcept;
bool is_symlink(const Path &p);
bool is_symlink(const Path &p, Error_Code &ec) noexcept;

File_Time_Type last_write_time(const Path &p);
File_Time_Type last_write_time(const Path &p, Error_Code &ec) noexcept;
void last_write_time(const Path &p, File_Time_Type new_time);
void last_write_time(const Path &p, File_Time_Type new_time, Error_Code &ec) noexcept;

void permissions(const Path &p, Perms prms, Perm_Options opts = Perm_Options::replace);
void permissions(const Path &p, Perms prms, Error_Code &ec) noexcept;
void permissions(const Path &p, Perms prms, Perm_Options opts, Error_Code &ec);

Path proximate(const Path &p, Error_Code &ec);
Path proximate(const Path &p, const Path &base = current_path());
Path proximate(const Path &p, const Path &base, Error_Code &ec);

Path read_symlink(const Path &p);
Path read_symlink(const Path &p, Error_Code &ec);

Path relative(const Path &p, Error_Code &ec);
Path relative(const Path &p, const Path &base = current_path());
Path relative(const Path &p, const Path &base, Error_Code &ec);

bool remove(const Path &p);
bool remove(const Path &p, Error_Code &ec) noexcept;

uintmax_t remove_all(const Path &p);
uintmax_t remove_all(const Path &p, Error_Code &ec);

void rename(const Path &from, const Path &to);
void rename(const Path &from, const Path &to, Error_Code &ec) noexcept;

void resize_file(const Path &p, uintmax_t size);
void resize_file(const Path &p, uintmax_t size, Error_Code &ec) noexcept;

Space_Info space(const Path &p);
Space_Info space(const Path &p, Error_Code &ec) noexcept;

File_Status status(const Path &p);
File_Status status(const Path &p, Error_Code &ec) noexcept;

bool status_known(File_Status s) noexcept;

File_Status symlink_status(const Path &p);
File_Status symlink_status(const Path &p, Error_Code &ec) noexcept;

Path temp_directory_path();
Path temp_directory_path(Error_Code &ec);

Path weakly_canonical(const Path &p);
Path weakly_canonical(const Path &p, Error_Code &ec);

String executable_name();

} // namespace noble_steed::io::fs