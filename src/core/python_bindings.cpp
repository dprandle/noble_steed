#include <noble_steed/core/system.h>
#include <pybind11/pybind11.h>

PYBIND11_MODULE(Noble_Steed,m)
{
    using namespace noble_steed;
    namespace py = pybind11;
    
    py::class_<System>(m,"System")
        .def(py::init<>())
        .def_property("internal", &System::get_internal, &System::set_internal)
        .def("log_internal", &System::log_internal)
        .def_property("internal_num",&System::get_num,&System::set_num);
}
