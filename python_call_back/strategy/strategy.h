#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <string>
#include <iostream>
#include <boost/python.hpp>
#include <fstream>

#include <QTextStream>
#include <QFile>

//#include "interpreters_manager/interpreters_manager.h"

class Context;
class Strategy
{
public:
    Strategy() {}
    Strategy(double x_);

public:
    std::shared_ptr<Context> m_context;

    void set_script(std::string script);
    std::string get_script();

    int maxab(int a, int b);

    boost::python::dict m_main_namespace;
private:
    std::string read_file(const std::string& file_path);
    void save_file(const std::string& file_path,std::string script);

    //void init(const std::string& module, PyObject*(*initfunc)());

private:
    boost::python::object on_start_py;

public:
    double m_x;
    double m_y;
	std::string m_string;
    static void myrole();

    void write_log_string(boost::python::str log);
    void write_log_int(int log);
    void run_python();
    void whoami();

private:
    std::string m_script;
    std::string m_file_name;

private:
    //PyThreadState* m_thread_state;

    //SubInterpreter m_interpreter;                   ///mid 每个策略 自己 创建 一个 解释器
    //PyInterpreterState* m_interpreter_state;


};
#endif // FUNCTION_HPP
