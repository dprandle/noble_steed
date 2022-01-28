#pragma once

#include <memory_resource>
#include "../core/basic_types.h"

namespace noble_steed
{
using Mem_Resource_Base = std::pmr::memory_resource;
Mem_Resource_Base * get_default_resource();

class Allocator : public Mem_Resource_Base
{
  public:
    Allocator(sizet total_size, Mem_Resource_Base * upstream);
    Allocator(const Allocator &copy) = delete;
    virtual ~Allocator();

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