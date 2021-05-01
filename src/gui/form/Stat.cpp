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

#include "Stat.h"

Stat::Stat(QWidget *parent) : Form(parent){
	QGridLayout* _grid = new QGridLayout();
//	lang = new QComboBox();
//    for(int i=0;i<Index::lang.size();i++){
//    	QString s = QString::fromUtf8(Index::lang.at(i).c_str());
//    	lang->insertItem(i,s);
//    }
//	count = new QLabel();
	//	_grid->addWidget(lang,0,0);
	//	_grid->addWidget(count,0,1);


	table = new QTableView(this);
//	table->resizeColumnsToContents();
//	table->resizeRowsToContents();
///	table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
	table->verticalHeader()->hide();
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	_grid->addWidget(table,0,0);
	setLayout(_grid);


}

Stat::~Stat() {

}

void Stat::setIndex(Index* idx){
	Form::setIndex(idx);
	TableModel *model =new TableModel(idx, this);
	table->setModel(model);
	table->setSortingEnabled(true);

}


