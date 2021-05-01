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

#include "Entry.h"

Entry::Entry(){
	known = false;
	_correct = 0;
	_incorrect = 0;
	_redo = 0;
	_shown = 0;
	_part = "";
	_update = "";
};

void Entry::word2(std::string s){
	_word2 = s;
}

Entry::~Entry() {}

void Entry::fillVector(std::unique_ptr<Entry> &e){
	vector.push_back(std::move(e));
}

void Entry::shuffle(){
	std::random_shuffle ( vector.begin(), vector.end() );
}
