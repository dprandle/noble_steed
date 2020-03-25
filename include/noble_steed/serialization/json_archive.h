#pragma once

#include <noble_steed/serialization/archive.h>
#include <rttr/type>

namespace noble_steed
{
struct JSON_Archive : public Archive
{
    String json_str;
};

#define JSON_PACKABLE                                                                                                                                \
  public:                                                                                                                                            \
    inline virtual void pack_unpack(JSON_Archive & ar)                                                                                               \
    {                                                                                                                                                \
        noble_steed::pack_unpack(ar, *this);                                                                                                                    \
    }                                                                                                                                                \
    inline String to_json()                                                                                                                          \
    {                                                                                                                                                \
        JSON_Archive ar;                                                                                                                             \
        ar.io_dir = Archive::DIR_OUT;                                                                                                                \
        pack_unpack(ar);                                                                                                                             \
        return ar.json_str;                                                                                                                          \
    }                                                                                                                                                \
    inline void from_json(const String & json_str)                                                                                                   \
    {                                                                                                                                                \
        JSON_Archive ar;                                                                                                                             \
        ar.json_str = json_str;                                                                                                                      \
        ar.io_dir = Archive::DIR_IN;                                                                                                                 \
        pack_unpack(ar);                                                                                                                             \
    }                                                                                                                                                \
  private:

void pack_unpack(JSON_Archive & ar, rttr::instance obj);

class JSON_Packable
{
    RTTR_ENABLE()
    JSON_PACKABLE
};

} // namespace noble_steed