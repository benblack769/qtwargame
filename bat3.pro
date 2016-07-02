#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T04:58:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bat3
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -fpermissive
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -mtune=native
win32{
#put in directory for boost here if I need
#INCLUDEPATH += ../../../../Downloads/boost_1_56_0/boost_1_56_0
INCLUDEPATH += "../../PythonProjects/"
}
unix{
QMAKE_LFLAGS += -Wl,-rpath,/usr/local/lib64
INCLUDEPATH += ../../rand_projs/headerlib/
}
SOURCES += main.cpp \
	computer/compmacromove.cpp \
	computer/computerbuild.cpp \
	computer/computercore.cpp \
	computer/computermove.cpp \
	help/constvars.cpp \
	interface/mainwindow.cpp \
	interface/realplayer.cpp \
	interface/screen.cpp \
	interface/myview.cpp \
	troops/catapult.cpp \
	troops/soldier.cpp \
	troops/specialbuilds.cpp \
	troops/troop.cpp \
	base.cpp \
	building.cpp \
	group.cpp \
	player.cpp \
	interface/screenhelp.cpp \
	interface/debuginter.cpp

FORMS    += mainwindow.ui

RESOURCES += \
		pics.qrc

HEADERS += \
	interface/mainwindow.h \
	interface/myview.h \
	help/array2d.h \
	help/helpstuff.h \
	help/iterfns.h \
	help/pointoperators.h \
	help/rangearray.h \
	help/thingstuff.h \
	interface/screen.h \
	troops/catapult.h \
	troops/soldier.h \
	troops/specialbuilds.h \
	base.h \
	building.h \
	computerplayer.h \
	globalinfo.h \
	group.h \
	player.h \
	realplayer.h \
	troop.h \
	troopinfo.h \
	interface/screenhelp.h \
	interface/debuginter.h

DISTFILES += \
    AI_Descrip.txt
