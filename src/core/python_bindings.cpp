#include <pybind11/embed.h>
#include <noble_steed/core/system.h>
#include <noble_steed/core/context.h>

noble_steed::Context * get_global_context()
{
    return &noble_steed::ns_ctxt;
}

PYBIND11_EMBEDDED_MODULE(Noble_Steed,m)
{
    using namespace noble_steed;
    namespace py = pybind11;
    
    py::class_<Context>(m,"Context")
        .def(py::init<>())
        .def("inst",&Context::inst);
        //.def_readwrite("sys", &Context::sys_);
}