#include "context.h"

using namespace boost::python;

Context::Context(Strategy * strategy)
{
    m_strategy = strategy;

    m_strategy->m_main_namespace["Context"] = boost::python::ptr(m_strategy);
}

Context::~Context()
{
}

char const* greet()
{
   return "hello, world";
}

///mid 对应 #define INIT_MODULE PyInit_strategy  ,"strategy"是个后缀,可以任意,但是必须一致
BOOST_PYTHON_MODULE(strategy)
{
    // This will enable user-defined docstrings and python signatures,
    // while disabling the C++ signatures
    scope().attr("__version__") = "1.0.0";
    scope().attr("__doc__") = "a demo module to use boost_python.";
    docstring_options local_docstring_options(true, false, false);


    class_<Strategy>("Strategy")
            .def(init<>()) //mid init()方法 只适用用于 python 独立导入 c++ 导出类
            .def(init<double>())
            .def("maxab", &Strategy::maxab, "return max of two numbers.\n")
            .def("greet", greet)
            .def_readwrite("x",&Strategy::m_x)
            .def_readwrite("y",&Strategy::m_y)
            // methods for Chinese word segmentation
            .def("whoami", &Strategy::whoami, "method's doc string...")
            .def("myrole", &Strategy::myrole, "method's doc string...")
            .def("write_log_string",&Strategy::write_log_string,arg("log"))
            .def("write_log_int",&Strategy::write_log_int,arg("log"))
            .staticmethod("myrole");


    register_ptr_to_python<boost::shared_ptr< std::string >>();

}
