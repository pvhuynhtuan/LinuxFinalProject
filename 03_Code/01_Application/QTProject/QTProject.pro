QT       += core gui
QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INC_DIR = $${PWD}/inc
SRC_DIR = $${PWD}/src
QRC_DIR = $${PWD}/qrc
FORM_DIR = $${PWD}/ui

# Add include directory for headers
INCLUDEPATH += $${INC_DIR}

SOURCES += \
    $${SRC_DIR}/CircularProgressBar.cpp \
    $${SRC_DIR}/RingMeter.cpp \
    $${SRC_DIR}/StatusCircle.cpp \
    $${SRC_DIR}/clickablelabelclass.cpp \
    $${SRC_DIR}/cpuinfoclass.cpp \
    $${SRC_DIR}/cpuwindow.cpp \
    $${SRC_DIR}/main.cpp \
    $${SRC_DIR}/mainwindow.cpp \
    $${SRC_DIR}/memorywindow.cpp \
    $${SRC_DIR}/networkinfoclass.cpp \
    $${SRC_DIR}/radiusprogresswidget.cpp \
    $${SRC_DIR}/raminfoclass.cpp \
    $${SRC_DIR}/timewindow.cpp \
    $${SRC_DIR}/waveprogresswidget.cpp

HEADERS += \
    $${INC_DIR}/AppConfig.h \
    $${INC_DIR}/CircularProgressBar.h \
    $${INC_DIR}/RingMeter.h \
    $${INC_DIR}/StatusCircle.h \
    $${INC_DIR}/clickablelabelclass.h \
    $${INC_DIR}/cpuinfoclass.h \
    $${INC_DIR}/cpuwindow.h \
    $${INC_DIR}/mainwindow.h \
    $${INC_DIR}/memorywindow.h \
    $${INC_DIR}/networkinfoclass.h \
    $${INC_DIR}/radiusprogresswidget.h \
    $${INC_DIR}/raminfoclass.h \
    $${INC_DIR}/timewindow.h \
    $${INC_DIR}/waveprogresswidget.h

FORMS += \
    $${FORM_DIR}/cpuwindow.ui \
    $${FORM_DIR}/mainwindow.ui \
    $${FORM_DIR}/memorywindow.ui \
    $${FORM_DIR}/timewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $${QRC_DIR}/BackGround.qrc \
    $${QRC_DIR}/RadiusProgressWidgetResource.qrc \
    $${QRC_DIR}/button.qrc \
    $${QRC_DIR}/footer.qrc \
    $${QRC_DIR}/header.qrc \
    $${QRC_DIR}/temper.qrc
