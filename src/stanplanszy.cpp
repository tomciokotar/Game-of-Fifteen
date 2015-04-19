#include "stanplanszy.h"

stanPlanszy::stanPlanszy(unsigned long long int st, stanPlanszy *rodz)
{
	stan = st;
	rodzic = rodz;

	if (rodz == NULL)
		ilekrokow = 0;
	else
		ilekrokow = rodz->kroki() + 1;

	heuruj();
}

int stanPlanszy::kroki()
{
	return ilekrokow;
}

void stanPlanszy::heuruj()
{
	heur = ilekrokow + 1;

	int kl[4][4];
	unsigned long long int ll = stan;

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			kl[j][i] = (int)(ll % 16);
			ll = ll >> 4;
		}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (kl[j][i] != 0)
				heur += 2*abs( (kl[j][i] - 1)/4 - i ) + 2*abs( (kl[j][i] - 1)%4 - j );
}

int stanPlanszy::heureza()
{
	return heur;
}

unsigned long long int stanPlanszy::stanPl()
{
	return stan;
}

bool stanPlanszy::poprawna()
{
	return ilekrokow + 1 == heur;
}

unsigned long long int stanPlanszy::kolejnyStan(int x, int y)
{
	int kl[4][4], px, py;
	unsigned long long int ll = stan;

	for (int i = 3; i >= 0; i--)
		for (int j = 3; j >= 0; j--) {
			kl[j][i] = (int)(ll % 16);
			ll = ll >> 4;

			if (kl[j][i] == 0)
				px = j, py= i;
		}

	if (px + x < 0 || px + x > 3 || py + y < 0 || py + y > 3)
		return 0;

	std::swap(kl[px][py], kl[px + x][py + y]);

	ll = 0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			ll = ll << 4;
			ll += kl[j][i];
		}

	return ll;
}

stanPlanszy * stanPlanszy::tata()
{
	return rodzic;
}
