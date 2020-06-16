#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/container/vector.h>

namespace noble_steed
{
enum Trigger_State
{
    T_PRESS = 1,
    T_RELEASE = 2
};

union Trigger_Condition
{
    struct
    {
        // Mouse button or key code
        i16 input_code;
        // Modifiers required
        i16 modifier_mask;
    };
    // Union of input_code and modifier mask - use this as the key for Input_Context lookups
    i32 lookup_key;

    bool operator==(const Trigger_Condition & rhs)
    {
        return (lookup_key == rhs.lookup_key);
    }
};

struct Input_Action_Trigger
{
    //
    u32 name_hash;
    i8 trigger_state;
    Trigger_Condition condition; // Here for convenience
    bool operator==(const Input_Action_Trigger & rhs);
};

using Trigger_Map = Hash_Multimap<i32, Input_Action_Trigger>;
using Trigger_Range = Tuple2<Trigger_Map::iterator>;

struct Input_Context
{
    bool add_trigger(const Input_Action_Trigger & trigger);

    bool remove_trigger(const Input_Action_Trigger & trigger);

    u32 remove_triggers(const Trigger_Condition & cond);

    Trigger_Range get_triggers(const Trigger_Condition & cond);

    Trigger_Map t_map_;
};

using Context_Map = Hash_Map<u32, Input_Context>;

class Input_Map
{
  public:
    Input_Map();

    Input_Map(const Input_Map & copy_);

    ~Input_Map();

    Input_Map & operator=(Input_Map rhs);

    bool add_context(const String & name, const Input_Context & to_add);

    Input_Context * get_context(const String & name);

    bool remove_context(const String & name);

    bool rename_context(const String & old_name, const String & new_name);

  private:
    Context_Map contexts_;
};

} // namespace noble_steed