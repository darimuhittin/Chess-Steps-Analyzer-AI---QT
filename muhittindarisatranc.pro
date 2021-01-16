QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analyzernode.cpp \
    analyzewindow.cpp \
    analyzeview.cpp \
    chessalgorithm.cpp \
    chessboard.cpp \
    chessview.cpp \
    main.cpp \
    mainwindow.cpp \
    minimaxalgorithm.cpp

HEADERS += \
    analyzernode.h \
    analyzewindow.h \
    analyzeview.h \
    chessalgorithm.h \
    chessboard.h \
    chessview.h \
    mainwindow.h \
    minimaxalgorithm.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pieces.qrc
