#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <QLabel>
#include <cstdlib>

class BoardState : public QObject
{
	unsigned long long int state;
	int stepsCount, heuristicsResult;
	BoardState *parent;

public:
	BoardState(unsigned long long int, BoardState *);
	int getStepsCount();
	void doHeuristics();
	int getHeuristicsResult();
	unsigned long long int getState();
	BoardState * getParent();
	bool isCorrect();
	unsigned long long int nextState(int, int);
};

struct cmp
{
	bool operator()(BoardState *stateA, BoardState *stateB)
	{
		int distanceA = stateA->getHeuristicsResult(), distanceB = stateB->getHeuristicsResult();
		if (distanceA != distanceB)
			return distanceA < distanceB;
		return stateA->getState() < stateB->getState();
	}
};

#endif // BOARDSTATE_H
