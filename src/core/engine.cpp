#include <noble_steed/core/engine.h>

const std::chrono::nanoseconds TIMESTEP(10000000); // 10 milliseconds

namespace noble_steed
{
Engine::Engine()
{}

Engine::Engine(const Engine & copy) : System(copy), start_(), cur_time_(),dt_(), lag_()
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
    auto this_frame = std::chrono::high_resolution_clock::now();
    dt_ = this_frame - cur_time_;
    lag_ += dt_;
    while (lag_ >= TIMESTEP)
    {
        lag_ -= TIMESTEP;
        emit_sig update();
    }
}

void Engine::initialize(const Variant_Map & init_params)
{
    System::initialize(init_params);
    start_ = std::chrono::high_resolution_clock::now();
    cur_time_ = start_;
    lag_ = std::chrono::nanoseconds::zero();
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