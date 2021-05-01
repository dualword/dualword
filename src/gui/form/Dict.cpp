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

#include "Dict.h"

Dict::Dict(QWidget *parent) : Form(parent){
	QGridLayout* grid = new QGridLayout();
	grid->setAlignment(Qt::AlignTop);
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 8);
    grid->setColumnStretch(2, 1);

	lbl = new QLabel("Word");
	word = new QLineEdit();
	find = new QPushButton("Find");
	table = new QTableWidget();
	table->setColumnCount(2);

	grid->addWidget(lbl,0,0);
	grid->addWidget(word,0,1);
	grid->addWidget(find,0,2);
	grid->addWidget(table,1,0,1,3);

	setLayout(grid);
    connect(find,SIGNAL(clicked()), this, SLOT(addRows()));
    connect(word, SIGNAL(returnPressed()), this, SLOT(addRows()));

}

Dict::~Dict() {}

void Dict::addRows(){
	if(word->text().trimmed().length()<=0) return;

	table->clear();
	vector<unique_ptr<Entry>> v;
	_idx->getList(word->text().toStdString(), v);
	table->setRowCount(v.size());

	for(int i=0;i < (int)v.size();i++){
		QTableWidgetItem *newItem = new QTableWidgetItem( QString::fromUtf8(v.at(i)->word2().c_str()) );
		table->setItem(i, 0, newItem);
		newItem = new QTableWidgetItem( QString::fromUtf8(v.at(i)->part().c_str()) );
		table->setItem(i, 1, newItem);
	}


}

void Dict::showEvent(QShowEvent *e){
	word->setFocus();
}
