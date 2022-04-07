#pragma once

#include "nsstring.h"

namespace noble_steed
{
using path_str = fixed_str<256>;

/// Read all contents from file fname in to vector buffer
// bool read_file_to_buffer(const path_str &fname, u8 *buffer);

/// Read all contents from fname in to read_into as text
// bool read_file_to_string(const path_str &fname, string *read_into);

/// Write string to fname as text
// bool write_string_to_file(const path_str &fname, const string &str);

//const string &executable_name();

} // namespace noble_steed::io::fs