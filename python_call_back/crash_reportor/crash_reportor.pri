

#mid pro 文件中 包含此文件
#include(crash_reportor/crash_reportor.pri)
#mid 在 main() 中 调用 以下 语句,即可 在 release 中 输出奔溃 信息
#SET_DEFULTER_HANDLER();
# 只适用于 windows

#mid $$PWD 代表 pri 文件 所在 的目录
SOURCES += \
    $$PWD/../crash_reportor/base_exception.cpp \
    $$PWD/../crash_reportor/stack_walker.cpp \
    $$PWD/crash_handler.cpp

HEADERS += \
    $$PWD/../crash_reportor/base_exception.h \
    $$PWD/../crash_reportor/stack_walker.h \
    $$PWD/../crash_reportor/crash_handler.h \

INCLUDEPATH += $$PWD/../        #mid 这个重要,使用者都是从这个目录为参照做引用,如:#include "crash_reportor/base_exception.h"


#############################################################################
#mid 以下是为 dump 转为 text 的依赖,是 window 系统的自带库,不必指明路径
win32: LIBS += -lVersion
win32: LIBS += -lDbgHelp

#mid 以下目的是Release版也将生成“.pdb”后缀的调试信息文件
#INCREMENTAL:NO 非增量编译
#/DEBUG 生成调试信息
#-Zi 调试信息格式 pdb
#优化选项,O2(优化速度),Od(禁用优化)
#多线程,MD(多线程动态,默认安装的qt用这个模式),MT(多线程静态)

#QMAKE_CXXFLAGS_RELEASE    += -Od -MD -Zi                        #mid c++ compiler 选项,QMAKE_CFLAGS_RELEASE是 c compiler选项
#QMAKE_LFLAGS_RELEASE     += /INCREMENTAL:NO /DEBUG               #mid linker 选项

#mid 注意 CFLAGS 和 CXXFLAGS 的区别,一个用于指定 c compiler 编译选项,一个用于 指定 c++ compiler 选项
#mid QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO 用于设置参数,此参数将用于 QMAKE_CXXFLAGS_RELEASE

#QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -Od -MD -Zi                #mid Od 用于 取消编译优化,否则一些 有错误 的语句 会被 优化掉
#QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO += /INCREMENTAL:NO /DEBUG

#QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
#############################################################################
