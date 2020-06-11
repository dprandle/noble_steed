#pragma once

#include <noble_steed/core/common.h>
#include <noble_steed/container/vector.h>
#include <noble_steed/core/system.h>

struct GLFWwindow;

namespace noble_steed
{
struct Input_Context;
struct Input_Action_Trigger;

namespace Events
{
namespace Key_Press
{
extern const int id;
extern const String key;      // i32
extern const String scancode; // i32
extern const String action;   // i32
extern const String mods;     // i32
} // namespace Key_Press

namespace Mouse_Press
{
extern const int id;
extern const String button; // i32
extern const String action; // i32
extern const String mods;   // i32
} // namespace Mouse_Press

namespace Scroll
{
extern const int id;
extern const String x_offset; // double
extern const String y_offset; // double
} // namespace Scroll

} // namespace Events

// Keep track of current modifier state etc?
class Input_Translator : public System
{
  public:
    Input_Translator();
    ~Input_Translator();

    void initialize(const Variant_Map & init_params);

    void terminate();

    void push_context(Input_Context * ctxt);
    void pop_context();

    void update();

  private:
    bool _trigger_already_active(Input_Action_Trigger * trig);

    void handle_key_press(Event & ev);

    void handle_mouse_press(Event & ev);

    void handle_scroll(Event & ev);

    static void glfw_key_press_callback(GLFWwindow * window, i32 key, i32 scancode, i32 action, i32 mods);
    static void glfw_mouse_button_callback(GLFWwindow * window, i32 button, i32 action, i32 mods);
    static void glfw_scroll_callback(GLFWwindow * window, double x_offset, double y_offset);

    Vector<Input_Context *> context_stack_;
    Vector<Input_Action_Trigger *> active_triggers_;

    RTTR_ENABLE(System)
};

/* The unknown key */
extern const i16 KEY_UNKNOWN;

/* Printable keys */
extern const i16 KEY_SPACE;
extern const i16 KEY_APOSTROPHE;
extern const i16 KEY_COMMA;
extern const i16 KEY_MINUS;
extern const i16 KEY_PERIOD;
extern const i16 KEY_SLASH;
extern const i16 KEY_0;
extern const i16 KEY_1;
extern const i16 KEY_2;
extern const i16 KEY_3;
extern const i16 KEY_4;
extern const i16 KEY_5;
extern const i16 KEY_6;
extern const i16 KEY_7;
extern const i16 KEY_8;
extern const i16 KEY_9;
extern const i16 KEY_SEMICOLON;
extern const i16 KEY_EQUAL;
extern const i16 KEY_A;
extern const i16 KEY_B;
extern const i16 KEY_C;
extern const i16 KEY_D;
extern const i16 KEY_E;
extern const i16 KEY_F;
extern const i16 KEY_G;
extern const i16 KEY_H;
extern const i16 KEY_I;
extern const i16 KEY_J;
extern const i16 KEY_K;
extern const i16 KEY_L;
extern const i16 KEY_M;
extern const i16 KEY_N;
extern const i16 KEY_O;
extern const i16 KEY_P;
extern const i16 KEY_Q;
extern const i16 KEY_R;
extern const i16 KEY_S;
extern const i16 KEY_T;
extern const i16 KEY_U;
extern const i16 KEY_V;
extern const i16 KEY_W;
extern const i16 KEY_X;
extern const i16 KEY_Y;
extern const i16 KEY_Z;
extern const i16 KEY_LEFT_BRACKET;
extern const i16 KEY_BACKSLASH;
extern const i16 KEY_RIGHT_BRACKET;
extern const i16 KEY_GRAVE_ACCENT;
extern const i16 KEY_WORLD_1;
extern const i16 KEY_WORLD_2;

/* Function keys */
extern const i16 KEY_ESCAPE;
extern const i16 KEY_ENTER;
extern const i16 KEY_TAB;
extern const i16 KEY_BACKSPACE;
extern const i16 KEY_INSERT;
extern const i16 KEY_DELETE;
extern const i16 KEY_RIGHT;
extern const i16 KEY_LEFT;
extern const i16 KEY_DOWN;
extern const i16 KEY_UP;
extern const i16 KEY_PAGE_UP;
extern const i16 KEY_PAGE_DOWN;
extern const i16 KEY_HOME;
extern const i16 KEY_END;
extern const i16 KEY_CAPS_LOCK;
extern const i16 KEY_SCROLL_LOCK;
extern const i16 KEY_NUM_LOCK;
extern const i16 KEY_PRINT_SCREEN;
extern const i16 KEY_PAUSE;
extern const i16 KEY_F1;
extern const i16 KEY_F2;
extern const i16 KEY_F3;
extern const i16 KEY_F4;
extern const i16 KEY_F5;
extern const i16 KEY_F6;
extern const i16 KEY_F7;
extern const i16 KEY_F8;
extern const i16 KEY_F9;
extern const i16 KEY_F10;
extern const i16 KEY_F11;
extern const i16 KEY_F12;
extern const i16 KEY_F13;
extern const i16 KEY_F14;
extern const i16 KEY_F15;
extern const i16 KEY_F16;
extern const i16 KEY_F17;
extern const i16 KEY_F18;
extern const i16 KEY_F19;
extern const i16 KEY_F20;
extern const i16 KEY_F21;
extern const i16 KEY_F22;
extern const i16 KEY_F23;
extern const i16 KEY_F24;
extern const i16 KEY_F25;
extern const i16 KEY_KP_0;
extern const i16 KEY_KP_1;
extern const i16 KEY_KP_2;
extern const i16 KEY_KP_3;
extern const i16 KEY_KP_4;
extern const i16 KEY_KP_5;
extern const i16 KEY_KP_6;
extern const i16 KEY_KP_7;
extern const i16 KEY_KP_8;
extern const i16 KEY_KP_9;
extern const i16 KEY_KP_DECIMAL;
extern const i16 KEY_KP_DIVIDE;
extern const i16 KEY_KP_MULTIPLY;
extern const i16 KEY_KP_SUBTRACT;
extern const i16 KEY_KP_ADD;
extern const i16 KEY_KP_ENTER;
extern const i16 KEY_KP_EQUAL;
extern const i16 KEY_LEFT_SHIFT;
extern const i16 KEY_LEFT_CONTROL;
extern const i16 KEY_LEFT_ALT;
extern const i16 KEY_LEFT_SUPER;
extern const i16 KEY_RIGHT_SHIFT;
extern const i16 KEY_RIGHT_CONTROL;
extern const i16 KEY_RIGHT_ALT;
extern const i16 KEY_RIGHT_SUPER;
extern const i16 KEY_MENU;

// Keyboard modifiers
extern const i16 MOD_SHIFT;
extern const i16 MOD_CONTROL;
extern const i16 MOD_ALT;
extern const i16 MOD_SUPER;
extern const i16 MOD_CAPS_LOCK;
extern const i16 MOD_NUM_LOCK;

// Mouse buttons
extern const i16 MOUSE_BUTTON_1;
extern const i16 MOUSE_BUTTON_2;
extern const i16 MOUSE_BUTTON_3;
extern const i16 MOUSE_BUTTON_4;
extern const i16 MOUSE_BUTTON_5;
extern const i16 MOUSE_BUTTON_6;
extern const i16 MOUSE_BUTTON_7;
extern const i16 MOUSE_BUTTON_8;
extern const i16 MOUSE_BUTTON_LAST;
extern const i16 MOUSE_BUTTON_LEFT;
extern const i16 MOUSE_BUTTON_RIGHT;
extern const i16 MOUSE_BUTTON_MIDDLE;

} // namespace noble_steed