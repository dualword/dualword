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

#ifndef ENTRY_H_
#define ENTRY_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

class Entry {

public:
	enum class ELang{en,ru,es,fr,de};
	enum class EPart{NOUN,ADJ,VERB,ADV};

	Entry();
	virtual ~Entry();
	void id(string s){_id = s;}
	const string& id() const{return _id;}
	void update(string s){_update = s;}
	const string& update() const{return _update;}
	bool isKnown(){return known;}
	void setKnown(bool b){known = b;}
	void word1(string s){_word1 = s;}
	const string& word1() const {return _word1;}
	void word2(string s);
	const string& word2() const{return _word2;}
	void lang1(string s){_lang1 = s;}
	const string& lang1() {return _lang1;}
	void lang2(string s){_lang2 = s;}
	const string& lang2() {return _lang2;}
	const int& correct() const {return _correct;}
	void correct(const int& i){_correct = i;}
	const int& incorrect() const {return _incorrect;}
	void incorrect(const int& i){_incorrect = i;}
	const int& shown() const {return _shown;}
	void shown(const int& i){_shown = i;}
	const int& redo() const {return _redo;}
	void redo(const int& i){_redo = i;}
	void part(std::string s){_part = s;}
	const string& part() const {return _part;}
	void epart(const EPart& s){_epart = s;}
	EPart epart() {return _epart;}
	void trans(string s){_trans = s;}
	const string& trans() const {return _trans;}
	const std::vector<std::unique_ptr<Entry>>& getVector() const{return vector;} ;
	void fillVector(std::unique_ptr<Entry>& e);
	void shuffle();

	void toString();
private:
	string _id;
	string _word1;  //word
	string _word2;  //answer
	string _lang1; //from
	string _lang2; //to
	string _update;
	bool known;
	int _correct;
	int _incorrect;
	int _shown; //total shown
	//shown in choice, but answer is wrong
	int _redo;
	string _part;
	EPart _epart;
	string _trans;

	string eng;
	std::vector<unique_ptr<Entry>> vector;

};

#endif /* ENTRY_H_ */
