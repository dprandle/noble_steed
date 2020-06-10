#pragma once

#include <noble_steed/core/system.h>
#include <chrono>

extern const std::chrono::nanoseconds TIMESTEP;

#define ns_eng ns_ctxt.get_world()->get_system<Engine>()

namespace noble_steed
{
class Engine : public System
{
    RTTR_ENABLE(System)
  public:
    Engine();

    Engine(const Engine & copy);

    Engine & operator=(Engine rhs);

    virtual ~Engine();

    void run_frame();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

    Signal<> frame_start;
    Signal<> update;
    Signal<> post_update;
    Signal<> render;
    Signal<> post_render;
    Signal<> frame_finish;

  protected:
    void swap(const Engine & rhs);

    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::chrono::time_point<std::chrono::high_resolution_clock> cur_time_;
    std::chrono::nanoseconds dt_;
    std::chrono::nanoseconds lag_;
};

} // namespace noble_steed