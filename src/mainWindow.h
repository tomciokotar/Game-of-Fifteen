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
#include "tile.h"
#include "graphics.h"
#include "board.h"
#include <QResizeEvent>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QTimer>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Graphics *gview;
	QGraphicsScene *scene;
	QMenu *newGameMenu, *optionsMenu, *helpMenu;
	QAction *easyGame, *mediumGame, *hardGame;	// *randomGame;
	QAction *nextMove, *previousMove, *solve, *stopSolving;
	QAction *getInfo;
	QLabel *counter;

	Tile *tiles[16];
	Board *gameBoard;
	QTimer *timer;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void resizeEvent(QResizeEvent *);
	void keyPressEvent(QKeyEvent *);
	void generateBoard();
	void generateGame(int);

private slots:
	void showInstructions();
	void generateEasy();
	void generateMedium();
	void generateHard();
	void generateRandom();
	void moveBack();
	void moveForward();
	void showSolution();
	void gameFinished();
	void solvingStopped();
};

#endif // MAINWINDOW_H
