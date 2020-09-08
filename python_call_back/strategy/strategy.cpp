#include <boost/python.hpp>

#include "strategy.h"
#include "context.h"

int Strategy::maxab(int a, int b)
{
    int c = a>b?a:b;
    std::cout << "maxab(%d,%d) = "<< c << std::endl;
    return c;
}

Strategy::Strategy(double x) : m_x(x)
{
    m_file_name = "./strategy.py";
    m_script = read_file(m_file_name);
    m_context = std::make_shared<Context>(this);        ///mid 这个必须要python 环境初始化之后才行
}

void Strategy::myrole()
{
    std::cout << "I'm a student!" << std::endl;
}

void Strategy::write_log_string(boost::python::str log)
{
    std::cout << boost::python::extract<char*>(log) << std::endl;
}

void Strategy::write_log_int(int log)
{
    std::cout << log << std::endl;
}

void Strategy::run_python()
{
    try
    {
        {   ///mid 执行 文本 python,传入 参数 字典 namespace_
            boost::python::exec(
                        "print('12121212121')\n"
                        "\n", m_main_namespace);

            boost::python::exec(m_script.c_str(),m_main_namespace);     ///mid 此语句 执行时,会将 m_main_namespace 中的数据 传入 python 文件执行环境,同时将 python 文件 的执行数据 存入 m_main_namespace

            if(m_main_namespace.has_key("Context"))
            {
                ///mid 1) c++ 调用 python 无参无返回值函数
                if(m_main_namespace.has_key("on_start"))
                {
                    on_start_py = m_main_namespace["on_start"];
                    on_start_py();
                }

                ///mid 2) c++ 调用 python 无参有返回值 函数,数值
                if(m_main_namespace.has_key("get_age"))
                {
                    boost::python::object get_age_py = m_main_namespace["get_age"];
                    boost::python::object ret = get_age_py();
                    std::cout << boost::python::extract<double>(ret) << std::endl;
                    std::cout << boost::python::extract<double>(get_age_py()) << std::endl;
                }

                ///mid 2) c++ 调用 python 无参有返回值 函数,数值
                if(m_main_namespace.has_key("get_name"))
                {
                    boost::python::object get_name_py = m_main_namespace["get_name"];
                    boost::python::object ret = get_name_py();
                    std::cout << boost::python::extract<char *>(ret) << std::endl;
                    std::cout << boost::python::extract<char *>(get_name_py()) << std::endl;
                }

                ///mid 3) c++ 传入 python 参数
                if(m_main_namespace.has_key("show_info"))
                {
                    boost::python::object show_info_py = m_main_namespace["show_info"];
                    show_info_py("call with params from c++");
                }

                if(true)
                {   ///mid 说明变量可以赋值 python -> c++
                    boost::python::exec("Context.x = 110", m_main_namespace);
                    std::cout << m_x << std::endl;
                }
                if(true)
                {   ///mid python 通过 c++ 传递变量 成功
                    boost::python::exec("x = 10", m_main_namespace);    ///mid python 文件 若 定义 有 _add(a,b)函数,则此处调用成功
                    boost::python::object rand = boost::python::eval("sin(x)", m_main_namespace);
                    std::cout <<"**************"<< boost::python::extract<double>(rand) << std::endl;
                }
                if(true)
                {   ///mid python 调用 c++ 无参方法成功
                    boost::python::exec("Context.whoami()", m_main_namespace);
                    boost::python::exec("Context.maxab(2,5)", m_main_namespace);
                }
                {
                    boost::python::exec("Context.write_log_string('call write_log from python')", m_main_namespace,m_main_namespace);
                }
            }
        }
    }
    catch(boost::python::error_already_set& e)
    {
        PyErr_PrintEx(0);
    }
    catch(std::exception& e)
    {
        std::cout << "std::exception" << std::endl;
    }
    catch (...)
    {  //三个点表示可以捕获任意类型的异常
        std::cout << "there is an error" << std::endl;
    }
}

void Strategy::whoami()
{
    std::cout << "I am laowang"<<std::endl;
}
