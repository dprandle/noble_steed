#include <bgfx/bgfx.h>
#include <string.h>

#include "platform.h"
#include "build_and_battle.h"
#include "log.h"
#include "keycodes.h"
#include "filesystem"

namespace noble_steed
{

static bool s_showStats = false;

intern void process_events(bb_state *bstate)
{
    bb_event *ev = back(&bstate->events);
    while (ev)
    {
        if (ev->type == event_type::INPUT_KEY_PRESS && ev->kpress.key == imap::key::F11)
        {
            ilog("Got key press event for F11");
            s_showStats = true;
        }
        else if (ev->type == event_type::INPUT_KEY_RELEASE && ev->kpress.key == imap::key::F11)
        {
            ilog("Got key release event for F11");
            s_showStats = false;
        }

        auto ev_str = event_string(*ev);
        ilog("Processing %s", ev_str.buf);
        pop_back(&bstate->events);
        ev = back(&bstate->events);
    }
}

small_str event_string(const bb_event &ev)
{
    small_str ret;
    switch (ev.type)
    {
    case (event_type::WINDOW_RESIZE):
        copy_str(&ret, "WindowResize");
        break;
    case (event_type::INPUT_KEY_PRESS):
        copy_str(&ret, "InputKeyPress");
        break;
    case (event_type::INPUT_KEY_RELEASE):
        copy_str(&ret, "InputKeyRelease");
        break;
    default:
        copy_str(&ret, "Unknown");
    }
    return ret;
}

void bb_init(bb_state *bstate)
{
    path_str pstr;
    fixed_str<64> err_str;
    copy_str(&pstr.str, "data/shaders/simple.fbin");
    vector<u8> data{};
    resize(&data, 24);

    auto result = platform_read_file(pstr.str.data, &data, 300);
    if (error_occured(result))
    {
        error_to_string(result, &err_str.str);
        ilog("Error code: %s: %s", err_str.str.data, strerror(result.err.code));
    }
    else
    {
        result = platform_write_file("data/shaders/simple_test_copy.fbin", true, data, 0);

        if (error_occured(result))
        {
            error_to_string(result, &err_str.str);
            ilog("Error code: %s: %s", err_str.str.data, strerror(result.err.code));
        }
    }

    auto ret = platform_write_file("test.txt", false, "test", 4, -1);
    if (error_occured(ret))
    {
        error_to_string(ret, &err_str.str);
        ilog("Error code: %s: %s", err_str.str.data, strerror(ret.err.code));
    }
}

void bb_update_and_render(bb_state *bstate)
{
    process_events(bstate);

    bgfx::touch(0);
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
    const bgfx::Stats *stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0,
                        2,
                        0x0f,
                        "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.",
                        stats->width,
                        stats->height,
                        stats->textWidth,
                        stats->textHeight);
    // Enable stats or debug text.
    bgfx::setDebug(s_showStats ? BGFX_DEBUG_TEXT : BGFX_DEBUG_STATS);
    bgfx::frame();
}

void bb_term(bb_state *bstate)
{
}
} // namespace noble_steed