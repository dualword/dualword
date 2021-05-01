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

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <memory>
#include <thread>

#include <QtGui>

#include "app/Speech.h"
#include "gui/form/Log.h"
#include "gui/form/Stat.h"
#include "gui/form/Dict.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~MainWindow();
	Index* index() {return idx.get();};

protected:
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *);

private:
	void showRadio();
	void createRadio(QButtonGroup* group, QBoxLayout* lay);
	void setSlots();
	void clear();

Q_SIGNALS:
	void quit();
	void recreate();
	void Next();

public Q_SLOTS:
	void init();
	void doExit();
	void showBox(const QString& msg);
	void askRecreate(const QString& msg);
	void newMessage(int, const QString& msg);
	void refreshTab1();
	void showModal();
	void hideModal();
	void reset();
	void showAnswer();
	void makeKnown();
	void tabChanged(int i);
	void on_questionBox_click(bool on);
	void on_list1_IndexChanged(int index);
	void on_list2_IndexChanged(int index);
	void showAboutForm();
	void speechOn(int);
	void selectVoice(int);
	void say();
	void clearMessage();
	void langChange();
	void setNext(unique_ptr<Entry>&);
	void getNext();

private:
	QLineEdit *word, *answer, *txtTotal, *txtKnown, *txtNoun,*txtAdj,*txtVerb,*txtAdv;
	QPushButton *btnNext, *btnShow, *btnKnow, *btnReset;
	QGroupBox *questionBox;
	QTabWidget* tab;
	QButtonGroup *btnGroup;
	QComboBox *list1, *list2, *voice;
	QProgressBar *bar;
	QAction *aAbout, *aQuit ;
	QCheckBox *chkSpeech;
	QToolButton *btnSay;
	QMessageBox* modalBox;
	Log *log;
	Stat* stat;
	Dict* dict;
	unique_ptr<std::thread> th;
	unique_ptr<Index> idx;
	unique_ptr<Entry> entry;
	unique_ptr<QTimer> timer;
	unique_ptr<Speech> speech;

};

#endif /* MAINWINDOW_H_ */
