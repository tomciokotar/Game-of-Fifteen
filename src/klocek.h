#ifndef KLOCEK_H
#define KLOCEK_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPoint>
#include <QTimeLine>
#include <QLabel>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemAnimation>
#include "plansza.h"


class klocek : public QGraphicsRectItem
{
	QGraphicsTextItem *napis;
	QPoint wsplrz;
	plansza *rodzic;
	QLabel *licznik;
	QGraphicsItemAnimation *anim;
	int nr;

public:
	QTimeLine *timeline;
	
	klocek(int, int, int, plansza *, QGraphicsScene *, QLabel *);
	~klocek();

	void przesun(QPoint);
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void zmianaPozycji(bool, bool);

	QPoint wspl();
	int numer();
};

#endif // KLOCEK_H

