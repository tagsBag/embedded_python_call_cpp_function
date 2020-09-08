#ifndef CRASH_HANDLER_H
#define CRASH_HANDLER_H

#include <QDate>
#include <QDir>
#include <QMessageBox>
#include <QString>

#include <Windows.h>
#include <DbgHelp.h>
#include <Windows.h>

long ApplicationCrashHandler(EXCEPTION_POINTERS *pException);
#endif // CRASH_HANDLER_H
