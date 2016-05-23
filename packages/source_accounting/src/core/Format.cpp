#include "Format.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Database.h"

/*!
\file
\brief  
*/


const QString& Format::name() const
{
	return m_name;
}

int Format::id(QString name) 
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("SELECT id FROM formats WHERE name=\'" + name + "\'"))
	{
		qDebug() << "Zapros ne proshel";
		qDebug() << query.lastError().text();
		return -1;
	}
	else
	{
		if (query.next())
		{
			int id = query.value(0).toInt();
			return id;
		}
	}
}


Format::Format(const QString &name)
{
	m_name = name;
	m_id = 0;
}

Format::Format(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(this, db);
	model.setTable("formats");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString name = model.record(0).value("name").toString();
	db.close();

	m_id = id;
	m_name = name;
}

Format::~Format()
{
}

bool Format::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO formats(name)\
	VALUES (?)");
	query.addBindValue(m_name);
	if (!query.exec()) {
		qDebug() << "Format::insertIntoDatabase(): error inserting into Table formats";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Format::insert(QStringList formatNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString joinedNames = formatNames.join("'),('");
	QString qryString = QString("INSERT INTO formats(name) VALUES ('%1')").arg(joinedNames);
	if (!query.exec(qryString)) {
			qDebug() << "Format::insert(QStringList formatNames): error inserting into Table formats";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	db.close();
}

bool Format::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS formats (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{	
		qDebug() << "error creating formats Table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Format::completeTable()
{
	QStringList formatNames;
	formatNames << ".shp"
				<< ".osm.pbf"
				<< ".osm.bz2"
				<< ".png";
	return insert(formatNames);
}

QStringList Format::getNames()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QStringList listFormat;
	if (!query.exec("SELECT name FROM formats"))
	{
		qDebug() << query.lastError().text();
		db.close();
		return listFormat;
	}
	while (query.next()) {
		QString name = query.value(0).toString();
		listFormat.push_back(name);
	}
	return listFormat;
}
