#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <QTextCodec>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include "klocek.h"
#include "grafika.h"
#include "plansza.h"
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QTimer>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	grafika *gview;
	QGraphicsScene *scene;
	QMenu *nowagra, *opcje, *pomoc;
	QAction *latwy, *sredni, *trudny, *random;
	QAction *nastepny, *cofnij, *pokaz, *zatrzymaj;
	QAction *info;
	QLabel *licznik;

	klocek *kloc[16];
	plansza *planszaDoGry;
	QTimer *zegar;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *);
	void generujPlansze();
		void generujGre(int);

private slots:
	void wiadomosc();
	void genLatwy();
	void genSredni();
	void genTrudny();
	void genRandom();
	void cofanie();
	void doPrzodu();
	void dawajRozw();
	void wygrana();
	void stop();
};

#endif // MAINWINDOW_H
