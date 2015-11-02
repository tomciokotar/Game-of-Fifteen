#include <QtGui/QApplication>
#include "mainWindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	window.setWindowTitle("Game of Fifteen");

	window.show();

	return app.exec();
}
