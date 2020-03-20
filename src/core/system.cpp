#include <noble_steed/core/system.h>
#include <noble_steed/core/logger.h>

namespace noble_steed
{
System::System()
{}

System::~System()
{}

void System::initialize(const Variant_Map & init_params)
{
    rttr::type t = get_derived_info().m_type;
    ilog("Initializing system with type {}",String(t.get_name()));
}

void System::terminate()
{
    rttr::type t = get_derived_info().m_type;
    ilog("Terminating system with type {}",String(t.get_name()));
}

} // namespace noble_steed