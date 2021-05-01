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

#ifndef DICT_H_
#define DICT_H_

#include "gui/form/Form.h"

class Dict : public Form {
	Q_OBJECT

public:
	Dict(QWidget *parent);
	virtual ~Dict();

protected:
	void showEvent(QShowEvent *e);

public Q_SLOTS:
	void addRows();

private:
	QLabel* lbl;
	QLineEdit* word;
	QPushButton* find;
	QTableWidget* table;
};

#endif /* DICT_H_ */
