#ifndef PLANSZA_H
#define PLANSZA_H

#include <QGraphicsScene>
#include <QPoint>
#include <QStack>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include "stanplanszy.h"

class plansza : public QObject
{
	Q_OBJECT

	int klocki[4][4];
	int px, py;
	std::set<stanPlanszy *, cmp> plansze;
	QStack<stanPlanszy *> wskazniki;

public:
	QStack<int> poprzednie, nastepne;

	plansza(int);

	int pustyX();
	int pustyY();
	unsigned long long int zLongLonguj();
	void aGwiazdka();
	void zamien(int);
	void randomuj();
	bool parzyste(int *);
	int numerKlocka(int, int);
	void spr();
	int coPrzesunac(stanPlanszy *, stanPlanszy *);
	void genRozw();

	QPoint * czyMozna(QPoint);
	void przesunieto(QPoint, QPoint, bool);

signals:
	void koniec();
};

#endif // PLANSZA_H
