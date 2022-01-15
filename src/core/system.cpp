#include <noble_steed/core/system.h>
#include <noble_steed/io/logger.h>
#include <noble_steed/core/variant.h>

namespace noble_steed
{
System::System():Context_Obj()
{}

System::System(const System & copy):Context_Obj(copy)
{}

System & System::operator=(System rhs)
{
    Context_Obj::swap(rhs);
    swap(rhs);
    return *this;
}

void System::swap(const System & rhs)
{}

System::~System()
{}

void System::initialize(const Variant_Map & init_params)
{
    //ilog("Initializing system with type {}", String(t.get_name()));
}

void System::terminate()
{
    //ilog("Terminating system with type {}", String(t.get_name()));
}

} // namespace noble_steed