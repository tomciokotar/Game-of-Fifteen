#ifndef TILE_H
#define TILE_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPoint>
#include <QTimeLine>
#include <QLabel>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemAnimation>
#include "board.h"


class Tile : public QGraphicsRectItem
{
	QGraphicsTextItem *text;
	QPoint coords;	// wsplrz
	Board *board;
	QLabel *counter;
	QGraphicsItemAnimation *animation;
	int number;	// nr

public:
	QTimeLine *timeline;
	
	Tile(int, int, int, Board *, QGraphicsScene *, QLabel *);
	~Tile();

	void moveBy(QPoint);
	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void changePosition(bool, bool);

	QPoint getCoords();	// wspl
	int getNumber();	// numer
};

#endif // TILE_H

