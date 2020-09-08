#include "strategy.h"

std::string Strategy::read_file(const std::string& file_path)
{
    std::stringstream output;
    std::ifstream file;
    file.open(file_path.c_str());
    std::string line;
    while(std::getline(file,line))
    {
        output << line << std::endl;
    }
    return output.str();
}

void Strategy::save_file(const std::string& file_path,std::string script)
{
    QFile file(file_path.c_str());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out<<script.c_str();
        file.close();
    }
}

void Strategy::set_script(std::string script)
{   ///mid 设置 被编辑后的 python 程序
    m_script = script;
    save_file(m_file_name,m_script);
}

std::string Strategy::get_script()
{
    return m_script;
}
