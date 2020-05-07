#pragma once
//#include <experimental/filesystem>
#include <filesystem>
#include <noble_steed/container/string.h>
#include <fstream>
#include <sstream>

namespace noble_steed
{
noble_steed::String executable_name();
}

namespace fs = std::filesystem;