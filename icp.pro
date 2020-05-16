QT += core gui xml widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    src/datamodel.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mainwindowzoom.cpp \
    src/maptimer.cpp \
    src/stop.cpp \
    src/streetitem.cpp \
    src/trafficcircleitem.cpp \
    src/trip.cpp \
    src/uilogic.cpp \
    src/util.cpp \
    src/vehicle.cpp

HEADERS += \
    src/datamodel.h \
    src/deselectabletreeview.h \
    src/direction.h \
    src/mainwindow.h \
    src/maptimer.h \
    src/stop.h \
    src/street.h \
    src/streetitem.h \
    src/trafficcircleitem.h \
    src/trip.h \
    src/util.h \
    src/vehicle.h

FORMS += \
    src/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    README.txt \
    Doxyfile

MAKEFILE = QMakefile
TARGET = icp

#CONFIG(release, debug|release) {
    DEFINES += NDEBUG
#}

cleantarget.commands = rm -f $${TARGET};
clean.depends = cleantarget
run.commands = ./$${TARGET};
QMAKE_EXTRA_TARGETS += clean cleantarget run
