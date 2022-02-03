#pragma once

#include "../core/resource.h"
#include "../container/hash_map.h"
#include "../container/tuple.h"

namespace noble_steed
{
enum Trigger_State
{
    T_PRESS = 1,
    T_RELEASE = 2,
    T_MOUSE_MOVE_OR_SCROLL = 4
};

union Trigger_Condition
{
    Trigger_Condition():lookup_key(0) {}

    struct
    {
        // Mouse button or key code
        i16 input_code;
        // Modifiers required
        i16 modifier_mask;
    };
    // Union of input_code and modifier mask - use this as the key for Input_Context lookups
    i32 lookup_key;

    bool operator==(const Trigger_Condition & rhs) const
    {
        return (lookup_key == rhs.lookup_key);
    }
};

struct Input_Action_Trigger
{
    Input_Action_Trigger(const Str_Hash & name, const Trigger_Condition & tcond = Trigger_Condition(), Trigger_State tstate = T_PRESS);
    Input_Action_Trigger(const Input_Action_Trigger &) = default;

    Str_Hash name_hash;
    i8 trigger_state;
    Trigger_Condition condition; // Here for convenience
    bool operator==(const Input_Action_Trigger & rhs) const;
};

using Trigger_Map = hash_multimap<i32, Input_Action_Trigger>;
using Trigger_Range = Tuple2<Trigger_Map::iterator>;

struct Input_Context
{
    bool add_trigger(const Input_Action_Trigger & trigger);

    bool remove_trigger(const Input_Action_Trigger & trigger);

    u32 remove_triggers(const Trigger_Condition & cond);

    Trigger_Range get_triggers(const Trigger_Condition & cond);

    Trigger_Map t_map_;
};

using Context_Map = Str_Hash_Map<Input_Context>;

class Input_Map : public Resource
{
  public:
    Input_Map();

    Input_Map(const Input_Map & copy_);

    ~Input_Map();

    Input_Map & operator=(Input_Map rhs);

    Input_Context * add_context(const Str_Hash & name, const Input_Context & to_add);

    Input_Context * get_context(const Str_Hash & name);

    bool remove_context(const Str_Hash & name);

    bool rename_context(const Str_Hash & old_name, const Str_Hash & new_name);

  private:
    Context_Map contexts_;
};

namespace imap::key
{
/* The unknown key */
extern const i16 UNKNOWN;

/* Printable keys */
extern const i16 SPACE;
extern const i16 APOSTROPHE;
extern const i16 COMMA;
extern const i16 MINUS;
extern const i16 PERIOD;
extern const i16 SLASH;
extern const i16 N0;
extern const i16 N1;
extern const i16 N2;
extern const i16 N3;
extern const i16 N4;
extern const i16 N5;
extern const i16 N6;
extern const i16 N7;
extern const i16 N8;
extern const i16 N9;
extern const i16 SEMICOLON;
extern const i16 EQUAL;
extern const i16 A;
extern const i16 B;
extern const i16 C;
extern const i16 D;
extern const i16 E;
extern const i16 F;
extern const i16 G;
extern const i16 H;
extern const i16 I;
extern const i16 J;
extern const i16 K;
extern const i16 L;
extern const i16 M;
extern const i16 N;
extern const i16 O;
extern const i16 P;
extern const i16 Q;
extern const i16 R;
extern const i16 S;
extern const i16 T;
extern const i16 U;
extern const i16 V;
extern const i16 W;
extern const i16 X;
extern const i16 Y;
extern const i16 Z;
extern const i16 LEFT_BRACKET;
extern const i16 BACKSLASH;
extern const i16 RIGHT_BRACKET;
extern const i16 GRAVE_ACCENT;
extern const i16 WORLD_1;
extern const i16 WORLD_2;

/* Function keys */
extern const i16 ESCAPE;
extern const i16 ENTER;
extern const i16 TAB;
extern const i16 BACKSPACE;
extern const i16 INSERT;
extern const i16 DELETE;
extern const i16 RIGHT;
extern const i16 LEFT;
extern const i16 DOWN;
extern const i16 UP;
extern const i16 PAGE_UP;
extern const i16 PAGE_DOWN;
extern const i16 HOME;
extern const i16 END;
extern const i16 CAPS_LOCK;
extern const i16 SCROLL_LOCK;
extern const i16 NUM_LOCK;
extern const i16 PRINT_SCREEN;
extern const i16 PAUSE;
extern const i16 F1;
extern const i16 F2;
extern const i16 F3;
extern const i16 F4;
extern const i16 F5;
extern const i16 F6;
extern const i16 F7;
extern const i16 F8;
extern const i16 F9;
extern const i16 F10;
extern const i16 F11;
extern const i16 F12;
extern const i16 F13;
extern const i16 F14;
extern const i16 F15;
extern const i16 F16;
extern const i16 F17;
extern const i16 F18;
extern const i16 F19;
extern const i16 F20;
extern const i16 F21;
extern const i16 F22;
extern const i16 F23;
extern const i16 F24;
extern const i16 F25;
extern const i16 KP_0;
extern const i16 KP_1;
extern const i16 KP_2;
extern const i16 KP_3;
extern const i16 KP_4;
extern const i16 KP_5;
extern const i16 KP_6;
extern const i16 KP_7;
extern const i16 KP_8;
extern const i16 KP_9;
extern const i16 KP_DECIMAL;
extern const i16 KP_DIVIDE;
extern const i16 KP_MULTIPLY;
extern const i16 KP_SUBTRACT;
extern const i16 KP_ADD;
extern const i16 KP_ENTER;
extern const i16 KP_EQUAL;
extern const i16 LEFT_SHIFT;
extern const i16 LEFT_CONTROL;
extern const i16 LEFT_ALT;
extern const i16 LEFT_SUPER;
extern const i16 RIGHT_SHIFT;
extern const i16 RIGHT_CONTROL;
extern const i16 RIGHT_ALT;
extern const i16 RIGHT_SUPER;
extern const i16 MENU;
}

// Keyboard modifiers
namespace imap::mod
{
extern const i16 SHIFT;
extern const i16 CONTROL;
extern const i16 ALT;
extern const i16 SUPER;
extern const i16 CAPS_LOCK;
extern const i16 NUM_LOCK;
extern const i16 MOUSE_LEFT;
extern const i16 MOUSE_RIGHT;
extern const i16 MOUSE_MIDDLE;
extern const i16 NONE;
}

// Mouse buttons
namespace imap::mouse
{
extern const i16 BTN_1;
extern const i16 BTN_2;
extern const i16 BTN_3;
extern const i16 BTN_4;
extern const i16 BTN_5;
extern const i16 BTN_6;
extern const i16 BTN_7;
extern const i16 BTN_8;
extern const i16 BTN_LAST;
extern const i16 BTN_LEFT;
extern const i16 BTN_RIGHT;
extern const i16 BTN_MIDDLE;
extern const i16 SCROLL;
extern const i16 MOVEMENT;
}

} // namespace noble_steed