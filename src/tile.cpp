#include "tile.h"

Tile::Tile(int x, int y, int number, Board *mummy, QGraphicsScene *scene, QLabel *fromStatus)
	: QGraphicsRectItem(10 + x*110, 10 + y*110, 100, 100, 0, scene)
{
	text = new QGraphicsTextItem(QString::number(number), this, scene);
	text->setDefaultTextColor("#222222");
	text->setPos(10 + x*110, 10 + y*110);
	setBrush(QBrush(QColor("#aaccee"), Qt::SolidPattern));

	coords = QPoint(x,y);
	this->number = number;
	board = mummy;
	counter = fromStatus;
	
	timeline = new QTimeLine(300);
	timeline->setCurrentTime(300);
	timeline->setFrameRange(0,20);
	
	animation = NULL;
}

Tile::~Tile()
{
	delete timeline;
	delete animation;
}

void Tile::moveBy(QPoint direction)
{
	timeline->setCurrentTime(300);
	timeline->stop();
	
	if (animation != NULL)
		delete animation;
	
	animation = new QGraphicsItemAnimation;
	animation->setItem(this);
	animation->setTimeLine(timeline);

	animation->setPosAt(1, QPoint(x() + 110 * direction.x() * scale(), y() + 110 * direction.y() * scale()));

	timeline->start();
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
		changePosition(false, true);
}

QPoint Tile::getCoords()
{
	return coords;
}

int Tile::getNumber()
{
	return number;
}

void Tile::changePosition(bool backwards, bool clicked)
{
	if (clicked == true)
		board->nextMoves.clear();

	QPoint *direction = board->getDirection(coords);

	if (direction == NULL)
		return;

	moveBy(*direction);
	board->positionChanged(coords, *direction, backwards);

	coords.setX(coords.x() + direction->x());
	coords.setY(coords.y() + direction->y());
	delete direction;

	counter->setText("Number of moves: " + QString::number(board->previousMoves.size()));
	board->checkIfSolved();
}
