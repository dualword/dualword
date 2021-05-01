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

#pragma once

#ifndef INDEX_H_
#define INDEX_H_

#include <xapian.h>

#include <stdexcept>
#include <exception>
#include <cstdlib>

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QTimer>

#include "app/Entry.h"

using namespace std;

class index_exception: public runtime_error
{
public:
	index_exception(const string& arg): runtime_error(arg) {}
};

class MaxLimit{
public:
	static const int CHOICEMAX=8;
	static const int CHOICEMIN=1;
	static const int tmpCOUNT=3;
	static const int REDOLIMIT=3;
public:
	MaxLimit();
	virtual ~MaxLimit();
	int choiceNum() const { return _choiceNum + 2;}
	int nextChoiceNum() const {return rand() % _choiceNum +1;}
	void incChoiceNum(){
		if(++_tmpCorrect >= tmpCOUNT){
			if(++_choiceNum > CHOICEMAX) _choiceNum = CHOICEMAX;
			_tmpCorrect = 0;
		}
	}
	void decChoiceNum(){
		if(++_tmpIncorrect >= tmpCOUNT){
			if(--_choiceNum < CHOICEMIN) _choiceNum = CHOICEMIN;
			_tmpIncorrect = 0;
		}
	}
private:
	int _choiceNum, _tmpCorrect, _tmpIncorrect;
};

class Index : public QObject{
  Q_OBJECT

public:
	static const vector<string> lang;

	enum IndexMode{READ_ONLY, READ_WRITE};
	enum FieldIndex{ID,WORD1,WORD2,LANG1,LANG2,UPDATE,KNOWN,CORRECT,INCORRECT,REDO,
		LENGTH,PART,TRANS,SHOWN};

	static const string F_ID,F_WORD1,F_WORD2,F_LANG1,F_LANG2,F_UPDATE,
		F_KNOWN,F_CORRECT,F_INCORRECT,F_REDO,F_TRANS,F_SHOWN,F_PART;

	static const string FILE_VERSION;

	Index(IndexMode mode, QObject* parent = 0);
	Index(string, IndexMode mode, QObject* parent = 0);
	virtual ~Index();
	const string dir() const {return _dir;};
	void processAnswer(Entry* e, Entry* a);
	unique_ptr<Entry> getEntry(const string& field, const string& value);
	unique_ptr<Entry> getEntry(const string&);
	unique_ptr<Entry> getEntrySorted(const int& ,const FieldIndex&, bool);
	int getTotal();
	int recordCount(const string& field, const string& value);
	void lang1(Entry::ELang l){_lang1 = l;Q_EMIT languageChange();}
	Entry::ELang lang1() {return _lang1;}
	void lang2(Entry::ELang l){_lang2 = l;Q_EMIT languageChange();}
	Entry::ELang lang2() {return _lang2;}
	void save(Entry* e);
	void getList(const string&, vector<unique_ptr<Entry>>&);

Q_SIGNALS:
	void newMessage(int, const QString& msg);
	void newBox(const QString& msg);
	void requestRebuild(const QString& msg);
	void indexReady();
	void reinit();
	void resetStart();
	void resetDone();
	void languageChange();
	void setNext(unique_ptr<Entry>&);

public Q_SLOTS:
	void init();
	void close();
	void recreate();
	void reset();
	void makeKnown(Entry*);
	void getNext();

private:
	unique_ptr<Entry> getRandom();
	unique_ptr<Entry> getEntry(const Xapian::Document& doc);
	Xapian::Query getCommonQuery();
	void fillEntry(Entry* e);
	void sendError(const string& msg);
	void checkVersion();
	QString getLine(const QString&, const int&);
	void validate();

private:
	unique_ptr<Xapian::WritableDatabase> db;
	IndexMode _mode;
	Entry::ELang _lang1;
	Entry::ELang _lang2;
	MaxLimit maxlimit;
	string _dir,_dir_orig;

};



#endif /* INDEX_H_ */

