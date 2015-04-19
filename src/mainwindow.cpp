#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	setMinimumSize(450, 480);
	
	nowagra = menuBar()->addMenu("Nowa gra");
	opcje = menuBar()->addMenu("Opcje");
	pomoc = menuBar()->addMenu("Pomoc");

	latwy = new QAction("Łatwy", this);
	nowagra->addAction(latwy);
	connect(latwy, SIGNAL(triggered()), this, SLOT(genLatwy()));
	latwy->setShortcut(Qt::CTRL + Qt::Key_1);
	sredni = new QAction("Średni", this);
	nowagra->addAction(sredni);
	connect(sredni, SIGNAL(triggered()), this, SLOT(genSredni()));
	sredni->setShortcut(Qt::CTRL + Qt::Key_2);
	trudny = new QAction("Trudny", this);
	nowagra->addAction(trudny);
	connect(trudny, SIGNAL(triggered()), this, SLOT(genTrudny()));
	trudny->setShortcut(Qt::CTRL + Qt::Key_3);
	random = new QAction("Random", this);
	nowagra->addAction(random);
	connect(random, SIGNAL(triggered()), this, SLOT(genRandom()));
	random->setShortcut(Qt::CTRL + Qt::Key_4);

	pokaz = new QAction("Pokaż rozwiązanie", this);
	opcje->addAction(pokaz);
	connect(pokaz, SIGNAL(triggered()), this, SLOT(dawajRozw()));
	pokaz->setShortcut(Qt::CTRL + Qt::Key_F);
	zatrzymaj = new QAction("Zatrzymaj rozwiązywanie", this);
	opcje->addAction(zatrzymaj);
	connect(zatrzymaj, SIGNAL(triggered()), this, SLOT(stop()));
	zatrzymaj->setShortcut(Qt::CTRL + Qt::Key_R);
	cofnij = new QAction("Cofnij ruch", this);
	opcje->addAction(cofnij);
	connect(cofnij, SIGNAL(triggered()), this, SLOT(cofanie()));
	cofnij->setShortcut(Qt::CTRL + Qt::Key_Z);
	nastepny = new QAction("Kolejny ruch", this);
	opcje->addAction(nastepny);
	connect(nastepny, SIGNAL(triggered()), this, SLOT(doPrzodu()));
	nastepny->setShortcut(Qt::CTRL + Qt::Key_Y);

	info = new QAction("Informacje o grze", this);
	pomoc->addAction(info);
	info->setShortcut(Qt::Key_F1);
	connect(info, SIGNAL(triggered()), this, SLOT(wiadomosc()));

	gview = new grafika();
	setCentralWidget(gview);

	scene = new QGraphicsScene();
	scene->setBackgroundBrush(QColor("#e0e0e0"));

	gview->setScene(scene);
	gview->setEnabled(true);

	licznik = new QLabel();
	
	for (int i = 1; i <= 15; i++)
		kloc[i] = NULL;
	
	srand(time(NULL));
	planszaDoGry = NULL;
	
	genLatwy();

	statusBar()->addWidget(licznik);

	zegar = new QTimer(this);
	zegar->setInterval(400);
	connect(zegar, SIGNAL(timeout()), this, SLOT(doPrzodu()));
}

MainWindow::~MainWindow()
{
	qDeleteAll(scene->items());
	
	delete gview;
	delete scene;
	delete licznik;
}

void MainWindow::wiadomosc()
{
	QMessageBox::information(this, "Pomoc",
	 "Do przesuwania klocków używaj myszki lub klawiszy w,s,a,d. Inne skróty klawiaturowe są zaznaczone w menu");
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
	scene->setSceneRect(0, 0, gview->width() - 20, gview->height() - 10);
	generujPlansze();
}

void MainWindow::generujPlansze()
{
	for (int i = 1; i <= 15; i++)
		if (kloc[i] != NULL)
			kloc[i]->timeline->setCurrentTime(300);
	
	qDeleteAll(scene->items());

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (planszaDoGry->numerKlocka(i,j) != 0) {
				int nr = planszaDoGry->numerKlocka(i,j);

				kloc[nr] = new klocek(i, j, nr, planszaDoGry, scene, licznik);
				kloc[nr]->setScale((qreal) qMin(width(), height()) / 500.0);
			}
}

void MainWindow::cofanie()
{
	if (!planszaDoGry->poprzednie.empty()) {
		int kl = planszaDoGry->poprzednie.pop();
		kloc[kl]->zmianaPozycji(true, false);

		if (!planszaDoGry->nastepne.empty())
			planszaDoGry->nastepne.push(kl);
	}
}

void MainWindow::doPrzodu()
{
	if(!planszaDoGry->nastepne.empty()) {
		int kl = planszaDoGry->nastepne.pop();
		kloc[kl]->zmianaPozycji(false, false);
	}
}

void MainWindow::wygrana()
{
	zegar->stop();

	QMessageBox::information(this,
	 "Gratulacje!", "Brawo! Układanka została poprawnie ułożona! Po naciśnięciu OK zostanie wygenerowana nowa plansza.");

		genLatwy();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	int klx = planszaDoGry->pustyX(), kly = planszaDoGry->pustyY();

	if (e->key() == Qt::Key_A && planszaDoGry->numerKlocka(klx + 1, kly) != -1)
		kloc[ planszaDoGry->numerKlocka(klx + 1, kly) ]->zmianaPozycji(false, true);
	else if (e->key() == Qt::Key_D && planszaDoGry->numerKlocka(klx - 1, kly) != -1)
		kloc[ planszaDoGry->numerKlocka(klx - 1, kly) ]->zmianaPozycji(false, true);
	else if (e->key() == Qt::Key_W && planszaDoGry->numerKlocka(klx, kly + 1) != -1)
		kloc[ planszaDoGry->numerKlocka(klx, kly + 1) ]->zmianaPozycji(false, true);
	else if (e->key() == Qt::Key_S && planszaDoGry->numerKlocka(klx, kly - 1) != -1)
		kloc[ planszaDoGry->numerKlocka(klx, kly - 1) ]->zmianaPozycji(false, true);
}

void MainWindow::dawajRozw()
{
	if (planszaDoGry->nastepne.empty()) {
		licznik->setText("Generowanie rozwiązania...");
		planszaDoGry->genRozw();
		licznik->setText("Wygenerowano");
	}

	zegar->start();
}

void MainWindow::stop()
{
	zegar->stop();
}

void MainWindow::generujGre(int poziom)
{
	if (planszaDoGry != NULL)
		delete planszaDoGry;
	
	planszaDoGry = new plansza(poziom);
	generujPlansze();
	connect(planszaDoGry, SIGNAL(koniec()), this, SLOT(wygrana()));
	licznik->setText("Ilość wykonanych ruchów: " + QString::number(planszaDoGry->poprzednie.size()));
}

void MainWindow::genLatwy()
{
	generujGre(1);
}

void MainWindow::genSredni()
{
	generujGre(2);
}

void MainWindow::genTrudny()
{
	generujGre(3);
}

void MainWindow::genRandom()
{
	generujGre(4);
}
