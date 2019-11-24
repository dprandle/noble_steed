#pragma once

#include <Allocator.h>
#include <PoolAllocator.h>
#include <StackAllocator.h>

#include <noble_steed/core/system.h>


namespace noble_steed
{
class Context
{
    public:

    Context();
    ~Context();

    static Context & inst();

    System sys_;

    private:

    PoolAllocator mem_pool_;

    StackAllocator mem_stack_; 

    static Context * s_this_;
};
}

#define ns_ctxt Context::inst()
