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

#include "TableModel.h"

TableModel::TableModel( Index *idx, QObject *parent)
	:  QAbstractTableModel(parent), _idx(idx) {

	sortfield=Index::FieldIndex::WORD1 ;
	sortorder=true;

}

TableModel::~TableModel() {

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _idx->getTotal();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
	if( !index.isValid() )
	    return QVariant();

    if (role == Qt::DisplayRole){
        unique_ptr<Entry> e = _idx->getEntrySorted(index.row(), sortfield,sortorder);
        if(index.column() == 0){
        	return QString::fromUtf8(e->word1().c_str());
        }else if(index.column() == 1){
        	return  QString::fromUtf8(e->word2().c_str());
        }else if(index.column() == 2){
        	return  e->isKnown() ? "x" : "";
        }else if(index.column() == 3){
        	return QString::number(e->shown());
        }else if(index.column() == 4){
        	return QString::number(e->correct());
        }else if(index.column() == 5){
        	return QString::number(e->incorrect());
        }
    }

    if (index.column() == 2 && role == Qt::TextAlignmentRole) {
    	return Qt::AlignCenter;
	}
//    else {
//		return QAbstractTableModel::data(index, role);
//	}

//	if (index.isValid() && index.column() == 1 && role == Qt::CheckStateRole) {
//		return  e->isKnown() ? Qt::Checked : Qt::Unchecked;
//	}


    return QVariant();

}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (role==Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Word");
            case 1:
                return tr("");
            case 2:
                return tr("Known");
            case 3:
                return tr("Shown");
            case 4:
                return tr("Correct");
            case 5:
                return tr("Incorrect");
            default:
                return QVariant();
        }
    }
    if(role == Qt::ToolTipRole && orientation == Qt::Horizontal){
        switch (section) {
            case 0:
                return tr("Word");
            case 1:
                return tr("Translation");
            case 2:
                return tr("Marked as Known");
            case 3:
                return tr("Shown");
            case 4:
                return tr("Correct Answers");
            case 5:
                return tr("Incorrect Answers");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

void TableModel::sort ( int column, Qt::SortOrder order ){

	Q_EMIT layoutAboutToBeChanged();
	if(order==Qt::SortOrder::AscendingOrder){
		sortorder=true;
	}else{
		sortorder=false;
	}
	switch (column){
		case 0: sortfield=Index::FieldIndex::WORD1; break;
		case 1: sortfield=Index::FieldIndex::WORD2; break;
		case 2: sortfield=Index::FieldIndex::KNOWN; break;
		case 3: sortfield=Index::FieldIndex::SHOWN; break;
		case 4: sortfield=Index::FieldIndex::CORRECT; break;
		case 5: sortfield=Index::FieldIndex::INCORRECT; break;
	}

	Q_EMIT layoutChanged();
}

Qt::ItemFlags TableModel::flags ( const QModelIndex & index ) const{
	Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.column() == 1)
		flags |= Qt::ItemIsUserCheckable;
	return flags;

}
