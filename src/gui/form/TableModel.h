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

#ifndef TABLEMODEL_H_
#define TABLEMODEL_H_

#include <QAbstractTableModel>
#include <QSize>
#include "app/Index.h"
#include "app/Entry.h"

class TableModel  : public QAbstractTableModel {
     Q_OBJECT

public:
	TableModel(Index *idx=0, QObject *parent=0);
	virtual ~TableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void sort ( int column, Qt::SortOrder order = Qt::AscendingOrder );
    Qt::ItemFlags flags ( const QModelIndex & index ) const;

private:
    Index* _idx;
	Index::FieldIndex sortfield;
	bool sortorder;

};

#endif /* TABLEMODEL_H_ */
