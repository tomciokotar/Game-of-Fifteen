#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	setMinimumSize(450, 480);
	
	newGameMenu = menuBar()->addMenu("New game");
	optionsMenu = menuBar()->addMenu("Options");
	helpMenu = menuBar()->addMenu("Help");

	easyGame = new QAction("Easy", this);
	newGameMenu->addAction(easyGame);
	connect(easyGame, SIGNAL(triggered()), this, SLOT(generateEasy()));
	easyGame->setShortcut(Qt::CTRL + Qt::Key_1);
	
	mediumGame = new QAction("Medium", this);
	newGameMenu->addAction(mediumGame);
	connect(mediumGame, SIGNAL(triggered()), this, SLOT(generateMedium()));
	mediumGame->setShortcut(Qt::CTRL + Qt::Key_2);
	
	hardGame = new QAction("Hard", this);
	newGameMenu->addAction(hardGame);
	connect(hardGame, SIGNAL(triggered()), this, SLOT(generateHard()));
	hardGame->setShortcut(Qt::CTRL + Qt::Key_3);
	
	/*randomGame = new QAction("Random", this);
	newGameMenu->addAction(randomGame);
	connect(randomGame, SIGNAL(triggered()), this, SLOT(generateRandom()));
	randomGame->setShortcut(Qt::CTRL + Qt::Key_4);*/

	solve = new QAction("Solve", this);
	optionsMenu->addAction(solve);
	connect(solve, SIGNAL(triggered()), this, SLOT(showSolution()));
	solve->setShortcut(Qt::CTRL + Qt::Key_F);
	
	stopSolving = new QAction("Stop solving", this);
	optionsMenu->addAction(stopSolving);
	connect(stopSolving, SIGNAL(triggered()), this, SLOT(solvingStopped()));
	stopSolving->setShortcut(Qt::CTRL + Qt::Key_R);
	
	previousMove = new QAction("Move back", this);
	optionsMenu->addAction(previousMove);
	connect(previousMove, SIGNAL(triggered()), this, SLOT(moveBack()));
	previousMove->setShortcut(Qt::CTRL + Qt::Key_Z);
	
	nextMove = new QAction("Move forward", this);
	optionsMenu->addAction(nextMove);
	connect(nextMove, SIGNAL(triggered()), this, SLOT(moveForward()));
	nextMove->setShortcut(Qt::CTRL + Qt::Key_Y);
	
	getInfo = new QAction("Instructions", this);
	helpMenu->addAction(getInfo);
	connect(getInfo, SIGNAL(triggered()), this, SLOT(showInstructions()));
	getInfo->setShortcut(Qt::Key_F1);

	gview = new Graphics();
	setCentralWidget(gview);

	scene = new QGraphicsScene();
	scene->setBackgroundBrush(QColor("#e0e0e0"));

	gview->setScene(scene);
	gview->setEnabled(true);

	counter = new QLabel();
	
	for (int i = 1; i <= 15; i++)
		tiles[i] = NULL;
	
	srand(time(NULL));
	gameBoard = NULL;
	
	generateEasy();

	statusBar()->addWidget(counter);

	timer = new QTimer(this);
	timer->setInterval(400);
	connect(timer, SIGNAL(timeout()), this, SLOT(moveForward()));
}

MainWindow::~MainWindow()
{
	qDeleteAll(scene->items());
	
	delete gview;
	delete scene;
	delete counter;
}

void MainWindow::showInstructions()
{
	QMessageBox::information(this, "Instructions",
	 "Use mouse or w,s,a,d buttons to move tiles. Open menus to see keyboard shortcuts.");
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
	scene->setSceneRect(0, 0, gview->width() - 20, gview->height() - 10);
	generateBoard();
}

void MainWindow::generateBoard()
{
	for (int i = 1; i <= 15; i++)
		if (tiles[i] != NULL)
			tiles[i]->timeline->setCurrentTime(300);
	
	qDeleteAll(scene->items());

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (gameBoard->getTileNumber(i,j) != 0) {
				int number = gameBoard->getTileNumber(i,j);

				tiles[number] = new Tile(i, j, number, gameBoard, scene, counter);
				tiles[number]->setScale((qreal) qMin(width(), height()) / 500.0);
			}
}

void MainWindow::moveBack()
{
	if (!gameBoard->previousMoves.empty()) {
		int tileNumber = gameBoard->previousMoves.pop();
		tiles[tileNumber]->changePosition(true, false);

		if (!gameBoard->nextMoves.empty())
			gameBoard->nextMoves.push(tileNumber);
	}
}

void MainWindow::moveForward()
{
	if(!gameBoard->nextMoves.empty()) {
		int tileNumber = gameBoard->nextMoves.pop();
		tiles[tileNumber]->changePosition(false, false);
	}
}

void MainWindow::gameFinished()
{
	timer->stop();

	QMessageBox::information(this,
	 "Congratulations!", "Great! You've solved it! Click OK to mix the tiles up again.");
	
	generateEasy();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	int emptyTileX = gameBoard->getEmptyTileX(), emptyTileY = gameBoard->getEmptyTileY();

	if (e->key() == Qt::Key_A && gameBoard->getTileNumber(emptyTileX + 1, emptyTileY) != -1)
		tiles[ gameBoard->getTileNumber(emptyTileX + 1, emptyTileY) ]->changePosition(false, true);
		
	else if (e->key() == Qt::Key_D && gameBoard->getTileNumber(emptyTileX - 1, emptyTileY) != -1)
		tiles[ gameBoard->getTileNumber(emptyTileX - 1, emptyTileY) ]->changePosition(false, true);
		
	else if (e->key() == Qt::Key_W && gameBoard->getTileNumber(emptyTileX, emptyTileY + 1) != -1)
		tiles[ gameBoard->getTileNumber(emptyTileX, emptyTileY + 1) ]->changePosition(false, true);
		
	else if (e->key() == Qt::Key_S && gameBoard->getTileNumber(emptyTileX, emptyTileY - 1) != -1)
		tiles[ gameBoard->getTileNumber(emptyTileX, emptyTileY - 1) ]->changePosition(false, true);
}

void MainWindow::showSolution()
{
	if (gameBoard->nextMoves.empty()) {
		counter->setText("Generating the solution...");
		gameBoard->generateSolution();
		counter->setText("Generated");
	}

	timer->start();
}

void MainWindow::solvingStopped()
{
	timer->stop();
}

void MainWindow::generateGame(int level)
{
	if (gameBoard != NULL)
		delete gameBoard;
	
	gameBoard = new Board(level);
	generateBoard();
	connect(gameBoard, SIGNAL(theEnd()), this, SLOT(gameFinished()));
	counter->setText("Number of moves: " + QString::number(gameBoard->previousMoves.size()));
}

void MainWindow::generateEasy()
{
	generateGame(1);
}

void MainWindow::generateMedium()
{
	generateGame(2);
}

void MainWindow::generateHard()
{
	generateGame(3);
}

void MainWindow::generateRandom()
{
	generateGame(4);
}
