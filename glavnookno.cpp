//includes generated by Qt Creator
#include "glavnookno.h"
#include "ui_glavnookno.h"
#include <QtMultimedia/QMediaPlaylist>
#include <QtMultimedia/QMediaPlayer>

glavnookno::glavnookno(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::glavnookno)
{
  timer=new QTimer(this);
  ui->setupUi(this);
  connect(ui->actionZacni_igro, SIGNAL(triggered()), this, SLOT(start()));
  connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(undo2()));
  connect(ui->volume, SIGNAL(clicked()), this, SLOT(setMusic()));
  this->igra = NULL;
  this->i1 = NULL;
  this->i2 = NULL;
  ch = new ClickHandler(this);
  igP = ui->igralnaP;
  //volume = ui->volume;
  for(int i = 0; i<6; i++)
    for(int j = 0; j<7; j++){
      QWidget* w = (QWidget*)ui->igralnaP->itemAtPosition(i, j+1)->widget();
      w->installEventFilter(ch);
    }
  statusLabel = new QLabel(this);
  statusLabel->setLayoutDirection(Qt::LeftToRight);
  ui->statusBar->addPermanentWidget(statusLabel);
  //timer:
  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()), this, SLOT(izpiscas()));
  z = NULL;
}

glavnookno::~glavnookno()
{
  //počisti pomnilnik ko zaključiš program
  if(igra != NULL){
    delete igra;
    igra = NULL;
  }
  if(i1 != NULL && i2 != NULL){
    delete i1;
    i1 = NULL;
    delete i2;
    i2 = NULL;
  }
  if(igP != NULL)
    igP = NULL;
  z = NULL;
  delete ui;
}

void glavnookno::start(){
  //ovde se pokliče funkcija za začetek nove igre
  if(i1 == NULL && i2 == NULL)
    vnosImen();
  if(igra != NULL){
    delete igra;
    igra = NULL;
  }
  igra = new Igra(i1, i2);
  if(z != NULL)
    igra->setNaVrsti(z);
  cas = new QTime(0,0,0);
  timer->start(1000);
  if(igra->locked())
    igra->lock();
  this->updateUi();
  this->clearPolje();
}

void glavnookno::izpiscas(){
  *cas = cas->addSecs(1);//tick
  ui->cas->setText(cas->toString("hh:mm:ss"));
}

//funkcija za vnos imen
void glavnookno::vnosImen(){
  QString imeI, imeII;
  QDialog *dI = new QDialog(0,0),
      *dII = new QDialog(0,0);
  Ui_vnosImena1 vi1;
  Ui_vnosImena2 vi2;
  vi1.setupUi(dI);
  dI->exec();
  imeI = vi1.txtIme->text();
  i1 = new Igralec(imeI, 1);
  vi2.setupUi(dII);
  dII->exec();
  imeII = vi2.txtIme->text();
  i2 = new Igralec(imeII, 2);
}

void glavnookno::setPolje(int x, int y, int i){
  QWidget *w = (QWidget*)ui->igralnaP->itemAtPosition(x, y+1)->widget();
  if(i == 1)
    w->setStyleSheet("border-image: url(:/img/tileR.png);");
  else if(i == 2)
    w->setStyleSheet("border-image: url(:/img/tileY.png);");
  else if(i==0){
    w->setStyleSheet("border-image: url(:/img/tile.png);\nbackground-color: rgba(255, 255, 255, 200);");
    igra->polje[x][y]=0;
  }
  this->updateUi();
}

void glavnookno::undo2(){
  int* u = this->igra->undo2();
  if(u[0] == 0)
    this->setPolje(u[1], u[2], u[0]);
  else
    QMessageBox::warning(this, "Napaka", "Ni možno izvesti undo", QMessageBox::Ok, QMessageBox::Cancel);
}

void glavnookno::setMusic(QMediaPlayer *mp, QMediaPlaylist *m){
    //QPushButton *p = (QPushButton*)ui->volume;
    QMessageBox *qmb = new QMessageBox();
    QMessageBox *qmb1 = new QMessageBox();
    QMessageBox *qmb2 = new QMessageBox();
    //qmb->setText("NIČ!");
    //qmb1->setText("TO OFF!");
    //qmb2->setText("TO ON!");
    mp->setPlaylist(m);
    mp->play();
    //QMediaPlayer *m = new QMediaPlayer();
    //if(volume->styleSheet() == "border-image: url(:/img/SoundON.png);")
    if(ui->volume->styleSheet() == "border-image: url(:/img/SoundON.png);")
    {
        ui->volume->setStyleSheet(styleSheet().append(QString("border-image: url(:/img/SoundON.png);")));
        //volume->setStyleSheet("border-image: url(:/img/SoundOFF.png);");
        //m->pause();
        mp->pause();
        //qmb1->show();
    }
    else if(ui->volume->styleSheet() == "border-image: url(:/img/SoundOFF.png);")
    {
        ui->volume->setStyleSheet(styleSheet().append(QString("border-image: url(:/img/SoundON.png);")));
        //volume->setStyleSheet("border-image: url(:/img/SoundON.png);");
        mp->play();
        //qmb2->show();
    }
    //else qmb->show();
    this->updateUi();
}

/*void glavnookno::setMusic(QMediaPlayer *mp, QMediaPlaylist *m){
    QPushButton *p = (QPushButton*)ui->volume;
    mp->setPlaylist(m);
    mp->play();
    //QMediaPlayer *m = new QMediaPlayer();
    if(p->clicked() && p->styleSheet()=="border-image: url(:/img/SoundON.png);")
    {
        p->setStyleSheet("border-image: url(:/img/SoundOFF.png);");
        //m->pause();
        mp->pause();
    }
    else if(p->clicked() && p->styleSheet()=="border-image: url(:/img/SoundOFF.png);")
    {
        p->setStyleSheet("border-image: url(:/img/SoundON.png);");
        mp->play();
    }
    this->updateUi();
}*/



void glavnookno::clearPolje(){
  for(int i = 0; i<6; i++)
    for(int j = 0; j<7; j++){
      QWidget* w = (QWidget*)ui->igralnaP->itemAtPosition(i, j+1)->widget();
      w->setStyleSheet("border-image: url(:/img/tile.png);\nbackground-color: rgba(255, 255, 255, 200);");
    }
}

void glavnookno::ilegalMove(){
  QMessageBox::warning(this, "Napaka", "Nedovoljena poteza", QMessageBox::Ok, QMessageBox::Cancel);
}

void glavnookno::updateUi(){
  ui->txtIgralec1->setText(i1->getIme());
  ui->txtIgralec2->setText(i2->getIme());
  ui->lblScore1->setText(QString::number(i1->getZmage()));
  ui->lblScore2->setText(QString::number(i2->getZmage()));
  statusLabel->setText("Na vrsti je: " + igra->getNaVrsti()->getIme());
}

void glavnookno::zmagovalec(int z){
  QDialog* d = new QDialog(0,0);
  igra->lock();
  if(z==0){
    Ui_izpis_neodloceno nUi;
    nUi.setupUi(d);
    int s = d->exec();
    if (s == QDialog::Accepted)
      this->start();
    else
      timer->stop();
  } else {
    updateUi();
    Ui_Izpis_zmagovalca zUi;
    zUi.setupUi(d);
    if(i1->getSt() == z){
        i1->zmaga();
        this->z = i1;
        zUi.lblWin->setText(zUi.lblWin->text() + " " + i1->getIme());
    } else {
        i2->zmaga();
        this->z = i2;
        zUi.lblWin->setText(zUi.lblWin->text() + " " + i2->getIme());
    }
    int s = d->exec();

    if (s == QDialog::Accepted)
      this->start();
    else {
      timer->stop();
      updateUi();
    }
  }
}

void glavnookno::on_undo_clicked()
{
 //naslov
 /*  int* ii=igra->undo();
     printf("%p\n",ii);
     setPolje();*/
     //printf("velikost  %d\n",igra->velikost());
     if(igra->velikost()>0){
         int ii=igra->undo();
         int jj=igra->undo();
         //printf("%d %d\n",ii,jj);
         setPolje(ii,jj,0);
         igra->getNaVrsti()->getSt();
      }
}
