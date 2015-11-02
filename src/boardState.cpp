#include "boardState.h"

BoardState::BoardState(unsigned long long int givenState, BoardState *daddy)
{
	state = givenState;
	parent = daddy;

	if (daddy == NULL)
		stepsCount = 0;
	else
		stepsCount = daddy->getStepsCount() + 1;

	doHeuristics();
}

int BoardState::getStepsCount()
{
	return stepsCount;
}

void BoardState::doHeuristics()
{
	heuristicsResult = stepsCount + 1;

	int tiles[4][4];
	unsigned long long int localState = state;

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			tiles[j][i] = (int)(localState % 16);
			localState = localState >> 4;
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (tiles[j][i] != 0)
				heuristicsResult += 2*abs( (tiles[j][i] - 1)/4 - i ) + 2*abs( (tiles[j][i] - 1)%4 - j );
}

int BoardState::getHeuristicsResult()
{
	return heuristicsResult;
}

unsigned long long int BoardState::getState()
{
	return state;
}

bool BoardState::isCorrect()
{
	return stepsCount + 1 == heuristicsResult;
}

unsigned long long int BoardState::nextState(int x, int y)
{
	int tiles[4][4], emptyTileX, emptyTileY;
	unsigned long long int localState = state;

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			tiles[j][i] = (int)(localState % 16);
			localState = localState >> 4;

			if (tiles[j][i] == 0)
				emptyTileX = j, emptyTileY = i;
		}

	if (emptyTileX + x < 0 || emptyTileX + x > 3 || emptyTileY + y < 0 || emptyTileY + y > 3)
		return 0;

	std::swap(tiles[emptyTileX][emptyTileY], tiles[emptyTileX + x][emptyTileY + y]);

	localState = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			localState = localState << 4;
			localState += tiles[j][i];
		}

	return localState;
}

BoardState * BoardState::getParent()
{
	return parent;
}
