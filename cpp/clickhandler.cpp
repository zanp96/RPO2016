#include "clickhandler.h"

ClickHandler::ClickHandler(QObject *parent) : QObject(parent)
{
}

//Ta event filter dela samo če event sproži igralno polje, drugače nima funkcijo
bool ClickHandler::eventFilter(QObject *obj, QEvent *event){
  int i = 0, x = 0, y = 0, xs = 0, ys = 0, stv = 5, sts = 6;
  if(event->type() == QEvent::MouseButtonRelease){
    glavnookno *w = (glavnookno*)this->parent();//referenca na glavno okno, lažji dostop
    QGridLayout *g = w->igP;//referenca na igralno polje
    i = g->indexOf((QWidget *)obj);//obj je tipa QObject*, cast-ovanjem se spremeni v QWidget*
    int abc = &stv-&x;
    int abc2 = &sts-&y;
    //g->getItemPosition(i, &x, &y, &xs, &ys);//funkcija prevede indeks v pozicijo elementa v tabeli
    g->getItemPosition(i, &abc, &abc2, &xs, &ys);
    int* addr = w->igra->potez(abc2-1, w->igra->getNaVrsti()->getSt());//klic metode Igra::potez ki preko reference vključene v glavnookno
    if(addr[0] == 1 || addr[0] == 2){//če je potez pravilen, posodobljena različica vrne id igralca
      w->setPolje(addr[1], addr[2], addr[0]);//posodobi igralno polje
      if(w->igra->preveriZmaga(addr[1], addr[2], addr[0]))//če obstaja zmagovalec, izpiši ga
        w->zmagovalec(addr[0]);
      else if(w->igra->preveriNeodloceno())//če je nedoločeno, izpiši
        w->zmagovalec(0);
    } else { //če potez ni pravilen
      w->ilegalMove();
      //w->igra->potez(y-1, w->igra->getNaVrsti()->getSt());//v spremenjeni različici poteza se ta workaround ne uporablja
    }
    return true;
  } else {
    return QObject::eventFilter(obj, event);//Na vse druge evente se odzovi na običajen način
  }
}
