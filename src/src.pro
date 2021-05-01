include("dualword.version.pri")

TARGET = dualword
TEMPLATE = app
DEPENDPATH += .
CONFIG += qt release
CONFIG += no_keywords

message(Building: = $${TARGET} - $${VERSION})

HEADERS = \
	app/Entry.h \
	app/Speech.h \
	app/Index.h \
	gui/form/Form.h \
	gui/form/Stat.h \
	gui/form/Dict.h \
	gui/form/Log.h \
	gui/form/TableModel.h \
	gui/MainWindow.h	
			
SOURCES = \
	app/main.cpp \
	app/Entry.cpp \
	app/Speech.cpp \
	app/Index.cpp  \
	gui/form/Form.cpp \
	gui/form/Stat.cpp \
	gui/form/Dict.cpp \
	gui/form/Log.cpp \
	gui/form/TableModel.cpp \
	gui/slots.cpp \
	gui/MainWindow.cpp
								
unix {
	include("dualword.unix.pri")
}
