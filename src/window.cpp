#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "window.h"

namespace noble_steed
{

namespace imap::key
{
/* The unknown key */
const i16 UNKNOWN = GLFW_KEY_UNKNOWN;

/* Printable */
const i16 SPACE = GLFW_KEY_SPACE;
const i16 APOSTROPHE = GLFW_KEY_APOSTROPHE;
const i16 COMMA = GLFW_KEY_COMMA;
const i16 MINUS = GLFW_KEY_MINUS;
const i16 PERIOD = GLFW_KEY_PERIOD;
const i16 SLASH = GLFW_KEY_SLASH;
const i16 N0 = GLFW_KEY_0;
const i16 N1 = GLFW_KEY_1;
const i16 N2 = GLFW_KEY_2;
const i16 N3 = GLFW_KEY_3;
const i16 N4 = GLFW_KEY_4;
const i16 N5 = GLFW_KEY_5;
const i16 N6 = GLFW_KEY_6;
const i16 N7 = GLFW_KEY_7;
const i16 N8 = GLFW_KEY_8;
const i16 N9 = GLFW_KEY_9;
const i16 SEMICOLON = GLFW_KEY_SEMICOLON;
const i16 EQUAL = GLFW_KEY_EQUAL;
const i16 A = GLFW_KEY_A;
const i16 B = GLFW_KEY_B;
const i16 C = GLFW_KEY_C;
const i16 D = GLFW_KEY_D;
const i16 E = GLFW_KEY_E;
const i16 F = GLFW_KEY_F;
const i16 G = GLFW_KEY_G;
const i16 H = GLFW_KEY_H;
const i16 I = GLFW_KEY_I;
const i16 J = GLFW_KEY_J;
const i16 K = GLFW_KEY_K;
const i16 L = GLFW_KEY_L;
const i16 M = GLFW_KEY_M;
const i16 N = GLFW_KEY_N;
const i16 O = GLFW_KEY_O;
const i16 P = GLFW_KEY_P;
const i16 Q = GLFW_KEY_Q;
const i16 R = GLFW_KEY_R;
const i16 S = GLFW_KEY_S;
const i16 T = GLFW_KEY_T;
const i16 U = GLFW_KEY_U;
const i16 V = GLFW_KEY_V;
const i16 W = GLFW_KEY_W;
const i16 X = GLFW_KEY_X;
const i16 Y = GLFW_KEY_Y;
const i16 Z = GLFW_KEY_Z;
const i16 LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
const i16 BACKSLASH = GLFW_KEY_BACKSLASH;
const i16 RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
const i16 GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
const i16 WORLD_1 = GLFW_KEY_WORLD_1;
const i16 WORLD_2 = GLFW_KEY_WORLD_2;

/* */
const i16 ESCAPE = GLFW_KEY_ESCAPE;
const i16 ENTER = GLFW_KEY_ENTER;
const i16 TAB = GLFW_KEY_TAB;
const i16 BACKSPACE = GLFW_KEY_BACKSPACE;
const i16 INSERT = GLFW_KEY_INSERT;
const i16 DELETE = GLFW_KEY_DELETE;
const i16 RIGHT = GLFW_KEY_RIGHT;
const i16 LEFT = GLFW_KEY_LEFT;
const i16 DOWN = GLFW_KEY_DOWN;
const i16 UP = GLFW_KEY_UP;
const i16 PAGE_UP = GLFW_KEY_PAGE_UP;
const i16 PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
const i16 HOME = GLFW_KEY_HOME;
const i16 END = GLFW_KEY_END;
const i16 CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
const i16 SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
const i16 NUM_LOCK = GLFW_KEY_NUM_LOCK;
const i16 PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
const i16 PAUSE = GLFW_KEY_PAUSE;
const i16 F1 = GLFW_KEY_F1;
const i16 F2 = GLFW_KEY_F2;
const i16 F3 = GLFW_KEY_F3;
const i16 F4 = GLFW_KEY_F4;
const i16 F5 = GLFW_KEY_F5;
const i16 F6 = GLFW_KEY_F6;
const i16 F7 = GLFW_KEY_F7;
const i16 F8 = GLFW_KEY_F8;
const i16 F9 = GLFW_KEY_F9;
const i16 F10 = GLFW_KEY_F10;
const i16 F11 = GLFW_KEY_F11;
const i16 F12 = GLFW_KEY_F12;
const i16 F13 = GLFW_KEY_F13;
const i16 F14 = GLFW_KEY_F14;
const i16 F15 = GLFW_KEY_F15;
const i16 F16 = GLFW_KEY_F16;
const i16 F17 = GLFW_KEY_F17;
const i16 F18 = GLFW_KEY_F18;
const i16 F19 = GLFW_KEY_F19;
const i16 F20 = GLFW_KEY_F20;
const i16 F21 = GLFW_KEY_F21;
const i16 F22 = GLFW_KEY_F22;
const i16 F23 = GLFW_KEY_F23;
const i16 F24 = GLFW_KEY_F24;
const i16 F25 = GLFW_KEY_F25;
const i16 KP_0 = GLFW_KEY_KP_0;
const i16 KP_1 = GLFW_KEY_KP_1;
const i16 KP_2 = GLFW_KEY_KP_2;
const i16 KP_3 = GLFW_KEY_KP_3;
const i16 KP_4 = GLFW_KEY_KP_4;
const i16 KP_5 = GLFW_KEY_KP_5;
const i16 KP_6 = GLFW_KEY_KP_6;
const i16 KP_7 = GLFW_KEY_KP_7;
const i16 KP_8 = GLFW_KEY_KP_8;
const i16 KP_9 = GLFW_KEY_KP_9;
const i16 KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
const i16 KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
const i16 KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
const i16 KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
const i16 KP_ADD = GLFW_KEY_KP_ADD;
const i16 KP_ENTER = GLFW_KEY_KP_ENTER;
const i16 KP_EQUAL = GLFW_KEY_KP_EQUAL;
const i16 LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
const i16 LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
const i16 LEFT_ALT = GLFW_KEY_LEFT_ALT;
const i16 LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
const i16 RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
const i16 RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
const i16 RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
const i16 RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
const i16 MENU = GLFW_KEY_MENU;
} // namespace imap::key

namespace imap::mod
{
const i16 SHIFT = GLFW_MOD_SHIFT;
const i16 CONTROL = GLFW_MOD_CONTROL;
const i16 ALT = GLFW_MOD_ALT;
const i16 SUPER = GLFW_MOD_SUPER;
const i16 CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
const i16 NUM_LOCK = GLFW_MOD_NUM_LOCK;
const i16 MOUSE_LEFT = 0x0040;
const i16 MOUSE_RIGHT = 0x0080;
const i16 MOUSE_MIDDLE = 0x0100;
const i16 NONE = 0;
} // namespace imap::mod

namespace imap::mouse
{
const i16 BTN_1 = GLFW_MOUSE_BUTTON_1;
const i16 BTN_2 = GLFW_MOUSE_BUTTON_2;
const i16 BTN_3 = GLFW_MOUSE_BUTTON_3;
const i16 BTN_4 = GLFW_MOUSE_BUTTON_4;
const i16 BTN_5 = GLFW_MOUSE_BUTTON_5;
const i16 BTN_6 = GLFW_MOUSE_BUTTON_6;
const i16 BTN_7 = GLFW_MOUSE_BUTTON_7;
const i16 BTN_8 = GLFW_MOUSE_BUTTON_8;
const i16 BTN_LAST = GLFW_MOUSE_BUTTON_LAST;
const i16 BTN_LEFT = GLFW_MOUSE_BUTTON_LEFT;
const i16 BTN_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
const i16 BTN_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;
const i16 SCROLL = 8;
const i16 MOVEMENT = 9;
} // namespace imap::mouse

void glfw_error_callback(i32 error, const char *description)
{
    // elog("GLFW Error {}",description);
}

void glfw_key_press_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods)
{
    // using namespace events::window;
    // using namespace imap::mod;

    // Event ev;
    // ev.id = key_press::id;
    // ev.data[key_press::key] = i16(key);
    // ev.data[key_press::scancode] = i16(scancode);
    // if (action == GLFW_PRESS)
    //     ev.data[key_press::action] = i8(Trigger_State::T_PRESS);
    // else if (action == GLFW_RELEASE)
    //     ev.data[key_press::action] = i8(Trigger_State::T_RELEASE);

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    //     mods |= MOUSE_LEFT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    //     mods |= MOUSE_RIGHT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    //     mods |= MOUSE_MIDDLE;

    // ev.data[key_press::mods] = i16(mods);
    // post_event(ev);
}

void glfw_mouse_button_callback(GLFWwindow *window, i32 button, i32 action, i32 mods)
{
    // using namespace events::window;
    // using namespace imap::mod;
    // Event ev;
    // ev.id = mouse_press::id;
    // ev.data[mouse_press::button] = i16(button);
    // if (action == GLFW_PRESS)
    //     ev.data[mouse_press::action] = i8(Trigger_State::T_PRESS);
    // else if (action == GLFW_RELEASE)
    //     ev.data[mouse_press::action] = i8(Trigger_State::T_RELEASE);

    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && button != GLFW_MOUSE_BUTTON_LEFT)
    //     mods |= MOUSE_LEFT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && button != GLFW_MOUSE_BUTTON_RIGHT)
    //     mods |= MOUSE_RIGHT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS && button != GLFW_MOUSE_BUTTON_MIDDLE)
    //     mods |= MOUSE_MIDDLE;

    // ev.data[mouse_press::mods] = i16(mods);
    // post_event(ev);
}

void glfw_scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    // using namespace events::window;
    // using namespace imap::mod;
    // Event ev;
    // ev.id = scroll::id;
    // ev.data[scroll::x_offset] = x_offset;
    // ev.data[scroll::y_offset] = y_offset;
    // i16 mods = 0;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    //     mods |= SHIFT;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    //     mods |= CONTROL;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS)
    //     mods |= SUPER;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    //     mods |= ALT;
    // if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    //     mods |= CAPS_LOCK;
    // if (glfwGetKey(window, GLFW_KEY_NUM_LOCK) == GLFW_PRESS)
    //     mods |= NUM_LOCK;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    //     mods |= MOUSE_LEFT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    //     mods |= MOUSE_RIGHT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    //     mods |= MOUSE_MIDDLE;
    // ev.data[scroll::mods] = mods;
    // post_event(ev);
}

void glfw_cursor_pos_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    // using namespace events::window;
    // using namespace imap::mod;
    // Event ev;
    // ev.id = cursor_moved::id;
    // ev.data[cursor_moved::new_pos] = dtup2(x_pos, y_pos);
    // i16 mods = 0;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    //     mods |= SHIFT;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
    //     mods |= CONTROL;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS)
    //     mods |= SUPER;
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
    //     mods |= ALT;
    // if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    //     mods |= CAPS_LOCK;
    // if (glfwGetKey(window, GLFW_KEY_NUM_LOCK) == GLFW_PRESS)
    //     mods |= NUM_LOCK;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    //     mods |= MOUSE_LEFT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    //     mods |= MOUSE_RIGHT;
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    //     mods |= MOUSE_MIDDLE;
    // ev.data[scroll::mods] = mods;
    // post_event(ev);
}

void glfw_resize_window_callback(GLFWwindow *window, i32 width, i32 height)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::resized::id;
    // ev.data[window::resized::new_size] = itup2(width, height);
    // post_event(ev);
}

void glfw_focus_change_callback(GLFWwindow *window, i32 focused)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::focus_change::id;
    // ev.data[window::sdfsfocus_change::focused] = i8(focused);
    // dtup2 pos;
    // glfwGetCursorPos(window, &pos.x, &pos.y);
    // ev.data[window::focus_change::new_pos] = pos;
    // post_event(ev);
}

void glfw_close_window_callback(GLFWwindow *window)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::closed::id;
    // post_event(ev);
}

void glfw_iconify_window_callback(GLFWwindow *window, i32 iconified)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::iconified::id;
    // ev.data[window::iconified::iconified] = i8(iconified);
    // post_event(ev);
}

void glfw_maximize_window_callback(GLFWwindow *window, i32 maximized)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::maximized::id;
    // ev.data[window::maximized::maximized] = i8(maximized);
    // post_event(ev);
}

void glfw_window_position_callback(GLFWwindow *window, i32 x_pos, i32 y_pos)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::moved::id;
    // ev.data[window::moved::new_pos] = itup2(x_pos, y_pos);
    // post_event(ev);
}

void glfw_franebuffer_resized_callback(GLFWwindow *window, i32 width, i32 height)
{
    // using namespace events;
    // Event ev;
    // ev.id = window::framebuffer_resized::id;
    // ev.data[window::framebuffer_resized::new_size] = itup2(width, height);
    // post_event(ev);
}

window create_window(const window_create_params &param)
{
    window ret = {};
    ret.type = window_type::glfw;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return ret;

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    bool fullsreen = false;
    if (test_flags(param.win_flags, window_flags::FULLSCREEN))
    {
        glfwWindowHint(GLFW_AUTO_ICONIFY, (int)test_flags(param.win_flags, window_flags::FULLSCREEN_AUTO_ICONIFTY));
        glfwWindowHint(GLFW_CENTER_CURSOR, (int)test_flags(param.win_flags, window_flags::FULLSCREEN_CENTER_CURSOR));
    }
    else
    {
        glfwWindowHint(GLFW_VISIBLE, (int)test_flags(param.win_flags, window_flags::VISIBLE));
        glfwWindowHint(GLFW_DECORATED, (int)test_flags(param.win_flags, window_flags::DECORATED));
        glfwWindowHint(GLFW_MAXIMIZED, (int)test_flags(param.win_flags, window_flags::MAXIMIZE));
        glfwWindowHint(GLFW_FLOATING, (int)test_flags(param.win_flags, window_flags::ALWAYS_ON_TOP));
        monitor = nullptr;
    }
    ret.wglfw.hndl = glfwCreateWindow(param.resolution.x, param.resolution.y, param.title.buf, monitor, nullptr);

    if (!ret.wglfw.hndl)
        return ret;

    glfwSetWindowSizeCallback(ret.wglfw.hndl, glfw_resize_window_callback);
    glfwSetWindowCloseCallback(ret.wglfw.hndl, glfw_close_window_callback);
    glfwSetWindowMaximizeCallback(ret.wglfw.hndl, glfw_maximize_window_callback);
    glfwSetWindowIconifyCallback(ret.wglfw.hndl, glfw_iconify_window_callback);
    glfwSetWindowPosCallback(ret.wglfw.hndl, glfw_window_position_callback);
    glfwSetWindowFocusCallback(ret.wglfw.hndl, glfw_focus_change_callback);
    glfwSetFramebufferSizeCallback(ret.wglfw.hndl, glfw_franebuffer_resized_callback);
    glfwSetInputMode(ret.wglfw.hndl, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
    glfwSetKeyCallback(ret.wglfw.hndl, glfw_key_press_callback);
    glfwSetMouseButtonCallback(ret.wglfw.hndl, glfw_mouse_button_callback);
    glfwSetScrollCallback(ret.wglfw.hndl, glfw_scroll_callback);
    glfwSetCursorPosCallback(ret.wglfw.hndl, glfw_cursor_pos_callback);
    return ret;
}

// bool GLFW_Window::initialize(const Variant_Map &init_params)
// {
// }

// GLFWwindow *GLFW_Window::get_glfw_window()
// {
//     return window_;
// }

// void *GLFW_Window::get_native_display()
// {
// #ifdef PLATFORM_OSX
//     return nullptr;
// #elif PLATFORM_LINUX
//     return glfwGetX11Display();
// #endif
//     return nullptr;
// }

// void GLFW_Window::terminate()
// {
//     glfwDestroyWindow(window_);
//     glfwTerminate();
// }

// itup2 GLFW_Window::get_size()
// {
//     itup2 ret;
//     glfwGetWindowSize(window_, &ret.w, &ret.h);
//     return ret;
// }

// itup2 GLFW_Window::get_framebuffer_size()
// {
//     itup2 ret;
//     glfwGetFramebufferSize(window_, &ret.w, &ret.h);
//     return ret;
// }

// dtup2 GLFW_Window::get_cursor_pos()
// {
//     dtup2 ret;
//     glfwGetCursorPos(window_, &ret.w, &ret.h);
//     return ret;
// }

// void *GLFW_Window::get_native_window()
// {
// #ifdef PLATFORM_OSX
//     return glfwGetCocoaWindow(window_);
// #elif PLATFORM_LINUX
//     return reinterpret_cast<void *>(glfwGetX11Window(window_));
// #endif
//     return nullptr;
// }

// Window::Window()
// {}

// Window::~Window()
// {}

// dtup2 Window::get_normalized_cursor_pos()
// {
//     dtup2 ret = get_cursor_pos();
//     itup2 scrn = get_size();
//     ret.w /= scrn.w;
//     ret.h /= scrn.h;
//     return ret;
// }
} // namespace noble_steed