#include "board.h"

Board::Board(int level)
{
	for (int i = 0, k = 1; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i != 3 || j != 3)
				tiles[j][i] = k++;

	tiles[3][3] = 0;
	emptyTileX = 3, emptyTileY = 3;

	switch (level) {
		case 1:
			mixTiles(30);
			break;
		case 2:
			mixTiles(80);
			break;
		case 3:
			mixTiles(130);
			break;
		case 4:
			randomizeTiles();
			break;

		default:
			break;
	}
}

void Board::mixTiles(int swapsCount)
{
	for (int i = 0; i < swapsCount; i++) {
		int right, down;
		do {
			int direction = rand() % 4;
			switch (direction) {
				case 0:
					right = -1, down = 0;
					break;
				case 1:
					right = 1, down = 0;
					break;
				case 2:
					down = -1, right = 0;
					break;
				case 3:
					down = 1, right = 0;
					break;

				default:
					break;
			}
		} while (emptyTileX + right < 0 || emptyTileX + right > 3
				 || emptyTileY + down < 0 || emptyTileY + down > 3);

		int newEmptyTileX = emptyTileX + right, newEmptyTileY = emptyTileY + down;

		std::swap(tiles[emptyTileX][emptyTileY], tiles[newEmptyTileX][newEmptyTileY]);
		emptyTileX = newEmptyTileX, emptyTileY = newEmptyTileY;

	}
}

void Board::randomizeTiles()
{
	int permutation[16];
	for (int i = 0; i < 16; i++)
		permutation[i] = i + 1;
	permutation[15] = 0;

	do {
		std::random_shuffle(permutation, permutation+15);
	} while (!isPermutationEven(permutation));

	for (int i = 0, k = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (permutation[k] == 0)
					emptyTileX = j, emptyTileY = i;

			tiles[j][i] = permutation[k++];
		}
}

bool Board::isPermutationEven(int *permutation)
{
	int inversions = 0;
	for (int i = 0; i < 14; i++)
		for (int j = i + 1; j < 15; j++)
			if (permutation[i] > permutation[j])
				inversions++;

	return inversions % 2 == 0;
}

int Board::getTileNumber(int i, int j)
{
	if (i < 0 || j < 0 || i > 3 || j > 3)
		return -1;
	return tiles[i][j];
}

QPoint * Board::getDirection(QPoint coords)
{
	if (emptyTileX == coords.x() - 1 && emptyTileY == coords.y())
		return new QPoint(-1, 0);
	else if (emptyTileX == coords.x() + 1 && emptyTileY == coords.y())
		return new QPoint(1, 0);
	else if (emptyTileX == coords.x() && emptyTileY == coords.y() - 1)
		return new QPoint(0, -1);
	else if (emptyTileX == coords.x() && emptyTileY == coords.y() + 1)
		return new QPoint(0, 1);

	return NULL;
}

void Board::positionChanged(QPoint tile, QPoint direction, bool backwards)
{
	std::swap(tiles[ tile.x() ][ tile.y() ],
			  tiles[ tile.x() + direction.x() ][ tile.y() + direction.y() ]);

	emptyTileX = tile.x();
	emptyTileY = tile.y();

	if (backwards == false)
		previousMoves.push(tiles[ tile.x() + direction.x() ][ tile.y() + direction.y() ]);
}

void Board::checkIfSolved()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if ((i != 3 || j != 3) && tiles[j][i] != 4*i + j + 1)
				return;

	emit theEnd();
}

int Board::getEmptyTileX()
{
	return emptyTileX;
}

int Board::getEmptyTileY()
{
	return emptyTileY;
}

unsigned long long int Board::getBoardState()
{
	unsigned long long int state = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			state = state << 4;
			state += tiles[j][i];
		}

	return state;
}

void Board::aStar()
{
	nextMoves.clear();
	boardStates.clear();

	BoardState *state;
	boardStatePointers.push(new BoardState(getBoardState(), NULL));
	boardStates.insert(boardStatePointers.top());

	while (!boardStates.empty()) {
		state = *boardStates.begin();
		boardStates.erase(boardStates.begin());

		if (state->isCorrect()) {
			boardStates.clear();
			break;
		}

		unsigned long long int newStates[4];
		newStates[0] = state->nextState(-1,0);
		newStates[1] = state->nextState(1,0);
		newStates[2] = state->nextState(0,-1);
		newStates[3] = state->nextState(0,1);

		for (int i = 0; i < 4; i++)
			if (newStates[i] != 0) {
				boardStatePointers.push(new BoardState(newStates[i], state));
				boardStates.insert(boardStatePointers.top());
			}
	}

	while (state->getParent() != NULL) {
		nextMoves.push(whatToMove(state->getParent(), state));
		state = state->getParent();
	}
	
	while (!boardStatePointers.empty())
		delete boardStatePointers.pop();
}

int Board::whatToMove(BoardState *oldBoardState, BoardState *newBoardState)
{
	int oldBoard[4][4], oldEmptyTileX, oldEmptyTileY, newBoard[4][4], newEmptyTileX, newEmptyTileY;
	unsigned long long int oldState = oldBoardState->getState();
	unsigned long long int newState = newBoardState->getState();

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			oldBoard[j][i] = (int)(oldState % 16);
			oldState = oldState >> 4;

			if (oldBoard[j][i] == 0)
				oldEmptyTileX = j, oldEmptyTileY = i;

			newBoard[j][i] = (int)(newState % 16);
			newState = newState >> 4;

			if (newBoard[j][i] == 0)
				newEmptyTileX = j, newEmptyTileY = i;
		}

	return oldBoard[newEmptyTileX][newEmptyTileY];
}

void Board::generateSolution()
{
	if (nextMoves.empty())
		aStar();
}
