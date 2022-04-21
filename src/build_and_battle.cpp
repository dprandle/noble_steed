#include "bgfx/bgfx.h"
#include "build_and_battle.h"
#include "log.h"
#include "keycodes.h"
#include "filesystem"

namespace noble_steed
{

struct test_item
{
    char scoob;
    double doob;
};

bool operator==(const test_item &lhs, const test_item &rhs)
{
    return ((lhs.scoob == rhs.scoob) && math::fequals(lhs.doob, rhs.doob));
}

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
    ilog("Try to load shader");
    vector<test_item> buf;
    emplace_back(&buf, test_item{'c', 4.6});
    emplace_back(&buf, test_item{'d', 7.6});
    emplace_back(&buf, test_item{'a', 4.6});
    emplace_back(&buf, test_item{'e', 3.6});
    emplace_back(&buf, test_item{'f', 2.6});
    emplace_back(&buf, test_item{'g', 1.6});

    resize(&buf, 15, {'z', 5.5});

    resize(&buf, 4);

    auto fiter = find(&buf, test_item{'d',7.6});
    if (fiter != buf.end())
        fiter = erase(&buf, fiter);
    clear(&buf);

    ilog("Fiter: ");
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
{}
} // namespace noble_steed