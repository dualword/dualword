#qmake indexer.pro

include("dualword.version.pri")

TARGET = dualword-indexer
TEMPLATE = app
DEPENDPATH += .
CONFIG += qt release
CONFIG += no_keywords

HEADERS = \
	app/Entry.h \
	app/Index.h
			
SOURCES = \
	app/indexer.cpp \
	app/Entry.cpp \
	app/Index.cpp

unix {
	include("dualword.unix.pri")
}
