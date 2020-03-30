#pragma once
//#include <experimental/filesystem>
#include <filesystem>
#include <noble_steed/container/string.h>
#include <fstream>
#include <sstream>

namespace std
{
namespace __fs
{
namespace filesystem
{

noble_steed::String executable_name();

} // namespace filesystem
} // namespace __fs
} // namespace std

namespace fs = std::__fs::filesystem;