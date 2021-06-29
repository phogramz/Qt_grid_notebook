QT       += core gui charts \
            printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comboboxdelegate.cpp \
    dialogdiagram.cpp \
    dialogsettings.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    timer.cpp \
    tmonitor.cpp

HEADERS += \
    comboboxdelegate.h \
    dialogdiagram.h \
    dialogsettings.h \
    mainwindow.h \
    qcustomplot.h \
    timer.h \
    tmonitor.h

FORMS += \
    dialogdiagram.ui \
    dialogsettings.ui \
    mainwindow.ui

TRANSLATIONS += \
    translations/Monitors_QT_ru_RU.ts \
    translations/Monitors_QT_en_US.ts \
    translations/Monitors_QT_fi_FN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \

