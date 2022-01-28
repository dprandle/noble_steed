#pragma once

#include "allocator.h"

namespace noble_steed
{

class Linear_Allocator : public Allocator
{
  public:
    Linear_Allocator(sizet total_size, Mem_Resource_Base *upstream);
    Linear_Allocator(sizet total_size);
    ~Linear_Allocator();

  private:
    void do_reset() override;

    void *do_allocate(sizet size, sizet alignment = 0) override;

    /// NO op for this type of allocator - all memory released on destroy
    void do_deallocate(void *, sizet, sizet) override;

    sizet _offset;
};

} // namespace noble_steed