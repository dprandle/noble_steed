#include <bx/bx.h>
#include <bx/spscqueue.h>
#include <bx/thread.h>
#include <bx/debug.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "window.h"

#include "../core/variant.h"
#include "../core/context.h"
#include "../core/context_obj.h"
#include "../core/string_hash.h"

namespace noble_steed
{

Window::Window()
{}

Window::~Window()
{}

dtup2 Window::get_normalized_cursor_pos()
{
    dtup2 ret = get_cursor_pos();
    itup2 scrn = get_size();
    ret.w /= scrn.w;
    ret.h /= scrn.h;
    return ret;
}

namespace init_param_key::window
{
/// String - title of window - defaults to "New Window"
const Str_Hash TITLE = "TITLE";

/// Bool - Should the window be fullscreen or not? If not specified will default to false
const Str_Hash FULLSCREEN = "FULLSCREEN";

/// itup2 - Size of window in screen coordinates.. if not provided then for fullscreen will be default video mode size, for windowed
/// it will be half of that.
const Str_Hash SIZE = "SIZE";

/// bool - Specify whether the windowed mode window will be visible on creation - ignored for fullscreen windows - default is true
const Str_Hash VISIBLE = "VISIBLE";

/// bool - Specify whether the windowed mode window should have border and normal close/title bar/etc - ignored for full screen windows - default is true
const Str_Hash DECORATED = "DECORATED";

/// bool - Maximize window on first creation - ignored for full screen windows - defualt is false
const Str_Hash MAXIMIZE = "MAXIMIZE";

/// bool - Always show this window on top of others - ignored for full screen windows - default is false
const Str_Hash ALWAS_ON_TOP = "ALWAS_ON_TOP";

/// bool - Auto iconify fullscreen window when it looses focus - ignored for windowed mode windows - default is true
const Str_Hash FULLSCREEN_AUTO_ICONIFY = "FULLSCREEN_AUTO_ICONIFY";

/// bool - Focus this window when first created - ignored for full screen and hidden windows - default is true
const Str_Hash FOCUSED = "FOCUSED";

/// bool - Center the cursor on fullscreen windows - ignored for windowed mode windows - default is true
const Str_Hash CENTER_CURSOR = "CENTER_CURSOR";

} // namespace init_param_key::window

namespace events::window
{
namespace closed
{
const Str_Hash id = "Window_Closed";
} // namespace closed

namespace resized
{
const Str_Hash id = "Window_Resized";
const Str_Hash new_size = "new_size";
} // namespace resized

namespace focus_change
{
const Str_Hash id = "Window_Focus_Change";
const Str_Hash focused = "focused";
const Str_Hash new_pos = "new_pos";
} // namespace focus_change

namespace iconified
{
const Str_Hash id = "Window_Iconified";
const Str_Hash iconified = "iconified";
} // namespace iconified

namespace maximized
{
const Str_Hash id = "Window_Maximized";
const Str_Hash maximized = "maximized";
} // namespace maximized

namespace moved
{
const Str_Hash id = "Window_Moved";
const Str_Hash new_pos = "new_pos";
} // namespace moved

namespace framebuffer_resized
{
const Str_Hash id = "Framebuffer_Resized";
const Str_Hash new_size = "new_size";
} // namespace framebuffer_resized

namespace key_press
{
const Str_Hash id = "Key_Press";
const Str_Hash key = "key";
const Str_Hash scancode = "scancode";
const Str_Hash action = "action";
const Str_Hash mods = "mods";
} // namespace key_press

namespace mouse_press
{
const Str_Hash id = "Mouse_Press";
const Str_Hash button = "button";
const Str_Hash action = "action";
const Str_Hash mods = "mods";
} // namespace mouse_press

namespace scroll
{
const Str_Hash id = "Scroll";
const Str_Hash x_offset = "x_offset"; // double
const Str_Hash y_offset = "y_offset"; // double
const Str_Hash mods = "mods";
} // namespace scroll

namespace cursor_moved
{
const Str_Hash id = "Cursor_Moved";
const Str_Hash mods = "mods";
const Str_Hash new_pos = "new_pos";
} // namespace cursor_moved

} // namespace events::window

} // namespace noble_steed