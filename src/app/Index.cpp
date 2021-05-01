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

#include "Index.h"

const string Index::F_ID = "XID";
const string Index::F_WORD1 = "XWORD1";
const string Index::F_WORD2 = "XWORD2";
const string Index::F_LANG1 = "XLANG1";
const string Index::F_LANG2 = "XLANG2";
const string Index::F_UPDATE = "XUPDATE";
const string Index::F_KNOWN = "XKNOWN";
const string Index::F_CORRECT = "XCORRECT";
const string Index::F_INCORRECT = "XINCORRECT";
const string Index::F_REDO = "XREDO";
const string Index::F_TRANS = "XTRANS";
const string Index::F_SHOWN = "XSHOWN";
const string Index::F_PART = "XPART";
const string Index::FILE_VERSION = "dualword.version";

//"Русский","Español","Français","Deutsch"
const vector<string> Index::lang ({"english","russian","spanish","french","german"});

MaxLimit::MaxLimit() : _choiceNum(4), _tmpCorrect(0), _tmpIncorrect(0){};
MaxLimit::~MaxLimit(){};

Index::Index(string dir, IndexMode mode, QObject* parent) : QObject(parent){
	_mode = mode;
	_lang1 = Entry::ELang::en;
	_lang2 = Entry::ELang::ru;
	_dir = dir.c_str();
	_dir_orig = "/usr/share/dualword/index";
}

Index::Index(IndexMode mode, QObject* parent) : QObject(parent) {
	_mode = mode;
	_lang1 = Entry::ELang::en;
	_lang2 = Entry::ELang::ru;
	_dir = QDir::homePath().append(QDir::separator()).append(".dualword/index").toStdString();
	_dir_orig = "/usr/share/dualword/index";
}

Index::~Index() {

}

void Index::init(){

	checkVersion();

	QString path (_dir.c_str());
	try {
		if(_mode == Index::READ_ONLY){
			db = unique_ptr<Xapian::WritableDatabase>(new Xapian::WritableDatabase(path.toStdString(), Xapian::DB_OPEN));
		}else if(_mode == Index::READ_WRITE){
			db = unique_ptr<Xapian::WritableDatabase>(new Xapian::WritableDatabase(path.toStdString(), Xapian::DB_CREATE_OR_OPEN));
		}
		Q_EMIT newMessage(5000,QString::fromStdString("Validating index..."));
		validate();
		Q_EMIT newMessage(5000,QString::fromStdString("Index initialized: "+_dir));
		Q_EMIT indexReady();
		return;
	}catch (Xapian::DatabaseLockError& e) {
		Q_EMIT newBox(QString("Couldn't open the index. Is another instance of DualWord running?"));
	}catch (Xapian::DatabaseOpeningError& e) {
		Q_EMIT requestRebuild(QString("Couldn't open index.").append(dir().c_str()).append(" Do you want to create a new one?"));
	}catch (Xapian::DatabaseCorruptError& e) {
		Q_EMIT requestRebuild(QString("Corrupted index.").append(dir().c_str()).append(" Do you want to create a new one?"));
	}catch (Xapian::Error& e) {
		Q_EMIT requestRebuild(QString("Index error:").append(QString(e.get_msg().c_str())));
	}catch (const index_exception& e) {
		Q_EMIT requestRebuild(QString(e.what()));
	}

	return;
}

void Index::validate(){
	int total = db->get_doccount();
	vector<string> v;
	v.push_back(F_ID);
	v.push_back(F_WORD1);
	v.push_back(F_WORD2);
	v.push_back(F_LANG1);
	v.push_back(F_LANG2);
	v.push_back(F_PART);

	for(int i=0;i<v.size();i++){
		int w=0;
		for(Xapian::TermIterator t = db->allterms_begin(v.at(i));
				t != db->allterms_end(v.at(i)); t++){
			w += t.get_termfreq();
		}
		if(w != total)
			Q_EMIT newBox(QString(("Index data is not right(Field:"+v.at(i)+")").c_str()));
	}

}

void Index::close() {
	try {
		if(db.get()) db->close();
	} catch (const Xapian::Error& err) {

	}
}

void Index::recreate(){
	try {

		QDir src(QString(_dir_orig.c_str()));
		QDir dest(QString(_dir.c_str()));
			if(!QDir(src).exists()){
				throw index_exception("Couldn't find original index at: "+_dir_orig);
			}

		if(QDir(dest).exists()){
			QFileInfoList list = QDir(dest).entryInfoList(QDir::Dirs |QDir::Files | QDir::NoDotAndDotDot);
			for(int i=0; i<list.count(); i++) {
				QFileInfo fi = list[i];
				if( fi.isFile() ) {
					QFile(fi.absoluteFilePath()).remove(); //bool ret =
				}
			}
		}else{
			QDir(QDir::homePath().append(QDir::separator())).mkdir(QString(".dualword"));
			QDir(QDir::homePath().append(QDir::separator())).mkdir(QString(".dualword/index"));
		}

		QFileInfoList list = src.entryInfoList(QDir::Dirs |QDir::Files | QDir::NoDotAndDotDot);
		for(int i=0; i<list.count(); i++) {
			QFileInfo fi = list[i];

			if( fi.isFile() ) {
				QFile::copy( fi.absoluteFilePath(), dest.absoluteFilePath(fi.fileName()) );//bool ret =
			}
		}
		Q_EMIT newMessage(5000,QString::fromStdString("Index recreated: "+_dir));
	}catch (exception& e) {
		sendError(e.what());
	}
	Q_EMIT reinit();
}

void Index::getNext() {
	unique_ptr<Entry> e;

	try {
		int total = getTotal();
	    Xapian::Enquire enquire(*db);
		enquire.set_sort_by_value(PART,false);
	    Xapian::QueryParser qp;
	    qp.add_prefix("id", F_ID);
	    qp.set_database(*db);

	    Xapian::Query query = getCommonQuery();
	    query = Xapian::Query(Xapian::Query::OP_AND_NOT, query, Xapian::Query(F_KNOWN+QString(F_KNOWN.c_str()).toLower().toStdString()));

	    enquire.set_query(query);
	    Xapian::MSet matches = enquire.get_mset(0, maxlimit.choiceNum());

	    if( matches.size() == 0 && total>0)
	    	throw index_exception("All words are marked as known.Reset to start over again.");

	    if( matches.size() == 0)
	    	throw index_exception("No records found for this language.");

		int j = rand() % matches.size();

		e = getEntry(matches[j].get_document());
		fillEntry(e.get());
		e->shown(e->shown() + 1 );
		save(e.get());
		Q_EMIT setNext(e);
	}catch (Xapian::Error& ex) {
		sendError("Couldn't get next word." + string(ex.get_msg()));
	}catch (exception& ex) {
		sendError("Couldn't get next word." + string(ex.what()));
	}
}

unique_ptr<Entry> Index::getRandom() {
	unique_ptr<Entry> e;

	int rows = getTotal();
	Xapian::Enquire enquire(*db);
	Xapian::QueryParser qp;
	qp.set_database(*db);
	Xapian::Query query = getCommonQuery();
	enquire.set_query(query);
	Xapian::MSet matches = enquire.get_mset(0, rows);

	int j = rand() % rows;
	e = getEntry(matches[j].get_document());

	return e;
}

unique_ptr<Entry> Index::getEntry(const string& field, const string& value) {
	unique_ptr<Entry> e;

	try {
	    Xapian::Enquire enquire(*db);
	    Xapian::QueryParser qp;
	    qp.add_prefix("id", field);
	    qp.set_database(*db);

	    Xapian::Query query = getCommonQuery();
	    query = Xapian::Query(Xapian::Query::OP_AND, query, field+(QString(value.c_str()).toLower().toStdString()));

	    enquire.set_query(query);
	    Xapian::MSet matches = enquire.get_mset(0, 1);
	    Xapian::MSetIterator i = matches.begin();

	    if(matches.size() == 1){
	    	e = getEntry(i.get_document());
	    }else{
	    	throw index_exception( "No Entry found for:" + field+(QString(value.c_str()).toLower().toStdString()) );
	    }
	}catch (exception& e) {
		sendError(e.what());
	}
	return e;
}

void Index::processAnswer(Entry* e, Entry* a){
	//correct
	if(QString::fromUtf8(e->word2().c_str()).compare(QString::fromUtf8(a->word2().c_str())) == 0 ){
		e->correct( e->correct() + 1 );
		maxlimit.incChoiceNum();
	}else{
		if(a->isKnown()){
			a->redo( a->redo() + 1 );
			if(a->redo() > maxlimit.REDOLIMIT) {
				a->redo(0);
				a->setKnown(false);
			}
		}
		a->incorrect( a->incorrect() + 1 );
		e->incorrect( e->incorrect() + 1 );
		maxlimit.decChoiceNum();
	}
	try {
		save(a);
		save(e);
	}catch (exception& e) {
		sendError(e.what());
	}
}

void Index::makeKnown(Entry* e){
	maxlimit.incChoiceNum();
	e->setKnown(true);
	try{
		save(e);
	}catch (exception& e) {
		sendError(e.what());
	}
}

int Index::getTotal(){
	try {
		Xapian::Enquire enquire(*db);
		Xapian::QueryParser qp;
		qp.set_database(*db);
		Xapian::Query query = getCommonQuery();
		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, 100000);

		return matches.size();
	}catch (exception& e) {
		sendError(e.what());
	}

	return -1;
}

int Index::recordCount(const string& field, const string& value){

	try {
	    Xapian::Enquire enquire(*db);
	    Xapian::QueryParser qp;
	    qp.set_database(*db);
	    Xapian::Query query = getCommonQuery();
	    query = Xapian::Query(Xapian::Query::OP_AND, query, field+(QString(value.c_str()).toLower().toStdString()));
	    enquire.set_query(query);
	    Xapian::MSet matches = enquire.get_mset(0, 100);
	    return matches.size();
	}catch (exception& e) {
		sendError(e.what());
	}

	return -1;
}

unique_ptr<Entry> Index::getEntry(const Xapian::Document& doc){
	unique_ptr<Entry> e = unique_ptr<Entry>(new Entry());
	e->id(doc.get_value(Index::ID));
	e->word1(doc.get_value(Index::WORD1));
	e->word2(doc.get_value(Index::WORD2));
	e->lang1(doc.get_value(Index::LANG1));
	e->lang2(doc.get_value(Index::LANG2));

	if(QString::fromStdString(F_KNOWN).compare(doc.get_value(Index::KNOWN).c_str()) == 0){
		e->setKnown(true);
	}

	e->correct(Xapian::sortable_unserialise(doc.get_value(CORRECT)));
	e->incorrect(Xapian::sortable_unserialise(doc.get_value(INCORRECT)));
	e->redo(Xapian::sortable_unserialise(doc.get_value(REDO)));
	e->trans(doc.get_value(Index::TRANS));
	e->shown(Xapian::sortable_unserialise(doc.get_value(SHOWN)));

	Entry::EPart p = (Entry::EPart)Xapian::sortable_unserialise(doc.get_value(PART));
	switch(p){
		case Entry::EPart::VERB : e->part("verb") ; break;
		case Entry::EPart::NOUN : e->part("noun") ; break;
		case Entry::EPart::ADJ : e->part("adjective") ; break;
		case Entry::EPart::ADV : e->part("adverb") ; break;
		default:throw index_exception("Field part can't be empty.");break;
	}
	e->epart(p);

	return e;

}

void Index::save(Entry* e){

	Xapian::TermGenerator indexer;
	Xapian::Document doc;

	indexer.set_document(doc);

	doc.add_value(ID,e->id());
	indexer.index_text(e->id(),1,F_ID);

	doc.add_value(WORD1,e->word1());
	indexer.index_text(e->word1(),1,F_WORD1);

	doc.add_value(WORD2,e->word2());
	indexer.index_text(e->word2(),1,F_WORD2);

	doc.add_value(LANG1,e->lang1());
	indexer.index_text(e->lang1(),1,F_LANG1);

	doc.add_value(LANG2,e->lang2());
	indexer.index_text(e->lang2(),1,F_LANG2);

	QDateTime now = QDateTime::currentDateTime();
	doc.add_value(UPDATE,Xapian::sortable_serialise(now.toUTC().toTime_t()));

	if(e->isKnown()){
		doc.add_value(KNOWN,F_KNOWN);
		indexer.index_text(F_KNOWN,1,F_KNOWN);
	}else{
		doc.add_value(KNOWN,"");
	}

	doc.add_value(CORRECT,Xapian::sortable_serialise(e->correct()));
	doc.add_value(INCORRECT,Xapian::sortable_serialise(e->incorrect()) );
	doc.add_value(REDO,Xapian::sortable_serialise(e->redo()) );
	doc.add_value(LENGTH,Xapian::sortable_serialise(e->word1().length()) );
	doc.add_value(PART,Xapian::sortable_serialise((int)e->epart()) );
	indexer.index_text(QString::number((int)e->epart()).toStdString(),1,F_PART);
	doc.add_value(TRANS,e->trans());
	doc.add_value(SHOWN,Xapian::sortable_serialise(e->shown()) );

	db->replace_document(F_ID+e->id(),doc);
	db->commit();

}

void Index::reset(){
	Q_EMIT newMessage(-1,"Resetting...");
    Q_EMIT resetStart();
	try {
		int rows =  getTotal();
		Xapian::Enquire enquire(*db);
		Xapian::Query query = getCommonQuery();
		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, rows);

	    for(int i=0;i<rows;i++){
	    	unique_ptr<Entry> e = getEntry(matches[i].get_document());
	    	e->setKnown(false);
	    	e->update("");
	    	e->shown(0);
	    	e->correct(0);
	    	e->incorrect(0);
	    	e->redo(0);
	    	save(e.get());
	    }
	    Q_EMIT resetDone();
	    Q_EMIT newMessage(5000,"Reset was done.");
	}catch (exception& e) {
	    Q_EMIT resetDone();
	    Q_EMIT sendError("Couldn't reset." + string(e.what()));
	}
}

void Index::fillEntry(Entry* entry){

	    Xapian::Enquire enquire(*db);

		//add answer
		unique_ptr<Entry> a = getEntry(F_ID,entry->id());
		entry->fillVector( a );

	    Xapian::Query query = getCommonQuery();
	    Xapian::Query q1 = Xapian::Query(Xapian::Query::OP_AND, query, Xapian::Query(F_PART+QString::number((int)entry->epart()).toStdString()));
	    Xapian::Query qor = Xapian::Query(Xapian::Query(F_ID+entry->id()));
	    qor = Xapian::Query(Xapian::Query::OP_OR, qor, Xapian::Query(F_WORD1+entry->word1()));
	    qor = Xapian::Query(Xapian::Query::OP_OR, qor, Xapian::Query(F_WORD2+entry->word2()));
	    Xapian::Query q = Xapian::Query(Xapian::Query::OP_AND_NOT, q1, qor);

	    enquire.set_query(q);
	    Xapian::MSet matches = enquire.get_mset(0, 10000);
	    Xapian::MSetIterator i = matches.begin();

		int num = maxlimit.nextChoiceNum();
		unsigned rows = matches.size();

		set<string> exist;
		for(int i = 0;i<num;i++){
			int j = rand() % rows ;
			unique_ptr<Entry> e = getEntry(matches[j].get_document());
			if(exist.count(e->word2())>0) continue;
			string s(e->word2());
			exist.insert(s);
			entry->fillVector(e);
		}
		entry->shuffle();
}

Xapian::Query Index::getCommonQuery(){
    Xapian::Query query(F_LANG1+QString::number((int)_lang1).toStdString());
    query = Xapian::Query(Xapian::Query::OP_AND, query, Xapian::Query(F_LANG2+QString::number((int)_lang2).toStdString()));
    return query;
}

unique_ptr<Entry> Index::getEntry(const string& id){

	unique_ptr<Entry> e;

	try {
		Xapian::Enquire enquire(*db);
		Xapian::QueryParser qp;
		qp.set_database(*db);

		Xapian::Query query = getCommonQuery();
		query = Xapian::Query(Xapian::Query::OP_AND, query, F_ID+(QString(id.c_str()).toLower().toStdString()));

		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, 1);
		Xapian::MSetIterator i = matches.begin();
		e = getEntry(i.get_document());

	}catch (exception& e) {
		sendError(e.what());
	}
	return e;

}

unique_ptr<Entry> Index::getEntrySorted(const int& num, const FieldIndex& sort, bool order){
	if( sort < 0 )
		throw index_exception("Couldn't get Entry object.Wrong parameter:"+num);

	unique_ptr<Entry> e;

	try {
		Xapian::Enquire enquire(*db);

		enquire.set_sort_by_value(sort,order);

		Xapian::QueryParser qp;
		qp.set_database(*db);

		Xapian::Query query = getCommonQuery();
		enquire.set_query(query);
		Xapian::MSet set = enquire.get_mset(0, 10000);

		if( (int)set.size() < num)
			throw index_exception("Couldn't get Entry object. Parameter:"+num);
		e = getEntry(set[num].get_document());

	}catch (exception& e) {
		sendError(e.what());
	}

	return e;
}

void Index::sendError(const string& msg){
    Q_EMIT newMessage(5000,QString::fromStdString(msg));
    Q_EMIT newBox(QString::fromStdString(msg));
}

void Index::getList(const string& w,vector<unique_ptr<Entry>>& v){
    Xapian::Enquire enquire(*db);
    Xapian::QueryParser qp;
    qp.set_database(*db);

    Xapian::Query query = getCommonQuery();
	query = Xapian::Query(Xapian::Query::OP_AND, query, F_WORD1+(QString(w.c_str()).toLower().toStdString()));
	enquire.set_query(query);
    Xapian::MSet matches = enquire.get_mset(0, 1000);
	Xapian::MSetIterator i = matches.begin();
	int rows = (int)matches.size();

	for(int i = 0;i<rows;i++){
		v.push_back(getEntry(matches[i].get_document()));
	}

}

void Index::checkVersion(){
	QString fname(_dir_orig.c_str());
	fname.append(QDir::separator()).append(FILE_VERSION.c_str());

	 QString ver_orig,ver_local;
	 try {
		ver_orig = getLine(fname,100);
		fname = _dir.c_str();
		fname.append(QDir::separator()).append(FILE_VERSION.c_str());
		ver_local = getLine(fname,100);
	} catch (exception& e) {
		Q_EMIT newMessage(5000,QString(e.what()));
	}

	Q_EMIT newMessage(5000,QString("Original index version:").append(ver_orig));
	Q_EMIT newMessage(5000,QString("Local index version:").append(ver_local));
}

QString Index::getLine(const QString& file, const int& length){
	 QString line;
	 QFile f(file);

	 if (f.open(QFile::ReadOnly)) {
		 QTextStream in(&f);
		 line = in.readLine(length);
	 }
	 f.close();

	return line;
}
