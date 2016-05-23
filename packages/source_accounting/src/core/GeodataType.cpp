#include "GeodataType.h"
#include "Database.h"
#include <QStringList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
bool GeodataType::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS geodata_types (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{
		qDebug() << "error creating Geodata_types table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool GeodataType::completeTable()
{
	QStringList typeNames;
	typeNames << "аэрофотоснимки"
		<< "картографические материалы"
		<< "данные дистанционного зондирования";
	return insert(typeNames);
}

bool GeodataType::insert(QStringList typeNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	for (int i = 0; i < typeNames.count(); i++)
	{
		query.prepare("INSERT INTO geodata_types(name)\
	VALUES (?)");
		query.addBindValue(typeNames.at(i));
		if (!query.exec()) {
			qDebug() << "GeodataType::insert(QStringList typeNames): error inserting into table Geodata_types";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	}
	db.close();
}

QStringList GeodataType::getList()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QStringList typeNames;
	if (!query.exec("SELECT name FROM geodata_types"))
	{
		qDebug() << query.lastError().text();
		db.close();
		return typeNames;
	}
	while (query.next()) {
		QString name = query.value(0).toString();
		typeNames.push_back(name);
	}
	return typeNames;
}

QStringList GeodataType::getListForSites(int id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery queryGPI(db);
	QStringList typeNames;
	queryGPI.prepare(("SELECT name FROM geodata_types WHERE id in (SELECT site_types.type_id FROM site_types WHERE site_types.site_id = :id )"));
	queryGPI.bindValue(":id", id);
	if (!queryGPI.exec())
	{
		qDebug() << queryGPI.lastError().text();
	}
	while (queryGPI.next())
		typeNames.push_back(queryGPI.value(0).toString());
	return typeNames;
}

QStringList GeodataType::getListForDepartments(int id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery queryGPI(db);
	QStringList typeNames;
	queryGPI.prepare(("SELECT name FROM geodata_types WHERE id in (SELECT department_types.type_id FROM department_types WHERE department_types.department_id = :id )"));
	queryGPI.bindValue(":id", id);
	if (!queryGPI.exec())
	{
		qDebug() << queryGPI.lastError().text();
	}
	while (queryGPI.next())
		typeNames.push_back(queryGPI.value(0).toString());
	return typeNames;
}

QList<int> GeodataType::getIDs(QStringList listGPI)
{
	QSqlDatabase db = Database::database();
	QList<int> listIDs;
	QSqlQuery query(db);
	QString str = listGPI.join("','");
	qDebug().noquote() << str;
	QString qry = QString("SELECT id FROM geodata_types WHERE name IN ('%1')").arg(str);
	qDebug().noquote() << qry;
	query.prepare(qry);
	if (!query.exec())
	{
		qDebug() << query.lastError().text();
	}
	while (query.next())
	{
		int gr = query.value(0).toInt();
		listIDs.push_back(gr);
	}

	return listIDs;
}
