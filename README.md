# embedded_python_call_cpp_function

# 有个天坑
  ## 1.debug模式下以下两种方式都正确
    void Strategy::write_log_string(boost::python::str log)
    {
        std::cout << boost::python::extract<char*>(log) << std::endl;
    }
    void Strategy::write_log_string(std::string log)
    {
        std::cout << log << std::endl;
    }
  ## 2.release模式下,只有以下方式正确
    void Strategy::write_log_string(boost::python::str log)
    {
        std::cout << boost::python::extract<char*>(log) << std::endl;
    }
    而以下方式会直接奔溃,不做任何提示
    void Strategy::write_log_string(std::string log)
    {
        std::cout << log << std::endl;
    }
    而且 crash reportor也不做任何记录,该是跟boost::python没有包含调试信息所致(boost::python使用了预编译版本,无pdb文件)
    后来使用 visual studio 的qt插件直接导入qm工程运行后发现了引发asm断点的最近一条c++调用才猜测了可能的问题
      猜测可能是 boost::python::str 再release模式也有引用计数,而std::string 在release模式下局部变量以非引用方式传入,用完即销毁
