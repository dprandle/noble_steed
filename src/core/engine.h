#pragma once

#include <chrono>

#include "system.h"

extern const std::chrono::nanoseconds TIMESTEP;

namespace noble_steed
{
class Engine : public System
{
  public:
    Engine();

    Engine(const Engine & copy);

    Engine & operator=(Engine rhs);

    virtual ~Engine();

    void run_frame();

    std::chrono::nanoseconds elapsed();

    virtual void initialize(const Variant_Map & init_params);

    virtual void terminate();

    bool is_running();
    void set_running(bool running);

    Signal<> frame_start;
    Signal<> update;
    Signal<> post_update;
    Signal<> render;
    Signal<> post_render;
    Signal<> frame_finish;

  protected:
    void swap(const Engine & rhs);
    bool running_;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    std::chrono::time_point<std::chrono::high_resolution_clock> cur_time_;
    std::chrono::nanoseconds dt_;
    std::chrono::nanoseconds lag_;
};

} // namespace noble_steed