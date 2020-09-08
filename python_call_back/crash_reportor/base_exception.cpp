#include "base_exception.h"
#include <atlconv.h>  //需要加入的 1

#include <QDir>
#include <QDateTime>
#include <QTextStream>

CBaseException::CBaseException(HANDLE hProcess, WORD wPID, LPCTSTR lpSymbolPath, PEXCEPTION_POINTERS pEp):
    CStackWalker(hProcess, wPID, lpSymbolPath)
{
    if (nullptr != pEp)
    {
        m_pEp = new EXCEPTION_POINTERS;
        CopyMemory(m_pEp, pEp, sizeof(EXCEPTION_POINTERS));
    }
}

CBaseException::~CBaseException(void)
{
}

void CBaseException::OutputString(LPCTSTR lpszFormat, ...)
{
    TCHAR szBuf[1024] = _T("");
    va_list args;
    va_start(args, lpszFormat);
    _vsntprintf_s(szBuf, 1024, lpszFormat, args);
    va_end(args);

    {   ///mid 将 TCHAR 转化 为 string
        std::wstring str(szBuf);
        std::string c(str.begin(),str.end());  //wstring 转换为string
        m_error_msg.append(c);
    }

    //WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), szBuf, _tcslen(szBuf), NULL, NULL);
}

void CBaseException::ShowLoadModules()
{
    OutputString( _T("loaded modules:\r\n") );
    LoadSymbol();
    LPMODULE_INFO pHead = GetLoadModules();
    LPMODULE_INFO pmi = pHead;

    TCHAR szBuf[MAX_COMPUTERNAME_LENGTH] = _T("");
    DWORD dwSize = MAX_COMPUTERNAME_LENGTH;
    //GetUserName(szBuf, &dwSize);
    //OutputString(_T("Current User:%s\r\n"), szBuf);
    OutputString(_T("BaseAddress:\tSize:\tName\tPath\tVersion\tSymbolPath\r\n"));
    while (NULL != pmi)
    {   ///mid szSymbolPath 在 release 模式下 如果不自带 debug 信息则没有输出,若带,则输出相应的 pdb 文件 路径
        OutputString(_T("%08x\t%d\t%s\t%s\t%s\t%s\r\n"), (unsigned long)(pmi->ModuleAddress), pmi->dwModSize, pmi->szModuleName, pmi->szModulePath, pmi->szVersion, pmi->szSymbolPath);
        pmi = pmi->pNext;
    }

    FreeModuleInformations(pHead);
}

void CBaseException::ShowCallstack(HANDLE hThread, const CONTEXT* context)
{
    OutputString(_T("call stack:\r\n"));
    LPSTACKINFO phead = StackWalker(hThread, context);
    FreeStackInformations(phead);
}

void CBaseException::ShowExceptionResoult(DWORD dwExceptionCode)
{
    OutputString(_T("exception code : %08x,description : "), dwExceptionCode);
    switch (dwExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
    {
        OutputString(_T("ACCESS_VIOLATION(%s)\r\n"), _T("读写非法内存"));
    }
        return ;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
    {
        OutputString(_T("DATATYPE_MISALIGNMENT(%s)\r\n"), _T("线程视图在不支持对齐的硬件上读写未对齐的数据"));
    }
        return ;
    case EXCEPTION_BREAKPOINT:
    {
        OutputString(_T("BREAKPOINT(%s)\r\n"), _T("遇到一个断点"));
    }
        return ;
    case EXCEPTION_SINGLE_STEP:
    {
        OutputString(_T("SINGLE_STEP(%s)\r\n"), _T("单步")); //一般是发生在调试事件中
    }
        return ;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
    {
        OutputString(_T("ARRAY_BOUNDS_EXCEEDED(%s)\r\n"), _T("数组访问越界"));
    }
        return ;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
    {
        OutputString(_T("FLT_DENORMAL_OPERAND(%s)\r\n"), _T("浮点操作的一个操作数不正规，给定的浮点数无法表示")); //操作数的问题
    }
        return ;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    {
        OutputString(_T("FLT_DIVIDE_BY_ZERO(%s)\r\n"), _T("浮点数除0操作"));
    }
        return ;
    case EXCEPTION_FLT_INEXACT_RESULT:
    {
        OutputString(_T("FLT_INEXACT_RESULT(%s)\r\n"), _T("浮点数操作的结果无法表示")); //无法表示一般是数据太小，超过浮点数表示的范围, 计算之后产生的结果异常
    }
        return ;
    case EXCEPTION_FLT_INVALID_OPERATION:
    {
        OutputString(_T("FLT_INVALID_OPERATION(%s)\r\n"), _T("其他浮点数异常"));
    }
        return ;
    case EXCEPTION_FLT_OVERFLOW:
    {
        OutputString(_T("FLT_OVERFLOW(%s)\r\n"), _T("浮点操作的指数超过了相应类型的最大值"));
    }
        return ;
    case EXCEPTION_FLT_STACK_CHECK:
    {
        OutputString(_T("STACK_CHECK(%s)\r\n"), _T("栈越界或者栈向下溢出"));
    }
        return ;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    {
        OutputString(_T("INT_DIVIDE_BY_ZERO(%s)\r\n"), _T("整数除0异常"));
    }
        return ;
    case EXCEPTION_INVALID_HANDLE:
    {
        OutputString(_T("INVALID_HANDLE(%s)\r\n"), _T("句柄无效"));
    }
        return ;
    case EXCEPTION_PRIV_INSTRUCTION:
    {
        OutputString(_T("PRIV_INSTRUCTION(%s)\r\n"), _T("线程试图执行当前机器模式不支持的指令"));
    }
        return ;
    case EXCEPTION_IN_PAGE_ERROR:
    {
        OutputString(_T("IN_PAGE_ERROR(%s)\r\n"), _T("线程视图访问未加载的虚拟内存页或者不能加载的虚拟内存页"));
    }
        return ;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    {
        OutputString(_T("ILLEGAL_INSTRUCTION(%s)\r\n"), _T("线程视图执行无效指令"));
    }
        return ;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
    {
        OutputString(_T("NONCONTINUABLE_EXCEPTION(%s)\r\n"), _T("线程试图在一个不可继续执行的异常发生后继续执行"));
    }
        return ;
    case EXCEPTION_STACK_OVERFLOW:
    {
        OutputString(_T("STACK_OVERFLOW(%s)\r\n"), _T("栈溢出"));
    }
        return ;
    case EXCEPTION_INVALID_DISPOSITION:
    {
        OutputString(_T("INVALID_DISPOSITION(%s)\r\n"), _T("异常处理程序给异常调度器返回了一个无效配置")); //使用高级语言编写的程序永远不会遇到这个异常
    }
        return ;
    case EXCEPTION_FLT_UNDERFLOW:
    {
        OutputString(_T("FLT_UNDERFLOW(%s)\r\n"), _T("浮点数操作的指数小于相应类型的最小值"));
    }
        return ;
    case EXCEPTION_INT_OVERFLOW:
    {
        OutputString(_T("INT_OVERFLOW(%s)\r\n"), _T("整数操作越界"));
    }
        return ;
    }

    TCHAR szBuffer[512] = { 0 };

    FormatMessage(  FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
                    GetModuleHandle( _T("NTDLL.DLL") ),
                    dwExceptionCode, 0, szBuffer, sizeof( szBuffer ), 0 );

    OutputString(_T("%s"), szBuffer);
    OutputString(_T("\r\n"));
}

LONG WINAPI CBaseException::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo )
{
    CBaseException base(GetCurrentProcess(), GetCurrentProcessId(), nullptr, pExceptionInfo);
    base.ShowExceptionInformation();

    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CBaseException::GetLogicalAddress(
        PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )
{
    MEMORY_BASIC_INFORMATION mbi;

    if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
        return FALSE;

    DWORD hMod = (DWORD)mbi.AllocationBase;

    if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
        return FALSE;

    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;
    PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

    DWORD rva = (DWORD)addr - hMod;

    //计算当前地址在第几个节
    for (unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++ )
    {
        DWORD sectionStart = pSection->VirtualAddress;
        DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

        if ( (rva >= sectionStart) && (rva <= sectionEnd) )
        {
            section = i+1;
            offset = rva - sectionStart;
            return TRUE;
        }
    }

    return FALSE;   // Should never get here!
}

void CBaseException::ShowRegistorInformation(PCONTEXT pCtx)
{
#ifdef _M_IX86  // Intel Only!  ///mid _M_x64 也属于 intel
    OutputString( _T("\nRegisters:\r\n") );

    OutputString(_T("EAX:%08X\r\nEBX:%08X\r\nECX:%08X\r\nEDX:%08X\r\nESI:%08X\r\nEDI:%08X\r\n"),
                 pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );

    OutputString( _T("CS:EIP:%04X:%08X\r\n"), pCtx->SegCs, pCtx->Eip );
    OutputString( _T("SS:ESP:%04X:%08X  EBP:%08X\r\n"),pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
    OutputString( _T("DS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n"), pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
    OutputString( _T("Flags:%08X\r\n"), pCtx->EFlags );
#elif _M_X64    ///mid added 64 位系统 用 64 位寄存器,命名 为 R 开头,32 位 命名 为 E 开头
    OutputString( _T("\nRegisters:\r\n") );

    OutputString(_T("RAX:%08X\r\nRBX:%08X\r\nRCX:%08X\r\nRDX:%08X\r\nRSI:%08X\r\nRDI:%08X\r\n"),
                 pCtx->Rax, pCtx->Rbx, pCtx->Rcx, pCtx->Rdx, pCtx->Rsi, pCtx->Rdi );

    OutputString( _T("CS:RIP:%04X:%08X\r\n"), pCtx->SegCs, pCtx->Rip );
    OutputString( _T("SS:RSP:%04X:%08X  EBP:%08X\r\n"),pCtx->SegSs, pCtx->Rsp, pCtx->Rbp );
    OutputString( _T("DS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n"), pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
    OutputString( _T("Flags:%08X\r\n"), pCtx->EFlags );
#endif

    OutputString( _T("\r\n") );
}

void CBaseException::STF(unsigned int ui,  PEXCEPTION_POINTERS pEp)
{
    CBaseException base(GetCurrentProcess(), GetCurrentProcessId(), NULL, pEp);
    throw base;
}

void CBaseException::ShowExceptionInformation()
{
    m_error_msg = "";

    //mid 1) 输出 已加载 的 dll
    ShowLoadModules();

    //mid 2) 输出 异常类型
    OutputString( _T("\r\n") );
    OutputString(_T("exception type:\r\n"));
    ShowExceptionResoult(m_pEp->ExceptionRecord->ExceptionCode);

    //mid 3) 输出 异常地址
    OutputString( _T("\r\n") );
    OutputString(_T("exception address:\r\n"));
    TCHAR szFaultingModule[MAX_PATH];
    DWORD section, offset;
    GetLogicalAddress(m_pEp->ExceptionRecord->ExceptionAddress, szFaultingModule, sizeof(szFaultingModule), section, offset );
    OutputString( _T("%08X %02X:%08X %s\r\n"), m_pEp->ExceptionRecord->ExceptionAddress, section, offset, szFaultingModule );

    //mid 4) 输出 寄存器地址
    ShowRegistorInformation(m_pEp->ContextRecord);

    //mid 5) 输出 调用堆栈
    ShowCallstack(GetCurrentThread(), m_pEp->ContextRecord);

    QString dump_folder = "./crash_reportor/";
    QString file_extension = ".txt";
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

    {   ///mid 程序奔溃有可能是由 策略 导致的,策略由多线程库执行.若要显示弹出框口,需要将 消息 转发给 gui 线程
        ///mid 而 exception 的响应接口为 一个 固定 参数 的c函数,在错误发生的时候才创建一堆对象,这些对象 不知 如何 能方便 和 gui connect,待以后解决
        QFile file(file_name);
        if(! file.open(QIODevice::Append|QIODevice::Text))  //append追加，不会覆盖之前的文件
        {
            if(false)
            {
                QMessageBox::information(nullptr,"写入失败,无法打开文件",file_name);
            }
            return;
        }
        else
        {
            {   ///mid 将崩溃信息 写入文本
                QTextStream out(&file);//写入
                out << m_error_msg.c_str();
                file.close();
            }

            if(false)
            {   ///mid 弹出提示窗口
                QMessageBox::critical(nullptr,"程序奔溃",
                                      QString("<FONT size=4><div><b>请联系客服</b><br/></div> \
                                              <div>错误信息保存文件：%1</div> \
                                              </FONT>").arg(file_name),
                                              QMessageBox::Ok);
            }
        }
    }

}
