#include "plansza.h"

plansza::plansza(int poziom)
{
	for (int i = 0, k = 1; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (i != 3 || j != 3)
				klocki[j][i] = k++;

	klocki[3][3] = 0;
	px = 3, py = 3;

	switch (poziom) {
		case 1:
			zamien(30);
			break;
		case 2:
			zamien(80);
			break;
		case 3:
			zamien(200);
			break;
		case 4:
			randomuj();
			break;

		default:
			break;
	}
}

void plansza::zamien(int n)
{
	for (int i = 0; i < n; i++) {
		int wprawo, wdol;
		do {
			int r = rand()%4;
			switch (r) {
				case 0:
					wprawo = -1, wdol = 0;
					break;
				case 1:
					wprawo = 1, wdol = 0;
					break;
				case 2:
					wdol = -1, wprawo = 0;
					break;
				case 3:
					wdol = 1, wprawo = 0;
					break;

				default:
					break;
			}
		} while (px + wprawo < 0 || px + wprawo > 3
				 || py + wdol < 0 || py + wdol > 3);

		int npx = px + wprawo, npy = py + wdol;

		std::swap(klocki[px][py], klocki[npx][npy]);
		px = npx, py = npy;

	}
}

void plansza::randomuj()
{
	int tab[16];
	for (int i = 0; i < 16; i++)
		tab[i] = i + 1;
	tab[15] = 0;

	do {
		std::random_shuffle(tab, tab+15);
	} while (!parzyste(tab));

	for (int i = 0, k = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (tab[k] == 0)
					px = j, py = i;

			klocki[j][i] = tab[k++];
		}
}

bool plansza::parzyste(int *tab)
{
	int inw = 0;
	for (int i = 0; i < 14; i++)
		for (int j = i + 1; j < 15; j++)
			if (tab[i] > tab[j])
				inw++;

	return inw%2 == 0;
}

int plansza::numerKlocka(int i, int j)
{
	if (i < 0 || j < 0 || i > 3 || j > 3)
		return -1;
	return klocki[i][j];
}

QPoint * plansza::czyMozna(QPoint wspl)
{
	if (px == wspl.x() - 1 && py == wspl.y())
		return new QPoint(-1, 0);
	else if (px == wspl.x() + 1 && py == wspl.y())
		return new QPoint(1, 0);
	else if (px == wspl.x() && py == wspl.y() - 1)
		return new QPoint(0, -1);
	else if (px == wspl.x() && py == wspl.y() + 1)
		return new QPoint(0, 1);

	return NULL;
}

void plansza::przesunieto(QPoint klocek, QPoint kier, bool cofniecie)
{
	std::swap(klocki[ klocek.x() ][ klocek.y() ],
			  klocki[ klocek.x() + kier.x() ][ klocek.y() + kier.y() ]);

	px = klocek.x();
	py = klocek.y();

	if (cofniecie == false)
		poprzednie.push(klocki[ klocek.x() + kier.x() ][ klocek.y() + kier.y() ]);
}

void plansza::spr()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if ((i != 3 || j != 3) && klocki[j][i] != 4*i + j + 1)
				return;

	emit koniec();
}

int plansza::pustyX()
{
	return px;
}

int plansza::pustyY()
{
	return py;
}

unsigned long long int plansza::zLongLonguj()
{
	unsigned long long int ll = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			ll = ll << 4;
			ll += klocki[j][i];
		}

	return ll;
}

void plansza::aGwiazdka()
{
	nastepne.clear();
	plansze.clear();

	stanPlanszy *st;
	wskazniki.push(new stanPlanszy(zLongLonguj(), NULL));
	plansze.insert(wskazniki.top());

	while (!plansze.empty()) {
		st = *plansze.begin();
		plansze.erase(plansze.begin());

		if (st->poprawna()) {
			plansze.clear();
			break;
		}

		unsigned long long int noweStany[4];
		noweStany[0] = st->kolejnyStan(-1,0);
		noweStany[1] = st->kolejnyStan(1,0);
		noweStany[2] = st->kolejnyStan(0,-1);
		noweStany[3] = st->kolejnyStan(0,1);

		for (int i = 0; i < 4; i++)
			if (noweStany[i] != 0) {
				wskazniki.push(new stanPlanszy(noweStany[i], st));
				plansze.insert(wskazniki.top());
			}
	}

	while (st->tata() != NULL) {
		nastepne.push(coPrzesunac(st->tata(), st));
		st = st->tata();
	}
	
	while (!wskazniki.empty())
		delete wskazniki.pop();
}

int plansza::coPrzesunac(stanPlanszy *stary, stanPlanszy *nowy)
{
	int starapl[4][4], spx, spy, nowapl[4][4], npx, npy;
	unsigned long long int sll = stary->stanPl(), nll = nowy->stanPl();

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			starapl[j][i] = (int)(sll % 16);
			sll = sll >> 4;

			if (starapl[j][i] == 0)
				spx = j, spy = i;

			nowapl[j][i] = (int)(nll % 16);
			nll = nll >> 4;

			if (nowapl[j][i] == 0)
				npx = j, npy = i;
		}

	return starapl[npx][npy];
}

void plansza::genRozw()
{
	if (nastepne.empty())
		aGwiazdka();
}
