#include <noble_steed/core/system.h>
#include <noble_steed/dbg.h>
#include <rttr/registration>
#include <pybind11/pybind11.h>

namespace noble_steed
{
System::System()
{}

System::~System()
{}

const String & System::get_internal()
{
    return internal_;
}

void System::log_internal()
{
    dbg(internal_);
}

void System::set_internal(const String & to_set)
{
    internal_ = to_set;
}

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<System>("System")
        .constructor<>()
        .method("log_internal", &System::log_internal, registration::public_access)
        .property("internal", &System::get_internal, &System::set_internal);
}

PYBIND11_MODULE(Noble_Steed,m)
{
    namespace py = pybind11;
    py::class_<System>(m,"System")
        .def(py::init<>())
        .def("set_internal", &System::set_internal)
        .def("get_internal", &System::get_internal)
        .def("log_internal", &System::log_internal);
}

} // namespace noble_steed