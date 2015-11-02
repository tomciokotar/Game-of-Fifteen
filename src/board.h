#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <QPoint>
#include <QStack>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include "boardState.h"

class Board : public QObject
{
	Q_OBJECT

	int tiles[4][4];
	int emptyTileX, emptyTileY;
	std::set<BoardState *, cmp> boardStates;
	QStack<BoardState *> boardStatePointers;

public:
	QStack<int> previousMoves, nextMoves;

	Board(int);

	int getEmptyTileX();
	int getEmptyTileY();
	unsigned long long int getBoardState();
	void aStar();
	void mixTiles(int);
	void randomizeTiles();
	bool isPermutationEven(int *);
	int getTileNumber(int, int);
	void checkIfSolved();
	int whatToMove(BoardState *, BoardState *);
	void generateSolution();

	QPoint * getDirection(QPoint);
	void positionChanged(QPoint, QPoint, bool);

signals:
	void theEnd();
};

#endif // BOARD_H
