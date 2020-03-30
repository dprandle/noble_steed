#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include <noble_steed/serialization/json_archive.h>

namespace noble_steed
{
class Context_Obj
{    
    template<class T> friend class Pool_Factory;
    template<class T> friend class Free_List_Factory;

  public:
    Context_Obj();

    Context_Obj(const Context_Obj & copy);

    virtual ~Context_Obj();

    virtual void pack_unpack(JSON_Archive & ar);

    String to_json();

    void from_json(const String & json_str);

    bool is_owned_by_context();

    Context_Obj & operator=(Context_Obj rhs);

  protected:
    void swap(Context_Obj & rhs);

    virtual void pack_begin(JSON_Archive::Direction io_dir);

    virtual void pack_end(JSON_Archive::Direction io_dir);

    bool owned;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};
} // namespace noble_steed