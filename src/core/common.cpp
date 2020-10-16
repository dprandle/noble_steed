#include <noble_steed/core/common.h>
#include <rttr/registration.h>
#include <noble_steed/io/json_archive.h>

std::ostream & operator<<(std::ostream & lhs, const rttr::variant & rhs)
{
    noble_steed::JSON_Archive ja;
    ja.io_dir = noble_steed::Archive::DIR_OUT;
    pack_unpack(ja, rhs);
    return lhs << ja.json_str;
}