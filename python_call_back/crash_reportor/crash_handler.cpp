#include "crash_handler.h"

long ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    QString dump_folder = "./dump/";
    QString file_extension = ".dmp";
    QString current_date = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    QString file_name = dump_folder + current_date + file_extension;

    {
        // 在程序exe的上级目录中创建dmp文件夹
        QDir *dmp = new QDir;
        bool exist = dmp->exists(dump_folder);
        if(exist == false)
        {
            dmp->mkdir(dump_folder);
        }
    }

    {
        EXCEPTION_RECORD *record = pException->ExceptionRecord;
        QString errCode(QString::number(record->ExceptionCode, 16));
        QString errAddr(QString::number((uint)record->ExceptionAddress, 16));
        QString errFlag(QString::number(record->ExceptionFlags, 16));
        QString errPara(QString::number(record->NumberParameters, 16));

        QMessageBox::critical(nullptr,"程序奔溃",
                              QString("<FONT size=4><div><b>请联系客服</b><br/></div> \
                                      <div>错误代码：%1</div> \
                                      <div>错误地址：%2</div> \
                                      <div>错误标记：%3</div> \
                                      <div>错误参数：%4</div> \
                                      </FONT>").arg(errCode).arg(errAddr).arg(errFlag).arg(errPara),
                                      QMessageBox::Ok);
    }
    {
        if(true)
        {   ///mid CreateFile() 的 第一个参数的 utf16()和LPCWSTR必须要这样,不能在函数外计算,否则无法创建成功文件,不知何故?
            HANDLE hDumpFile = CreateFile((LPCWSTR)file_name.utf16(),GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if(hDumpFile != INVALID_HANDLE_VALUE)
            {
                MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
                dumpInfo.ExceptionPointers = pException;
                dumpInfo.ThreadId = GetCurrentThreadId();
                dumpInfo.ClientPointers = TRUE;
                {   ///mid 需要添加 LIBS += -lDbgHelp 否则无法调用,这个函数在一个库里
                    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),hDumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
                }
                CloseHandle(hDumpFile);
                QMessageBox::information(nullptr,"dump成功:",file_name);
            }
            else
            {
                QMessageBox::information(nullptr,"dump失败,dump文件创建失败:",file_name);
            }
        }
    }
    return EXCEPTION_EXECUTE_HANDLER;
    /*
    ————————————————
    版权声明：本文为CSDN博主「程序猿的杂货店」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
    原文链接：https://blog.csdn.net/taoerit/article/details/76297484
    */
}
