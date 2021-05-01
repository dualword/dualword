QMAKE_CXXFLAGS += $$CFLAGS $$CPPFLAGS $$CXXFLAGS
QMAKE_LFLAGS += $$LDFLAGS

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -I/usr/include/festival -I/usr/include/speech_tools
QMAKE_CXXFLAGS += -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-deprecated
#QMAKE_CXXFLAGS += -O0 -ggdb -g3

LIBS += -lxapian -lFestival -lestools -lestbase -leststring

OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc

isEmpty(PREFIX) {
	PREFIX = /usr
}

DATADIR =$$PREFIX/share
DEFINES += DATADIR=\\\"$$DATADIR\\\"
	
target.path = $$PREFIX/bin
desktop.path = $$DATADIR/applications/
desktop.files = ../pkg/$${TARGET}.desktop
icon.path = $$DATADIR/icons/hicolor/32x32/apps
icon.files += images/$${TARGET}.png
data.path = $$DATADIR/dualword/index
data.files = ../index/*
man.path = $$DATADIR/man/man1
man.files = ../pkg/dualword.1

#INSTALLS += target desktop icon data man
QMAKE_CLEAN += ../Makefile Makefile $${TARGET};
QMAKE_CLEAN += $(DEL_FILE) -r .build;

install.depends += first FORCE
install.commands += $(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/bin/;
install.commands += $(INSTALL_PROGRAM) "$(QMAKE_TARGET)" "$(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)";
install.commands += $(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/share/applications/ || $(MKDIR) $(INSTALL_ROOT)/usr/share/applications/;
install.commands += $(INSTALL_FILE) ../pkg/dualword.desktop $(INSTALL_ROOT)/usr/share/applications/;
install.commands += $(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/share/man/man1/ || $(MKDIR) $(INSTALL_ROOT)/usr/share/man/man1/;
install.commands += $(INSTALL_FILE) ../pkg/dualword.1 $(INSTALL_ROOT)/usr/share/man/man1/;
install.commands += $(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/share/dualword/index/ || $(MKDIR) $(INSTALL_ROOT)/usr/share/dualword/index/;
install.commands += $(COPY) ../index/* $(INSTALL_ROOT)/usr/share/dualword/index/;
	
uninstall.commands += $(DEL_FILE) "$(INSTALL_ROOT)/usr/bin/$(QMAKE_TARGET)";
uninstall.commands += $(DEL_FILE) $(INSTALL_ROOT)/usr/share/applications/dualword.desktop;
uninstall.commands += $(DEL_FILE) $(INSTALL_ROOT)/usr/share/man/man1/dualword.1;
uninstall.commands += $(DEL_FILE) $(INSTALL_ROOT)/usr/share/dualword/index/*;
uninstall.commands += $(DEL_FILE) -r $(INSTALL_ROOT)/usr/share/dualword/;
QMAKE_EXTRA_TARGETS += install uninstall

