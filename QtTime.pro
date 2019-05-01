TEMPLATE = app
TARGET = QtTime

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    window.cpp \
	alarmWidget.cpp \
	timerWidget.cpp \
	stopWatchWidget.cpp

HEADERS += \
    window.h \ 
	alarmWidget.h \
	timerWidget.h \
	stopWatchWidget.h \
    alarmEvent.h
