#pragma once
#include <experimental/filesystem>
//#include <filesystem>
#include <noble_steed/container/string.h>
#include <fstream>
#include <sstream>

namespace std
{
namespace experimental
{
namespace filesystem
{
bool read_file_to_string(const noble_steed::String & fname, noble_steed::String & read_into);
bool write_string_to_file(const noble_steed::String & fname, const noble_steed::String & string);
noble_steed::String executable_name();
}
} // namespace experimental
} // namespace std

namespace fs = std::experimental::filesystem;
