QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addlayerdialog.cpp \
    addlinedialog.cpp \
    addpaddialog.cpp \
    feature.cpp \
    job.cpp \
    layer.cpp \
    layerlistmodel.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    pad.cpp \
    rectangle.cpp \
    round.cpp \
    shape.cpp \
    view.cpp

HEADERS += \
    addlayerdialog.h \
    addlinedialog.h \
    addpaddialog.h \
    feature.h \
    job.h \
    layer.h \
    layerlistmodel.h \
    line.h \
    mainwindow.h \
    pad.h \
    rectangle.h \
    round.h \
    shape.h \
    typedefine.h \
    view.h

FORMS += \
    addlayerdialog.ui \
    addlinedialog.ui \
    addpaddialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Images.qrc
