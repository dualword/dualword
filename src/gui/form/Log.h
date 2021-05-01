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

#ifndef LOG_H_
#define LOG_H_

#include "gui/form/Form.h"

class Log : public Form {
	Q_OBJECT

	static const int _max = 21;

public:
 	Log(QWidget *parent);
	virtual ~Log();

public Q_SLOTS:
	void newMessage(int, const QString& msg);

private:
	QListWidget *log;

};

#endif /* LOG_H_ */
