QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addressconw.cpp \
    commentw.cpp \
    fooddetailw.cpp \
    fooditemw.cpp \
    forgetpassw.cpp \
    innershopw.cpp \
    loginw.cpp \
    main.cpp \
    mainw.cpp \
    orderitemw.cpp \
    payw.cpp \
    registerw.cpp \
    selectw.cpp \
    shopcaritemw.cpp \
    shopitemw.cpp \
    userinfow.cpp

HEADERS += \
    addressconw.h \
    commentw.h \
    fooddetailw.h \
    fooditemw.h \
    forgetpassw.h \
    innershopw.h \
    loginw.h \
    mainw.h \
    orderitemw.h \
    payw.h \
    registerw.h \
    selectw.h \
    shopcaritemw.h \
    shopitemw.h \
    userinfow.h

FORMS += \
    addressconw.ui \
    commentw.ui \
    fooddetailw.ui \
    fooditemw.ui \
    forgetpassw.ui \
    innershopw.ui \
    loginw.ui \
    mainw.ui \
    orderitemw.ui \
    payw.ui \
    registerw.ui \
    selectw.ui \
    shopcaritemw.ui \
    shopitemw.ui \
    userinfow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
