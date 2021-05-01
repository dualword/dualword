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

#include "Speech.h"

Speech::Speech(QObject* parent) : QObject(parent) {


}

Speech::~Speech() {
	close();
}

void Speech::init(){
	try {
		int heap_size = 210000;
		int load_init_files = 1;
		festival_initialize(load_init_files,heap_size);
	} catch (...) {
		throw runtime_error("Couldn't load Festival.");
	}
}

void Speech::close(){
	festival_tidy_up();
}

void Speech::say(const std::string& s){
	EST_String w(s.c_str());
	int ret = festival_say_text(w);
	ret++;
}

void Speech::setVoice(const std::string& str){
	EST_String w(("(voice_"+str+")").c_str());
    festival_eval_command(w);
    voice=str;
}

unique_ptr<vector<string>> Speech::getVoices(const string& name){
	unique_ptr<vector<string>> v = unique_ptr<vector<string>>(new vector<string>());
	v->push_back("Choose voice");
    QDir dir(QString("/usr/share/festival/voices/").append(QString(name.c_str())));
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
    	v->push_back(fileInfo.fileName().toStdString());
    }
    if(v->size() == 1) v->at(0)="Not found";

	return v;
}

