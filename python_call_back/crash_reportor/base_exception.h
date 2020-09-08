#pragma once
#include "stack_walker.h"
#include <eh.h>
#include <iostream>
#include "crash_handler.h"

class CBaseException : public CStackWalker
{
public:
	CBaseException(HANDLE hProcess = GetCurrentProcess(), WORD wPID = GetCurrentProcessId(), LPCTSTR lpSymbolPath = NULL, PEXCEPTION_POINTERS pEp = NULL);
	~CBaseException(void);
	virtual void OutputString(LPCTSTR lpszFormat, ...);
	virtual void ShowLoadModules();
	virtual void ShowCallstack(HANDLE hThread = GetCurrentThread(), const CONTEXT* context = NULL);
	virtual void ShowExceptionResoult(DWORD dwExceptionCode);
	virtual BOOL GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset );
	virtual void ShowRegistorInformation(PCONTEXT pCtx);
	virtual void ShowExceptionInformation();
	static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
	static void STF(unsigned int ui,  PEXCEPTION_POINTERS pEp);
protected:
	PEXCEPTION_POINTERS m_pEp;
    std::string m_error_msg;
};

///mid 以下提供 两种 奔溃信息获取方式,一个是 用 dump 文件,一个使用 text 文件
#define SET_DEFULTER_HANDLER() SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);  //mid dump
#define SET_DEFULTER_HANDLER() SetUnhandledExceptionFilter(CBaseException::UnhandledExceptionFilter)                //mid text

#define SET_DEFAUL_EXCEPTION() _set_se_translator(CBaseException::STF)
