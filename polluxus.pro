#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T09:39:17
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = polluxus
TEMPLATE = app

CONFIG += c++11
QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-unused-parameter

SOURCES += main.cpp\
        polluxusmain.cpp \
    tws/src/EClientSocketBase.cpp \
    tws/src/EPosixClientSocket.cpp \
    posixibclient.cpp \
    digitalclock.cpp \
    polluxuslogger.cpp \
    messageprocessor.cpp \
    strategy/kalmanpairui.cpp \
    contractmanagermodel.cpp \
    orderbookwidget.cpp \
    runguard.cpp \
    contractmanagerview.cpp \
    contractmanager.cpp \
    dbmanager.cpp \
    ordermanager.cpp

HEADERS  += polluxusmain.h \
    tws/Shared/CommissionReport.h \
    tws/Shared/CommonDefs.h \
    tws/Shared/Contract.h \
    tws/Shared/EClient.h \
    tws/Shared/EClientSocketBase.h \
    tws/Shared/EClientSocketBaseImpl.h \
    tws/Shared/EWrapper.h \
    tws/Shared/Execution.h \
    tws/Shared/IBString.h \
    tws/Shared/Order.h \
    tws/Shared/OrderState.h \
    tws/Shared/ScannerSubscription.h \
    tws/Shared/shared_ptr.h \
    tws/Shared/StdAfx.h \
    tws/Shared/TagValue.h \
    tws/Shared/TwsSocketClientErrors.h \
    tws/src/EPosixClientSocket.h \
    tws/src/EPosixClientSocketPlatform.h \
    posixibclient.h \
    marketdata.h \
    digitalclock.h \
    polluxuslogger.h \
    messageprocessor.h \
    strategy/kalmanpairui.h \
    contractmanagermodel.h \
    orderbookwidget.h \
    runguard.h \
    contractmanagerview.h \
    contractmanager.h \
    dbmanager.h \
    ordermanager.h


INCLUDEPATH += tws/src \
               tws/Shared

INCLUDEPATH += ../resources/images \
               ../resources/stylesheets

RESOURCES += \
    ../resources/resources.qrc

DISTFILES += \
    workspace.ini \
    readme.txt
