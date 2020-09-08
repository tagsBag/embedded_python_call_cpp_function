QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
win32{#mid {要紧随其后,否则无效,1)解决界面中文乱码问题.2)解决源码文件 utf-8 编码加载问题
        QMAKE_CXXFLAGS += /utf-8
}

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += D:/boost_1_73_0_14_1_64
INCLUDEPATH += D:/mid/Python380b3/include
INCLUDEPATH += 3rdparty/
INCLUDEPATH += gui/

LIBS += -LD:/mid/Python380b3/libs
LIBS += -lpython38

LIBS += -LD:/boost_1_73_0_14_1_64/lib64-msvc-14.1
LIBS += -lboost_python38-vc141-mt-gd-x64-1_73


CONFIG(debug,debug|release){#mid 此处的相对位置为构建目标目录的相对位置
    DESTDIR = debug
    PYTHON_DLL_FILE = D:/mid/Python380b3/python38.dll
    BOOST_PYTHON_DLL_FILE = D:/boost_1_73_0_14_1_64/lib64-msvc-14.1/boost_python38-vc141-mt-gd-x64-1_73.dll
} else {
    DESTDIR = release
    PYTHON_DLL_FILE = D:/mid/Python380b3/python38.dll
    BOOST_PYTHON_DLL_FILE = D:/boost_1_73_0_14_1_64/lib64-msvc-14.1/boost_python38-vc141-mt-gd-x64-1_73.dll
}
OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rc
UI_DIR = ui

win32{
    #mid 经测试 win 下拷贝有几个要求
    #mid 1) 文件路径必须没有空格
    #mid 2) 将/替换为\\才能正确识别路径(qt使用 /)
    #mid 3) 也有可能是目标文件夹没有权限
    message("当前系统:win,复制 python38")
    DIR1 =  $$PYTHON_DLL_FILE                               #mid 需要用通配符,因为 QMAKE_POST_LINK += xcopy $$DIR11 $$DIR21 /i /y 只能出现一次,两个文件分别 xcopy 会失败
    DIR2  = $$DESTDIR/                                      #mid 拷贝到程序运行目录
    DIR11 = $$replace(DIR1, /, \\)                          #mid replace函数的第一个参数必须是大写
    DIR21 = $$replace(DIR2, /, \\)

    message($$DIR11)
    message("to========")
    message($$DIR21)
    QMAKE_POST_LINK += xcopy $$DIR11 $$DIR21 /i /y
}

include(crash_reportor/crash_reportor.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    strategy/context.cpp \
    strategy/strategy.cpp \
    strategy/strategy_util.cpp

HEADERS += \
    mainwindow.h \
    strategy/context.h \
    strategy/strategy.h

#mid 以下编译链接参数 使 release 版本也带有 debug 信息,为了release后,当程序奔溃的时候任然可以生成 crash report
QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -Od -MD -ZI                #mid Od 用于 取消编译优化,否则一些 有错误 的语句 会被 优化掉
QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO += /INCREMENTAL:NO /DEBUG

QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE += $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
