QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FilterDialog.cpp \
    SyncPage.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    FilterDialog.h \
    MainWindow.h \
    SyncPage.h

FORMS += \
    FilterDialog.ui \
    MainWindow.ui \
    SyncPage.ui

INCLUDEPATH += ../LibFCL

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../Resource/Res.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../LibFCL/release/ -lLibFCL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../LibFCL/debug/ -lLibFCL
else:unix: LIBS += -L$$OUT_PWD/../LibFCL/ -lLibFCL

INCLUDEPATH += $$PWD/../LibFCL
DEPENDPATH += $$PWD/../LibFCL

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LibFCL/release/libLibFCL.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LibFCL/debug/libLibFCL.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LibFCL/release/LibFCL.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../LibFCL/debug/LibFCL.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../LibFCL/libLibFCL.a
