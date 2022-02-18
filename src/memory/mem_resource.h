#pragma once

#ifdef PLATFORM_OSX
#include <experimental/memory_resource>
#else
#include <memory_resource>
#endif
#include "../core/basic_types.h"

namespace noble_steed
{
#ifdef PLATFORM_OSX
using Mem_Resource_Base = std::experimental::pmr::memory_resource;
#else
using Mem_Resource_Base = std::pmr::memory_resource;
#endif

Mem_Resource_Base * get_default_resource();



class Mem_Resource : public Mem_Resource_Base
{
  public:
    Mem_Resource(sizet total_size, Mem_Resource_Base * upstream);
    Mem_Resource(const Mem_Resource &copy) = delete;
    virtual ~Mem_Resource();

    void reset();

    Mem_Resource_Base * upstream();

  protected:
    virtual void do_reset() = 0;

    bool do_is_equal(const Mem_Resource_Base &rhs) const noexcept override;

    sizet _total_size;
    sizet _used;
    sizet _peak;
    Mem_Resource_Base * _upstream;
    void * _start_ptr;
};
} // namespace noble_steed