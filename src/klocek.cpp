#include "klocek.h"

klocek::klocek(int x, int y, int nr, plansza *mamusia, QGraphicsScene *scene, QLabel *zeStatusu)
	: QGraphicsRectItem(10 + x*110, 10 + y*110, 100, 100, 0, scene)
{
	napis = new QGraphicsTextItem(QString::number(nr), this, scene);
	napis->setDefaultTextColor("#222222");
	napis->setPos(10 + x*110, 10 + y*110);
	setBrush(QBrush(QColor("#aaccee"), Qt::SolidPattern));

	wsplrz = QPoint(x,y);
	this->nr = nr;
	rodzic = mamusia;
	licznik = zeStatusu;
	
	timeline = new QTimeLine(300);
	timeline->setCurrentTime(300);
	timeline->setFrameRange(0,20);
	
	anim = NULL;
}

klocek::~klocek()
{
	delete timeline;
	delete anim;
}

void klocek::przesun(QPoint kier)
{
	timeline->setCurrentTime(300);
	timeline->stop();
	
	if (anim != NULL)
		delete anim;
	
	anim = new QGraphicsItemAnimation;
	anim->setItem(this);
	anim->setTimeLine(timeline);

	anim->setPosAt(1, QPoint(x() + 110 * kier.x() * scale(), y() + 110 * kier.y() * scale()));

	timeline->start();
}

void klocek::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
		zmianaPozycji(false, true);
}

QPoint klocek::wspl()
{
	return wsplrz;
}

int klocek::numer()
{
	return nr;
}

void klocek::zmianaPozycji(bool cofniecie, bool klik)
{
	if (klik == true)
		rodzic->nastepne.clear();

	QPoint *kier = rodzic->czyMozna(wsplrz);

	if (kier == NULL)
		return;

	przesun(*kier);
	rodzic->przesunieto(wsplrz, *kier, cofniecie);

	wsplrz.setX(wsplrz.x() + kier->x());
	wsplrz.setY(wsplrz.y() + kier->y());
	delete kier;

	licznik->setText("Ilość wykonanych ruchów: " + QString::number(rodzic->poprzednie.size()));
	rodzic->spr();
}
