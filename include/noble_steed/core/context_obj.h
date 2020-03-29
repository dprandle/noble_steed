#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>

namespace noble_steed
{
class Context_Obj
{
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
  public:
    Context_Obj();

    virtual ~Context_Obj();

    virtual void pack_unpack(JSON_Archive & ar);

    String to_json();

    void from_json(const String & json_str);

  protected:
    virtual void pack_begin(JSON_Archive::Direction io_dir);

    virtual void pack_end(JSON_Archive::Direction io_dir);
};
} // namespace noble_steed