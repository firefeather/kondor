#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T10:50:12
#
#-------------------------------------------------

QT       += core gui serialport network svg sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FonterControler
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


include(./tools/zipunzip/karchive.pri)

SOURCES += \
        main.cpp \
        controlmain.cpp \
    printer/printer.cpp \
    server/mserver.cpp \
    server/serverthread.cpp \
    interface/storagetype.cpp \
    interface/printset.cpp \
    interface/configset.cpp \
    tools/easylogging++.cc \
    camera/cvmatandqimage.cpp \
    camera/qcameraer.cpp \
    interface/controltool.cpp \
    printer/hardware/projector/dlpcontrol.cpp \
    printer/hardware/projector/projector.cpp \
    tools/zipunzip/zipunzip.cpp \
    printer/printfiledata/printfiledata.cpp \
    printer/printfiledata/BgzData/bgzformat.cpp \
    printer/printfiledata/SlcData/slcreader.cpp \
    printer/printfiledata/SvgData/svgreaders.cpp \
    interface/wifissid.cpp \
    interface/wifipwd.cpp \
    tools/keyboard/frmnum.cpp \
    tools/keyboard/frminput.cpp \
    interface/usbstoragefile.cpp \
    printer/hardware/serialport/cpserialport.cpp

HEADERS += \
        controlmain.h \
    printer/printer.h \
    server/mserver.h \
    server/serverthread.h \
    interface/storagetype.h \
    interface/printset.h \
    interface/configset.h \
    tools/easylogging++.h \
    camera/cvmatandqimage.h \
    camera/qcameraer.h \
    interface/controltool.h \
    printer/hardware/projector/dlpcontrol.h \
    printer/hardware/projector/projector.h \
    tools/zipunzip/zipunzip.h \
    printer/printfiledata/printfiledata.h \
    printer/printfiledata/BgzData/bgzformat.h \
    printer/printfiledata/SlcData/slcreader.h \
    printer/printfiledata/SvgData/svgreaders.h \
    interface/wifissid.h \
    interface/wifipwd.h \
    tools/keyboard/frmnum.h \
    tools/keyboard/frminput.h \
    interface/usbstoragefile.h \
    printer/hardware/serialport/cpserialport.h

FORMS += \
        controlmain.ui \
    interface/storagetype.ui \
    interface/printset.ui \
    interface/configset.ui \
    interface/controltool.ui \
    printer/hardware/projector/projector.ui \
    interface/wifissid.ui \
    interface/wifipwd.ui \
    tools/keyboard/frmnum.ui \
    tools/keyboard/frminput.ui \
    interface/usbstoragefile.ui \
    printer/hardware/serialport/cpserialport.ui



RESOURCES += \
    qrc/style.qrc


RC_ICONS = logo.ico


unix:!macx{
# linux only
INCLUDEPATH += /usr/include \
                /usr/include/opencv \
                /usr/include/opencv2

LIBS += /usr/lib/arm-linux-gnueabihf/libopencv_highgui.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_core.so    \
        /usr/lib/arm-linux-gnueabihf/libopencv_imgproc.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_calib3d.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_contrib.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_features2d.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_flann.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_gpu.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_legacy.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_ml.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_objdetect.so \
        /usr/lib/arm-linux-gnueabihf/libopencv_video.so

}
win32 {
#here is the opencv libary
INCLUDEPATH+= C:\opencv2.4.9\include\opencv\
              C:\opencv2.4.9\include\opencv2\
              C:\opencv2.4.9\include

LIBS += -lOpengl32

LIBS+= C:\opencv2.4.9\x64\mingw\lib\libopencv_calib3d249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_contrib249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_core249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_features2d249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_flann249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_gpu249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_highgui249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_imgproc249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_legacy249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_ml249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_objdetect249.dll.a\
        C:\opencv2.4.9\x64\mingw\lib\libopencv_video249.dll.a

}
