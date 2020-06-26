#include <noble_steed/io/input_translator.h>
#include <noble_steed/core/context.h>
#include <noble_steed/core/application.h>
#include <noble_steed/graphics/window.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/io/input_map.h>
#include <noble_steed/core/engine.h>
#include <noble_steed/scene/world.h>

#include <GLFW/glfw3.h>

namespace noble_steed
{
namespace Events
{
namespace Key_Press
{
const u32 id = str_hash("Key_Press");
const String key = "key";
const String scancode = "scancode";
const String action = "action";
const String mods = "mods";
} // namespace Key_Press

namespace Mouse_Press
{
const u32 id = str_hash("Mouse_Press");
const String button = "button";
const String action = "action";
const String mods = "mods";
} // namespace Mouse_Press

namespace Scroll
{
const u32 id = str_hash("Scroll");
const String x_offset = "x_offset"; // double
const String y_offset = "y_offset"; // double
const String mods = "mods";
} // namespace Scroll

namespace Trigger
{
extern const u32 id = -1;
extern const String state = "state";
} // namespace Trigger

} // namespace Events

Input_Translator::Input_Translator()
{}

Input_Translator::~Input_Translator()
{}

void Input_Translator::initialize(const Variant_Map & init_params)
{
    System::initialize(init_params);
    glfwSetKeyCallback(app.get_window()->get_glfw_window(), glfw_key_press_callback);
    glfwSetMouseButtonCallback(app.get_window()->get_glfw_window(), glfw_mouse_button_callback);
    glfwSetScrollCallback(app.get_window()->get_glfw_window(), glfw_scroll_callback);

    subscribe_event_handler(Events::Key_Press::id, this, &Input_Translator::handle_key_press);
    subscribe_event_handler(Events::Mouse_Press::id, this, &Input_Translator::handle_mouse_press);
    subscribe_event_handler(Events::Scroll::id, this, &Input_Translator::handle_scroll);

    sig_connect(ns_eng->update, this, &Input_Translator::update);
}

void Input_Translator::handle_key_press(Event & ev)
{
    using namespace Events;
    Trigger_Condition tc;
    i8 action = ev.data[Key_Press::action].get_value<i8>();
    tc.input_code = ev.data[Key_Press::key].get_value<i16>();
    tc.modifier_mask = ev.data[Key_Press::mods].get_value<i16>();
    send_event_for_trigger_action_(tc,action);
}

void Input_Translator::send_event_for_trigger_action_(const Trigger_Condition & tc, i8 action, const Variant_Map & other_params)
{
    // Go through the context stack starting with the top.. send out the event for the first
    // matching one found and return
    for (int i = context_stack_.size() - 1; i >= 0; --i)
    {
        Input_Context * cur_ctxt = context_stack_[i];
        auto trigger_range = cur_ctxt->get_triggers(tc);
        auto range_iter = trigger_range.first;
        bool found_match = false;
        while (range_iter != trigger_range.second)
        {
            if (check_bit(range_iter->second.trigger_state, action))
            {
                Event ev;
                ev.id = range_iter->second.name_hash;
                ev.data[Events::Trigger::state] = action;
                ev.data.insert(other_params.begin(),other_params.end());
                post_event(ev);
                found_match = true;
            }
            ++range_iter;
        }
        if (found_match)
            return;
    }
}

void Input_Translator::handle_mouse_press(Event & ev)
{
    using namespace Events;
    Trigger_Condition tc;
    i8 action = ev.data[Mouse_Press::action].get_value<i8>();
    tc.input_code = ev.data[Mouse_Press::button].get_value<i16>();
    tc.modifier_mask = ev.data[Mouse_Press::mods].get_value<i16>();
    send_event_for_trigger_action_(tc,action);
}

void Input_Translator::handle_scroll(Event & ev)
{
    using namespace Events;
    Trigger_Condition tc;
    i8 action = T_MOUSE_MOVE_OR_SCROLL;
    tc.input_code = MOUSE_SCROLL;
    tc.modifier_mask = ev.data[Scroll::mods].get_value<i16>();
    Variant_Map params;
    params[Scroll::x_offset] = ev.data[Scroll::x_offset];
    params[Scroll::y_offset] = ev.data[Scroll::y_offset];
    send_event_for_trigger_action_(tc,action, params);
}

void Input_Translator::terminate()
{
    System::terminate();
}

void Input_Translator::update()
{
    process_events();
}

bool Input_Translator::_trigger_already_active(Input_Action_Trigger * trig)
{
    return false;
}

void Input_Translator::push_context(Input_Context * ctxt)
{
    context_stack_.push_back(ctxt);
}

void Input_Translator::pop_context()
{
    context_stack_.pop_back();
}

void Input_Translator::glfw_key_press_callback(GLFWwindow * window, i32 key, i32 scancode, i32 action, i32 mods)
{
    using namespace Events;
    Event ev;
    ev.id = Key_Press::id;
    ev.data[Key_Press::key] = i16(key);
    ev.data[Key_Press::scancode] = i16(scancode);
    if (action == GLFW_PRESS)
        ev.data[Key_Press::action] = i8(Trigger_State::T_PRESS);
    else if (action == GLFW_RELEASE)
        ev.data[Key_Press::action] = i8(Trigger_State::T_RELEASE);
    ev.data[Key_Press::mods] = i16(mods);
    post_event(ev);
}

void Input_Translator::glfw_mouse_button_callback(GLFWwindow * window, i32 button, i32 action, i32 mods)
{
    using namespace Events;
    Event ev;
    ev.id = Mouse_Press::id;
    ev.data[Mouse_Press::button] = i16(button);
    if (action == GLFW_PRESS)
        ev.data[Mouse_Press::action] = i8(Trigger_State::T_PRESS);
    else if (action == GLFW_RELEASE)
        ev.data[Mouse_Press::action] = i8(Trigger_State::T_RELEASE);
    ev.data[Mouse_Press::mods] = i16(mods);
    post_event(ev);
}

void Input_Translator::glfw_scroll_callback(GLFWwindow * window, double x_offset, double y_offset)
{
    using namespace Events;
    Event ev;
    ev.id = Scroll::id;
    ev.data[Scroll::x_offset] = x_offset;
    ev.data[Scroll::y_offset] = y_offset;
    i16 mods;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        mods |= MOD_SHIFT;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        mods |= MOD_CONTROL;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS)
        mods |= MOD_SUPER;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
        mods |= MOD_ALT;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        mods |= MOD_CAPS_LOCK;
    if (glfwGetKey(window, GLFW_KEY_NUM_LOCK) == GLFW_PRESS)
        mods |= MOD_NUM_LOCK;
    ev.data[Scroll::mods] = mods;
    post_event(ev);
}

/* The unknown key */
const i16 KEY_UNKNOWN = GLFW_KEY_UNKNOWN;

/* Printable keys */
const i16 KEY_SPACE = GLFW_KEY_SPACE;
const i16 KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
const i16 KEY_COMMA = GLFW_KEY_COMMA;
const i16 KEY_MINUS = GLFW_KEY_MINUS;
const i16 KEY_PERIOD = GLFW_KEY_PERIOD;
const i16 KEY_SLASH = GLFW_KEY_SLASH;
const i16 KEY_0 = GLFW_KEY_0;
const i16 KEY_1 = GLFW_KEY_1;
const i16 KEY_2 = GLFW_KEY_2;
const i16 KEY_3 = GLFW_KEY_3;
const i16 KEY_4 = GLFW_KEY_4;
const i16 KEY_5 = GLFW_KEY_5;
const i16 KEY_6 = GLFW_KEY_6;
const i16 KEY_7 = GLFW_KEY_7;
const i16 KEY_8 = GLFW_KEY_8;
const i16 KEY_9 = GLFW_KEY_9;
const i16 KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
const i16 KEY_EQUAL = GLFW_KEY_EQUAL;
const i16 KEY_A = GLFW_KEY_A;
const i16 KEY_B = GLFW_KEY_B;
const i16 KEY_C = GLFW_KEY_C;
const i16 KEY_D = GLFW_KEY_D;
const i16 KEY_E = GLFW_KEY_E;
const i16 KEY_F = GLFW_KEY_F;
const i16 KEY_G = GLFW_KEY_G;
const i16 KEY_H = GLFW_KEY_H;
const i16 KEY_I = GLFW_KEY_I;
const i16 KEY_J = GLFW_KEY_J;
const i16 KEY_K = GLFW_KEY_K;
const i16 KEY_L = GLFW_KEY_L;
const i16 KEY_M = GLFW_KEY_M;
const i16 KEY_N = GLFW_KEY_N;
const i16 KEY_O = GLFW_KEY_O;
const i16 KEY_P = GLFW_KEY_P;
const i16 KEY_Q = GLFW_KEY_Q;
const i16 KEY_R = GLFW_KEY_R;
const i16 KEY_S = GLFW_KEY_S;
const i16 KEY_T = GLFW_KEY_T;
const i16 KEY_U = GLFW_KEY_U;
const i16 KEY_V = GLFW_KEY_V;
const i16 KEY_W = GLFW_KEY_W;
const i16 KEY_X = GLFW_KEY_X;
const i16 KEY_Y = GLFW_KEY_Y;
const i16 KEY_Z = GLFW_KEY_Z;
const i16 KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
const i16 KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
const i16 KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
const i16 KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
const i16 KEY_WORLD_1 = GLFW_KEY_WORLD_1;
const i16 KEY_WORLD_2 = GLFW_KEY_WORLD_2;

/* Function keys */
const i16 KEY_ESCAPE = GLFW_KEY_ESCAPE;
const i16 KEY_ENTER = GLFW_KEY_ENTER;
const i16 KEY_TAB = GLFW_KEY_TAB;
const i16 KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
const i16 KEY_INSERT = GLFW_KEY_INSERT;
const i16 KEY_DELETE = GLFW_KEY_DELETE;
const i16 KEY_RIGHT = GLFW_KEY_RIGHT;
const i16 KEY_LEFT = GLFW_KEY_LEFT;
const i16 KEY_DOWN = GLFW_KEY_DOWN;
const i16 KEY_UP = GLFW_KEY_UP;
const i16 KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
const i16 KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
const i16 KEY_HOME = GLFW_KEY_HOME;
const i16 KEY_END = GLFW_KEY_END;
const i16 KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
const i16 KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
const i16 KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
const i16 KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
const i16 KEY_PAUSE = GLFW_KEY_PAUSE;
const i16 KEY_F1 = GLFW_KEY_F1;
const i16 KEY_F2 = GLFW_KEY_F2;
const i16 KEY_F3 = GLFW_KEY_F3;
const i16 KEY_F4 = GLFW_KEY_F4;
const i16 KEY_F5 = GLFW_KEY_F5;
const i16 KEY_F6 = GLFW_KEY_F6;
const i16 KEY_F7 = GLFW_KEY_F7;
const i16 KEY_F8 = GLFW_KEY_F8;
const i16 KEY_F9 = GLFW_KEY_F9;
const i16 KEY_F10 = GLFW_KEY_F10;
const i16 KEY_F11 = GLFW_KEY_F11;
const i16 KEY_F12 = GLFW_KEY_F12;
const i16 KEY_F13 = GLFW_KEY_F13;
const i16 KEY_F14 = GLFW_KEY_F14;
const i16 KEY_F15 = GLFW_KEY_F15;
const i16 KEY_F16 = GLFW_KEY_F16;
const i16 KEY_F17 = GLFW_KEY_F17;
const i16 KEY_F18 = GLFW_KEY_F18;
const i16 KEY_F19 = GLFW_KEY_F19;
const i16 KEY_F20 = GLFW_KEY_F20;
const i16 KEY_F21 = GLFW_KEY_F21;
const i16 KEY_F22 = GLFW_KEY_F22;
const i16 KEY_F23 = GLFW_KEY_F23;
const i16 KEY_F24 = GLFW_KEY_F24;
const i16 KEY_F25 = GLFW_KEY_F25;
const i16 KEY_KP_0 = GLFW_KEY_KP_0;
const i16 KEY_KP_1 = GLFW_KEY_KP_1;
const i16 KEY_KP_2 = GLFW_KEY_KP_2;
const i16 KEY_KP_3 = GLFW_KEY_KP_3;
const i16 KEY_KP_4 = GLFW_KEY_KP_4;
const i16 KEY_KP_5 = GLFW_KEY_KP_5;
const i16 KEY_KP_6 = GLFW_KEY_KP_6;
const i16 KEY_KP_7 = GLFW_KEY_KP_7;
const i16 KEY_KP_8 = GLFW_KEY_KP_8;
const i16 KEY_KP_9 = GLFW_KEY_KP_9;
const i16 KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
const i16 KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
const i16 KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
const i16 KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
const i16 KEY_KP_ADD = GLFW_KEY_KP_ADD;
const i16 KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
const i16 KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
const i16 KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const i16 KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
const i16 KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
const i16 KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
const i16 KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const i16 KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
const i16 KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
const i16 KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
const i16 KEY_MENU = GLFW_KEY_MENU;

const i16 MOD_SHIFT = GLFW_MOD_SHIFT;
const i16 MOD_CONTROL = GLFW_MOD_CONTROL;
const i16 MOD_ALT = GLFW_MOD_ALT;
const i16 MOD_SUPER = GLFW_MOD_SUPER;
const i16 MOD_CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
const i16 MOD_NUM_LOCK = GLFW_MOD_NUM_LOCK;

const i16 MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
const i16 MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
const i16 MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
const i16 MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
const i16 MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
const i16 MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
const i16 MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
const i16 MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;
const i16 MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST;
const i16 MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
const i16 MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
const i16 MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;

const i16 MOUSE_SCROLL = 8;
const i16 MOUSE_MOVEMENT = 9;
} // namespace noble_steed
