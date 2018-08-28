#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T16:08:55
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lrt -lpcap

TARGET = ECTMaster
TEMPLATE = app
INCLUDEPATH += /usr/include
INCLUDEPATH += ./ECAT/osal
INCLUDEPATH += ./ECAT/oshw
INCLUDEPATH += ./ECAT/soem
INCLUDEPATH += ./ECAT/test
INCLUDEPATH += ./ECAT
INCLUDEPATH += /usr/local/include

SOURCES += main.cpp\
        ethercatmaster.cpp \
    ECAT/osal/osal.c \
    ECAT/oshw/nicdrv.c \
    ECAT/oshw/oshw.c \
    ECAT/soem/ethercatbase.c \
    ECAT/soem/ethercatcoe.c \
    ECAT/soem/ethercatconfig.c \
    ECAT/soem/ethercatdc.c \
    ECAT/soem/ethercatfoe.c \
    ECAT/soem/ethercatmain.c \
    ECAT/soem/ethercatprint.c \
    ECAT/soem/ethercatsoe.c \
    ECAT/test/simple_test.c

HEADERS  += ethercatmaster.h \
    ECAT/osal/osal.h \
    ECAT/osal/osal_defs.h \
    ECAT/oshw/nicdrv.h \
    ECAT/oshw/oshw.h \
    ECAT/soem/ethercat.h \
    ECAT/soem/ethercatbase.h \
    ECAT/soem/ethercatcoe.h \
    ECAT/soem/ethercatconfig.h \
    ECAT/soem/ethercatconfiglist.h \
    ECAT/soem/ethercatdc.h \
    ECAT/soem/ethercatfoe.h \
    ECAT/soem/ethercatmain.h \
    ECAT/soem/ethercatprint.h \
    ECAT/soem/ethercatsoe.h \
    ECAT/soem/ethercattype.h \
    ECAT/test/simple_test.h

FORMS    += ethercatmaster.ui
