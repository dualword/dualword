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

#ifndef SPEECH_H_
#define SPEECH_H_

#include <festival.h>

#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <exception>

#include <QObject>
#include <QDir>
#include <QDebug>

using namespace std;

class Speech : public QObject{
	Q_OBJECT

public:
	Speech(QObject* parent = 0);
	virtual ~Speech();
	void init();
	void close();
	static unique_ptr<vector<string>> getVoices(const string&);

public:
	void say(const string& str);
	void setVoice(const string& str);

private:
	string voice;
};

#endif /* SPEECH_H_ */
