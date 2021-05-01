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

#include "app/Index.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	QString version("");
#ifdef _VER
	version = _VER;
#endif

	QString file = QDir::homePath().append(QDir::separator()).append(".dualword/dualword.txt");
	QString index = QDir::homePath().append(QDir::separator()).append(".dualword/index-new");
	qDebug() << file;

	try {
		Index idx(index.toStdString(),Index::READ_WRITE);
		idx.init();

		int i=1;
		std::ifstream  data(file.toStdString());

		std::string line;
		while(std::getline(data,line))
		{
			QString str =QString::fromStdString(line);
			QStringList list = str.split(",");

			std::unique_ptr<Entry> e = std::unique_ptr<Entry>(new Entry());
			e->id( QString::number(i++).toStdString());
			e->word1(list.at(0).toStdString());
			e->word2(list.at(1).toStdString());
			e->lang1(list.at(2).toStdString());
			e->lang2(list.at(3).toStdString());
			QString s(list.at(4));

			if( QString::compare(s , "noun") == 0 ){
				e->epart(Entry::EPart::NOUN);
			}else if( QString::compare(s , "adjective") == 0 ){
				e->epart(Entry::EPart::ADJ);
			}else if( QString::compare(s , "verb") == 0 ){
				e->epart(Entry::EPart::VERB);
			}else if( QString::compare(s , "adverb") == 0 ){
				e->epart(Entry::EPart::ADV);
			}

			e->trans("");
			idx.save(e.get());
		}
		idx.close();

		if(version.length() <= 0)
			qDebug() << "Version number is not available.";

		QFile file(index.append(QDir::separator()).append(Index::FILE_VERSION.c_str()));
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
			qDebug() << "couldn't open file.";

		 QTextStream out(&file);
		 out << version;
		 out.flush();
		 file.close();

	} catch (exception& e) {
		qDebug() << e.what();
	}

}
