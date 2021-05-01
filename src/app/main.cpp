/*
 * Copyright (C) 2012 Alexander Busorgin
 *
 *	This file is part of DualWord. Website: http://github.com/dualword
 *
 *	DualWord is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	DualWord is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with DualWord.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#define _DUALWORD "DualWord"

#include <QApplication>

#include "gui/MainWindow.h"
#include "app/Index.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {

	QApplication app(argc, argv);
	QApplication::setStyle("windows");
	app.setApplicationName(QString(_DUALWORD));
	QCoreApplication::setOrganizationName("dualword");

#ifdef _VER
	app.setApplicationVersion(_VER);
#endif

	MainWindow mainWindow;
	mainWindow.setVisible(false);
	QRect screen = QApplication::desktop()->screenGeometry();
	int x = (screen.width()-mainWindow.width()) / 2;
	int y = (screen.height()-mainWindow.height()) / 2;
	mainWindow.move(x, y);
	mainWindow.show();
	mainWindow.setVisible(true);
    QTimer::singleShot(0,mainWindow.index(),SLOT(init()));
	return app.exec();

}
