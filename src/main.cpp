#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	w.setWindowTitle("Przesuwanka");

	w.show();

	return a.exec();
}
