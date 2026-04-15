QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# 禁用废弃API警告
DEFINES += QT_DEPRECATED_WARNINGS

# 目标名称
TARGET = QtClientExam
TEMPLATE = app

# 头文件目录
INCLUDEPATH += Headers

# 头文件
HEADERS += \
    Headers/LoginWindow.h \
    Headers/SplashWindow.h \
    Headers/MainWindow.h \
    Headers/DataModels.h \
    Headers/CarouselWidget.h \
    Headers/BarChartWidget.h \
    Headers/PanelWidget.h \
    Headers/AppListWidget.h \
    Headers/DataTableWidget.h

# 源文件
SOURCES += \
    Sources/main.cpp \
    Sources/LoginWindow.cpp \
    Sources/SplashWindow.cpp \
    Sources/MainWindow.cpp \
    Sources/CarouselWidget.cpp \
    Sources/BarChartWidget.cpp \
    Sources/PanelWidget.cpp \
    Sources/AppListWidget.cpp \
    Sources/DataTableWidget.cpp

# 默认部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
