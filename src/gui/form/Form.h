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

#ifndef FORM_H_
#define FORM_H_

#include "app/Index.h"
#include "app/Entry.h"

#include <memory>

#include <QtGui>

class Form : public QWidget {
     Q_OBJECT

public:
	Form(QWidget *parent = 0);
	virtual ~Form();
	void setIndex(Index* idx);

protected:
	Index* _idx;

};

#endif /* FORM_H_ */
