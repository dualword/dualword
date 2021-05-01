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

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *p, Qt::WindowFlags f) : QMainWindow(p, f){
    setFixedSize(375, 475);

    statusBar();
    statusBar()->setSizeGripEnabled(false);

    idx = unique_ptr<Index>(new Index(Index::READ_ONLY));

    tab = new QTabWidget();

    QMenu *menu = menuBar()->addMenu( tr("&Main") );
    aQuit = new QAction("&Quit", this);
    menu->addAction(aQuit);

    QMenu* about = menuBar()->addMenu( tr("&Help") );
    aAbout = new QAction(tr("About"), this);
    about->addAction(aAbout);

	QGridLayout *grid = new QGridLayout();
	grid->setAlignment(Qt::AlignTop);

	btnSay = new QToolButton();
	btnSay->setText("&Say");
	btnSay->setToolTip("Say");
	btnSay->setEnabled(false);
	word = new QLineEdit();
	word->setReadOnly(true);
	grid->addWidget(new QLabel("Word:"),0,0);
	grid->addWidget(word,0,1);
	grid->addWidget(btnSay,0,2);

	answer = new QLineEdit;
	answer->setReadOnly(true);
	grid->addWidget(new QLabel("Answer:"),1,0);
	grid->addWidget(answer,1,1);

    //question
    questionBox = new QGroupBox(tr("Question")) ;
    questionBox->setCheckable(true);
    questionBox->setChecked(true);
    questionBox->setFlat(false);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setDirection(QBoxLayout::TopToBottom);
    btnGroup = new QButtonGroup();
    createRadio(btnGroup,vbox);
    vbox->addStretch(1);
	questionBox->setLayout(vbox);
    grid->addWidget(questionBox,2,0,1,3);

	//buttons
	QHBoxLayout *hlay = new QHBoxLayout;
    btnShow = new QPushButton("Show &Answer");
    btnShow->setEnabled(false);
    hlay->addWidget(btnShow);

    btnNext = new QPushButton("&Next");
    //btnNext->setEnabled(false);
    hlay->addWidget(btnNext);

    btnKnow = new QPushButton("I &Know this one");
    btnKnow->setEnabled(false);
    hlay->addWidget(btnKnow);
    grid->addLayout(hlay,3,0,1,3);

    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 8);
    grid->setColumnStretch(2, 1);

    QWidget *w = new QWidget;
    w->setLayout(grid);
    tab->insertTab(1,w, tr("Words"));

    // tab 2
    QGridLayout *form2 = new QGridLayout;
    form2->setAlignment(Qt::AlignTop);

    list1 = new QComboBox();
    list2 = new QComboBox();

    int j = Index::lang.size();
    for(int i=0;i<j;i++){
    	QString s = QString::fromUtf8(Index::lang.at(i).c_str());
    	list1->insertItem(i,s);
    	QString s2 = QString(s);
    	list2->insertItem(i,s2);
    }
    form2->addWidget(new QLabel("Language:"),0,0);
    form2->addWidget(list1,0,1);
    form2->addWidget(list2,0,2);

    QGroupBox *statBox = new QGroupBox(tr("Statistics")) ;
    QGridLayout *statGrid = new QGridLayout;
    statGrid->setAlignment(Qt::AlignTop);

    txtTotal = new QLineEdit();
    txtTotal->setReadOnly(true);
    txtTotal->setAlignment(Qt::AlignRight);

    txtKnown = new QLineEdit();
    txtKnown->setReadOnly(true);
    txtKnown->setAlignment(Qt::AlignRight);

    statGrid->addWidget(new QLabel("Total:"),0,0);
    statGrid->addWidget(txtTotal,0,1);
    statGrid->addWidget(new QLabel("Known:"),0,2);
    statGrid->addWidget(txtKnown,0,3);

    txtNoun = new QLineEdit();
    txtNoun->setAlignment(Qt::AlignRight);
    txtAdj = new QLineEdit();
    txtAdj->setAlignment(Qt::AlignRight);
    txtVerb = new QLineEdit();
    txtVerb->setAlignment(Qt::AlignRight);
    txtAdv = new QLineEdit();
    txtAdv->setAlignment(Qt::AlignRight);

    statGrid->addWidget(new QLabel("Noun:"),1,0);
    statGrid->addWidget(txtNoun,1,1);
    statGrid->addWidget(new QLabel("Adj:"),1,2);
    statGrid->addWidget(txtAdj,1,3);
    statGrid->addWidget(new QLabel("Verb:"),1,4);
    statGrid->addWidget(txtVerb,1,5);
    statGrid->addWidget(new QLabel("Adv:"),1,6);
    statGrid->addWidget(txtAdv,1,7);

    bar = new QProgressBar();
    bar->setMinimum(0);
    bar->setMaximum(100);
    bar->setValue(0);
    btnReset = new QPushButton("Reset");

    statGrid->addWidget(new QLabel("Progress:"),2,0,1,2);
    statGrid->addWidget(bar,2,2,1,4);
    statGrid->addWidget(btnReset,2,6,1,2);

    statBox->setLayout(statGrid);
    form2->addWidget(statBox,1,0,1,3);

    QGroupBox *speechBox = new QGroupBox(tr("Speech")) ;
    QHBoxLayout *qh = new QHBoxLayout;
    chkSpeech = new QCheckBox("Turn the speech on");
    chkSpeech->setChecked(false);
    qh->addWidget(chkSpeech);
    voice = new QComboBox();
    voice->setEnabled(false);
    qh->addWidget(voice);
    speechBox->setLayout(qh);
    form2->addWidget(speechBox,3,0,1,3);

    QWidget *w2 = new QWidget;
    w2->setLayout(form2);
    tab->insertTab(2,w2, tr("Settings"));

    dict = new Dict(this);
    tab->insertTab(3,dict, tr("Dictionary"));

    stat = new Stat(this);
    tab->insertTab(4,stat, tr("Statistics"));

    log = new Log(this);
    tab->insertTab(5,log, tr("Log"));

    tab->setEnabled(true);
    setCentralWidget(tab);

    modalBox = new QMessageBox(this);
    modalBox->setModal(true);
	modalBox->setIcon(QMessageBox::Information);
	modalBox->setWindowTitle(QApplication::applicationName());
	modalBox->setStandardButtons(QMessageBox::NoButton);

    setSlots();

}

void MainWindow::init() {
    stat->setIndex(idx.get());
    dict->setIndex(idx.get());

	list1->setCurrentIndex((int)idx->lang1());
	list2->setCurrentIndex((int)idx->lang2());

	Q_EMIT Next();
}

MainWindow::~MainWindow() {}

void MainWindow::createRadio(QButtonGroup* group, QBoxLayout* lay){
	for(int i=0;i<9;i++){
		QRadioButton* radio = new QRadioButton("");
	    btnGroup->addButton(radio,i);
		lay->addWidget(radio,0,Qt::AlignTop);
		radio->setVisible(false);
	}
}

void MainWindow::getNext() {
	clear();
	Q_EMIT Next();
}

void MainWindow::showAnswer() {
	answer->setText( QString::fromUtf8(entry->word2().c_str()) );

	btnShow->setEnabled(false);

	if(questionBox->isChecked()){
		for(int j=0;j<btnGroup->buttons().size();j++){
			if(btnGroup->button(j)->isChecked()){
				idx->processAnswer( entry.get(), entry->getVector().at(j).get() );
			}
			btnGroup->button(j)->setEnabled(false);
			if(btnGroup->button(j)->text().compare(QString::fromUtf8(entry->word2().c_str())) == 0 ){
				QFont qf;
				qf.setBold(true);
				btnGroup->button(j)->setFont(qf);
			}
		}
	}
	questionBox->setEnabled(false);
}

void MainWindow::makeKnown() {
	idx->makeKnown(entry.get());
	btnKnow->setEnabled(false);
	btnShow->setEnabled(false);
}

void MainWindow::refreshTab1(){
	int total = idx->getTotal();
	int count = idx->recordCount(Index::F_KNOWN,Index::F_KNOWN);
	txtTotal->setText(QString::number(total));
	txtKnown->setText(QString::number(count));
	if(total != 0){
		bar->setValue((count*100)/total);
	}else{
		bar->setValue(0);
	}
	string s = QString::number((int)Entry::EPart::NOUN).toStdString();
	txtNoun->setText(QString::number(idx->recordCount(Index::F_PART,s)));
	s = QString::number((int)Entry::EPart::ADJ).toStdString();
	txtAdj->setText(QString::number(idx->recordCount(Index::F_PART,s)));
	s = QString::number((int)Entry::EPart::VERB).toStdString();
	txtVerb->setText(QString::number(idx->recordCount(Index::F_PART,s)));
	s = QString::number((int)Entry::EPart::ADV).toStdString();
	txtAdv->setText(QString::number(idx->recordCount(Index::F_PART,s)));

}

void MainWindow::tabChanged(int i){
	if(i == 0){
		//getNext();
	}else if(i == 1){
		refreshTab1();
	}else if(i == 2){
		//stat
	}
}

void MainWindow::reset(){
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setWindowTitle(QApplication::applicationName());
	msgBox.setText(tr("Do you really want to reset? All current statistics will be erased."));
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();
	if(ret==QMessageBox::Cancel) return;
	th.release();
	th = unique_ptr<std::thread>(new std::thread(&Index::reset,idx.get()));
	th->detach();
}

void MainWindow::on_questionBox_click(bool on){
	if(!entry) return;

	if(on){
		showRadio();
	}else{
		for(int j=0;j<btnGroup->buttons().size();j++){
			btnGroup->buttons().at(j)->setVisible(false);
		}
	}
}

void MainWindow::showRadio(){
	vector<string>::const_iterator i;
	const vector<unique_ptr<Entry>>& v = entry->getVector();

	btnGroup->setExclusive(false);
	for(int j=0;j<btnGroup->buttons().size();j++){
		QRadioButton* b = dynamic_cast<QRadioButton*> ( btnGroup->button(j));
		b->setChecked(false);
		b->setVisible(false);
		b->setEnabled(true);
	}
	btnGroup->setExclusive(true);

	if(v.size() > 0){
		int size = (int)btnGroup->buttons().size();
		for(int j=0;j<size;j++){
			if(j < (int)v.size()){
				btnGroup->buttons().at(j)->setVisible(true);
				btnGroup->buttons().at(j)->setText(QString::fromUtf8(v.at(j)->word2().c_str()));
				QFont qf;
				qf.setBold(false);
				btnGroup->buttons().at(j)->setFont(qf);
			}
		}
	}
}

void MainWindow::on_list1_IndexChanged(int index){
	idx->lang1((Entry::ELang)list1->currentIndex());
	if(list1->currentIndex() == 0){
		list2->setCurrentIndex(1);
	}else{
		list2->setCurrentIndex(0);
	}
	speechOn(1);
}

void MainWindow::on_list2_IndexChanged(int index){
	idx->lang2((Entry::ELang)list2->currentIndex());
	if(list1->currentIndex() == list2->currentIndex()){
		if(list1->currentIndex() == 0){
			list2->setCurrentIndex(1);
		}else{
			list2->setCurrentIndex(0);
		}
	}
}

void MainWindow::showAboutForm()
{
	QString str;
	str.append(qApp->applicationName());
	str.append(" ").append(qApp->applicationVersion()).append("<br>");
	str.append("License: GPL v3 <br/>");
	str.append("Website: <a href='http://github.com/dualword'>dualword</a> <br/>");
	str.append("&copy;2012 Alexander Busorgin <br/>");
	QMessageBox::about(this, tr("About"), str );
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case (Qt::Key_N):
			if(btnNext->isEnabled()) getNext();
			break;
		case (Qt::Key_A):
			if(btnShow->isEnabled()) showAnswer();
			break;
		case (Qt::Key_K):
			if(btnKnow->isEnabled()) makeKnown();
			break;
		case (Qt::Key_S):
			if(btnSay->isEnabled()) say();
			break;
		default: QWidget::keyPressEvent(event); break;
	}
}

void MainWindow::speechOn(int i){
	if(!speech.get()){
		try{
			speech = unique_ptr<Speech>(new Speech());
			speech->init();
		}catch (...) {
			showBox("Couldn't load Festival speech engine.");
		}
	}

	if(chkSpeech->isChecked()){
		voice->setEnabled(true);
		voice->clear();
		unique_ptr<vector<string>> v = Speech::getVoices(list1->currentText().toStdString());
		for(int i=0; i < (int)v->size(); i++){
			voice->addItem(v->at(i).c_str());
		}
		btnSay->setEnabled(true);
	}else{
		voice->clear();
		voice->setEnabled(false);
		btnSay->setEnabled(false);
	}
}

void MainWindow::selectVoice(int i){
	if(!chkSpeech->isChecked()) return;
	if(voice->count()<=1 || voice->currentIndex()==0) return;
	speech->setVoice( voice->currentText().toStdString());
}

void MainWindow::say(){
	if(chkSpeech->isChecked() && voice->count()>1
			&& voice->currentIndex()>0 && word->text().length()>0)
		speech->say(entry->word1().c_str());
}

void MainWindow::askRecreate(const QString& msg){
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle(QApplication::applicationName());
	msgBox.setText(msg);
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Ok);
	int ret = msgBox.exec();
	if(ret==QMessageBox::Cancel){
		Q_EMIT quit();
	}else{
		Q_EMIT recreate();
	}
}

void MainWindow::newMessage(int ms, const QString& msg){
	if(timer.get()){
		timer->stop();
		timer.reset();
	}
	if(ms >= 0){
		timer = unique_ptr<QTimer>(new QTimer());
		timer->setSingleShot(true);
		timer->setInterval(ms);
		QObject::connect(timer.get(), SIGNAL(timeout()), this, SLOT(clearMessage()));
		timer->start();
	}
	statusBar()->showMessage(msg);
}
void MainWindow::clearMessage(){
	statusBar()->showMessage("");
}

void MainWindow::showBox(const QString& msg){
	QMessageBox::critical(this, "Error", msg);
}

void MainWindow::langChange(){
	setWindowTitle( QApplication::applicationName() + " - " + list1->currentText() + "/" + list2->currentText() );
	refreshTab1();
	clear();
}

void MainWindow::showModal(){
	modalBox->setText("Please, wait...");
	modalBox->show();

}

void MainWindow::hideModal(){
	refreshTab1();
	modalBox->hide();
}

void MainWindow::setNext(unique_ptr<Entry>& e){
	entry.reset();
	entry = std::move(e);

	word->setText(QString::fromUtf8(entry->word1().c_str()));
	btnShow->setEnabled(true);
	btnKnow->setEnabled(true);
	btnNext->setEnabled(true);
	answer->clear();

	questionBox->setEnabled(true);
	if(questionBox->isChecked()){
		showRadio();
	}
	say();
}

void MainWindow::clear(){
	entry.reset();

	btnGroup->setExclusive(false);
	for(int j=0;j<btnGroup->buttons().size();j++){
		QRadioButton* b = dynamic_cast<QRadioButton*> ( btnGroup->button(j));
		b->setChecked(false);
		b->setVisible(false);
		b->setEnabled(true);
	}
	btnGroup->setExclusive(true);
	word->clear();
	answer->clear();

	btnShow->setEnabled(false);
	btnKnow->setEnabled(false);
	//btnNext->setEnabled(false);

}

void MainWindow::closeEvent(QCloseEvent *event){
	doExit();
    event->accept();
}

void MainWindow::doExit(){
	idx->close();
	qApp->quit();
}

