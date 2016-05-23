#include <QApplication>
#include <qDebug>
#include <QTranslator>
#include <QDir>
#include <QStandardPaths>

#include "database/Database.h"
#include "Language.h"
#include "MainWindow.h"



/*!
\file
\brief Enterance point
*/

int main(int argc, char* argv[])
{	
	setlocale(LC_ALL, "Russian");
	QApplication app(argc, argv);

	QTranslator* qtTranslator = new QTranslator;
	qtTranslator->load("qt_" + QLocale::system().name(), ":/translations");
	app.installTranslator(qtTranslator);

	QApplication::setOrganizationName("itain");
	QApplication::setOrganizationDomain("infokosmo.ru");
	QApplication::setApplicationName("source_accounting");

	QDir dir;
	dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
	qDebug() << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	Database::restore();
	MainWindow *w = new MainWindow();
	return app.exec();
};