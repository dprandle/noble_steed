#pragma once

#include "../core/variant.h"

namespace noble_steed
{

class Window
{
  public:
    Window();
    virtual ~Window();

    virtual bool initialize(const Variant_Map &init_params) = 0;

    virtual void terminate() = 0;

    virtual void *get_native_window() = 0;

    virtual void *get_native_display() = 0;

    virtual itup2 get_size() = 0;

    virtual itup2 get_framebuffer_size() = 0;

    virtual dtup2 get_cursor_pos() = 0;

    dtup2 get_normalized_cursor_pos();
};

namespace init_param_key::window
{
/// String - title of window - defaults to "New Window"
extern const Str_Hash TITLE;

/// Bool - Should the window be fullscreen or not? If not specified will default to false
extern const Str_Hash FULLSCREEN;

/// itup2 - Size of window in screen coordinates.. if not provided then for fullscreen will be default video mode size, for windowed
/// it will be half of that.
extern const Str_Hash SIZE;

/// bool - Specify whether the windowed mode window will be visible on creation - ignored for fullscreen windows - default is true
extern const Str_Hash VISIBLE;

/// bool - Specify whether the windowed mode window should have border and normal close/title bar/etc - ignored for full screen windows - default is true
extern const Str_Hash DECORATED;

/// bool - Maximize window on first creation - ignored for full screen windows - defualt is false
extern const Str_Hash MAXIMIZE;

/// bool - Always show this window on top of others - ignored for full screen windows - default is false
extern const Str_Hash ALWAS_ON_TOP;

/// bool - Auto iconify fullscreen window when it looses focus - ignored for windowed mode windows - default is true
extern const Str_Hash FULLSCREEN_AUTO_ICONIFY;

/// bool - Focus this window when first created - ignored for full screen and hidden windows - default is true
extern const Str_Hash FOCUSED;

/// bool - Center the cursor on fullscreen windows - ignored for windowed mode windows - default is true
extern const Str_Hash CENTER_CURSOR;

} // namespace init_param_key::window

namespace events::window
{
namespace closed
{
extern const Str_Hash id;
} // namespace closed

/// This event is posted whenever the window is resized
namespace resized
{
extern const Str_Hash id;
/// itup2 - new screen size in screen coordinates
extern const Str_Hash new_size;
} // namespace resized

/// This event is posted whenever the focus is gained or lost on the main window
namespace focus_change
{
extern const Str_Hash id;
/// i8 - 1 for window focused and 0 for not
extern const Str_Hash focused;
/// dtup2 - new cursor position in screen coordinates
extern const Str_Hash new_pos;
} // namespace focus_change

/// This event is posted whenever the window is iconified or restored from iconified. Iconified basically means minimized.
namespace iconified
{
extern const Str_Hash id;
/// i8 - 1 for window iconified and 0 for not
extern const Str_Hash iconified;
} // namespace iconified

/// This event is posted whenever the window is maximized or restored from maximized
namespace maximized
{
extern const Str_Hash id;
/// i8 - 1 for window maximized and 0 for not
extern const Str_Hash maximized;
} // namespace maximized

/// This event is posted whenever the window is moved - this event is never posted in full screen mode
namespace moved
{
extern const Str_Hash id;
/// itup2 - new window position - upper left corner in screen coordinates
extern const Str_Hash new_pos;
} // namespace moved

/// This event is posted whenever the framebuffer is resized (the main window default framebuffer)
namespace framebuffer_resized
{
extern const Str_Hash id;
/// itup2 - new framebuffer size in pixels
extern const Str_Hash new_size;
} // namespace framebuffer_resized

namespace key_press
{
extern const Str_Hash id;
/// i16 - key code (KEY_BLA)
extern const Str_Hash key;
/// i16 - os specific scancode
extern const Str_Hash scancode;
/// i8 - action is either Trigget_State::T_PRESS or Trigger_State::T_RELEASE
extern const Str_Hash action;
/// i16 - bitmask of all keyboard modifiers
extern const Str_Hash mods;
} // namespace key_press

namespace mouse_press
{
extern const Str_Hash id;
/// i16 - mouse button code (MOUSE_BUTTON_BLA)
extern const Str_Hash button;
/// i8 - action is either Trigget_State::T_PRESS or Trigger_State::T_RELEASE
extern const Str_Hash action;
/// i16 - bitmask of all keyboard modifiers
extern const Str_Hash mods;
} // namespace mouse_press

namespace scroll
{
extern const Str_Hash id;
/// i16 - bitmask of all keyboard modifiers
extern const Str_Hash mods;
/// double - scroll amount in x direction - this will be zero in normal cases
extern const Str_Hash x_offset;
/// double - scroll amount in vertical - this is the var of interest
extern const Str_Hash y_offset;
} // namespace scroll

/// This event is posted whenever the cursor is moved with the window having focus.
namespace cursor_moved
{
extern const Str_Hash id;
/// i16 - bitmask of all keyboard modifiers
extern const Str_Hash mods;
/// dtup2 - new cursor position in screen coordinates
extern const Str_Hash new_pos;
} // namespace cursor_moved

} // namespace events::window

} // namespace noble_steed