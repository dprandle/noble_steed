#pragma once
#include <experimental/filesystem>
#include <fstream>
#include <sstream>

#include "../container/string.h"
#include "../container/vector.h"

namespace std
{
namespace experimental
{
namespace filesystem
{
/// Read all contents from file fname in to vector buffer
bool read_file_to_buffer(const noble_steed::String & fname, noble_steed::i8_Vector & buffer);

/// Read all contents from fname in to read_into as text
bool read_file_to_string(const noble_steed::String & fname, noble_steed::String & read_into);

/// Write string to fname as text
bool write_string_to_file(const noble_steed::String & fname, const noble_steed::String & string);

/// This function forks a new executable with path/name exec_name, and args exec_args, and saves the command line output to prog_output
/// This function auto adds the self name to the first arg
void fork_and_wait(const noble_steed::String & exec_name, const noble_steed::Vector<noble_steed::String> & exec_args, noble_steed::String & prog_output);

noble_steed::String executable_name();
}
} // namespace experimental
} // namespace std

namespace fs = std::experimental::filesystem;
