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

#include "gui/MainWindow.h"

void MainWindow::setSlots() {

	//index
	connect(idx.get(),SIGNAL(newMessage(int, const QString&)), this, SLOT(newMessage(int, const QString&)),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(newMessage(int, const QString&)), log, SLOT(newMessage(int, const QString&)),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(newBox(const QString&)), this, SLOT(showBox(const QString&)));
	connect(idx.get(),SIGNAL(requestRebuild(const QString&)), this, SLOT(askRecreate(const QString&)),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(indexReady()), this, SLOT(init()),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(reinit()), idx.get(), SLOT(init()),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(resetDone()), this, SLOT(hideModal()),Qt::QueuedConnection);
	connect(idx.get(),SIGNAL(resetStart()), this, SLOT(showModal()),Qt::QueuedConnection);
    connect(idx.get(),SIGNAL(languageChange()), this, SLOT(langChange()),Qt::QueuedConnection);
    connect(idx.get(),SIGNAL(setNext(unique_ptr<Entry>&)), this, SLOT(setNext(unique_ptr<Entry>&)));

	//gui
    connect(list1,SIGNAL(currentIndexChanged(int)), this, SLOT(on_list1_IndexChanged(int)));
    connect(list2,SIGNAL(currentIndexChanged(int)), this, SLOT(on_list2_IndexChanged(int)));
	connect(btnSay, SIGNAL(clicked()), this, SLOT(say()));
    connect(questionBox,SIGNAL(toggled(bool)), this, SLOT(on_questionBox_click(bool)));
    connect(btnShow,SIGNAL(clicked()), this, SLOT(showAnswer()));
    connect(btnNext,SIGNAL(clicked()), this, SLOT(getNext()));
    connect(btnKnow,SIGNAL(clicked()), this, SLOT(makeKnown()));
    connect(btnReset,SIGNAL(clicked()), this, SLOT(reset()));
    connect(chkSpeech,SIGNAL(stateChanged (int)), this, SLOT(speechOn(int)));
    connect(voice,SIGNAL(currentIndexChanged(int)), this, SLOT(selectVoice(int)));
    connect(tab,SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    //menu
	connect(aAbout,SIGNAL(triggered()), this, SLOT(showAboutForm()));
    connect(aQuit, SIGNAL(triggered()), this, SLOT(doExit()));

    //mainwindow
    connect(this,SIGNAL(quit()), this, SLOT(close()),Qt::QueuedConnection);
    connect(this,SIGNAL(recreate()), idx.get(), SLOT(recreate()),Qt::QueuedConnection);
    connect(this,SIGNAL(Next()), idx.get(), SLOT(getNext()),Qt::QueuedConnection);

}

