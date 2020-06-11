#include <noble_steed/core/engine.h>
#include <noble_steed/core/context.h>
#include <noble_steed/graphics/window.h>
#include <noble_steed/io/json_archive.h>

const std::chrono::nanoseconds TIMESTEP(10000000); // 10 milliseconds

namespace noble_steed
{
Engine::Engine() : running_(false)
{}

Engine::Engine(const Engine & copy) : System(copy), start_(), cur_time_(), dt_(), lag_()
{}

Engine & Engine::operator=(Engine rhs)
{
    swap(rhs);
    return *this;
}

Engine::~Engine()
{}

void Engine::run_frame()
{
    emit_sig frame_start();

    process_events();

    auto this_frame = std::chrono::high_resolution_clock::now();
    dt_ = this_frame - cur_time_;
    lag_ += dt_;
    while (lag_ >= TIMESTEP)
    {
        lag_ -= TIMESTEP;
        emit_sig update();
        emit_sig post_update();
    }
    emit_sig render();
    emit_sig post_render();

    emit_sig frame_finish();
}

bool Engine::is_running()
{
    return running_;
}

void Engine::set_running(bool running)
{
    running_ = running;
}

void Engine::initialize(const Variant_Map & init_params)
{
    System::initialize(init_params);
    start_ = std::chrono::high_resolution_clock::now();
    cur_time_ = start_;
    lag_ = std::chrono::nanoseconds::zero();

    itup2 g;

    using namespace Events;
    subscribe_event_handler(Window_Closed::id, [=](Event & ev) { set_running(false); });

    subscribe_event_handler(Window_Focus_Change::id, [=](Event & ev) {
        dlog("Window is now {}!", (ev.data[Window_Focus_Change::focused].get_value<u32>()) ? "focused" : "unfocused");
    });
    subscribe_event_handler(Window_Iconified::id, [=](Event & ev) {
        dlog("Window is now {}!", (ev.data[Window_Iconified::iconified].get_value<u32>()) ? "iconified" : "restored");
    });
    subscribe_event_handler(Window_Maximized::id, [=](Event & ev) {
        dlog("Window is now {}!", (ev.data[Window_Maximized::maximized].get_value<u32>()) ? "maximized" : "restored");
    });
    subscribe_event_handler(Window_Resized::id,
                            [=](Event & ev) { dlog("Window resized to {}", ev.data[Window_Resized::new_size]); });
    subscribe_event_handler(Window_Moved::id, [=](Event & ev) { dlog("Window moved to {}", ev.data[Window_Moved::new_pos]); });
    subscribe_event_handler(Framebuffer_Resized::id,
                            [=](Event & ev) { dlog("Framebuffer resized to {}", ev.data[Framebuffer_Resized::new_size]); });
}

void Engine::terminate()
{
    System::terminate();
}

void Engine::swap(const Engine & rhs)
{
    System::swap(rhs);
}

} // namespace noble_steed