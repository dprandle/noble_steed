#include <noble_steed/core/context.h>
#include <noble_steed/core/common.h>

namespace noble_steed
{
Context * Context::s_this_ = nullptr;

Context::Context():
    mem_pool_(std::size_t(100 * MB_SIZE), std::size_t(KB_SIZE)),
    mem_stack_(std::size_t(100 * MB_SIZE))
{
    s_this_ = this;
}

Context & Context::inst()
{
    return *s_this_;
}

Context::~Context()
{}

} // namespace noble_steed
