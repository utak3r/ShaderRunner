#include "srMainWindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
	{
	QApplication a(argc, argv);
	srMainWindow w;
	w.show();
	return a.exec();
	}
