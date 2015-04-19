#ifndef GRAFIKA_H
#define GRAFIKA_H

#include <QGraphicsView>
#include <QMouseEvent>

class grafika : public QGraphicsView
{
protected:
	void mousePressEvent(QMouseEvent *);
};

#endif // GRAFIKA_H
