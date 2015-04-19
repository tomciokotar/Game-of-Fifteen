#ifndef STANPLANSZY_H
#define STANPLANSZY_H

#include <QLabel>
#include <cstdlib>

class stanPlanszy : public QObject
{
	unsigned long long int stan;
	int ilekrokow, heur;
	stanPlanszy *rodzic;

public:
	stanPlanszy(unsigned long long int, stanPlanszy *);
	int kroki();
	void heuruj();
	int heureza();
	unsigned long long int stanPl();
	stanPlanszy * tata();
	bool poprawna();
	unsigned long long int kolejnyStan(int, int);
};

struct cmp
{
	bool operator()(stanPlanszy *a, stanPlanszy *b)
	{
		int odla = a->heureza(), odlb = b->heureza();
		if (odla != odlb)
			return odla < odlb;
		return a->stanPl() < b->stanPl();
	}
};

#endif // STANPLANSZY_H
