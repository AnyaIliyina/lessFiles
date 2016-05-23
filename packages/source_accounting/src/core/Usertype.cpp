#include "Usertype.h"
#include "Database.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTextCodec>

QString Usertype::type_name()
{
	return m_name;
}

Usertype::Usertype(QString type_name)
{
	m_id = 0;
	m_name = type_name;
}

Usertype::Usertype(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(this, db);
	model.setTable("usertypes");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString type_name = model.record(0).value("name").toString();
	db.close();

	m_id = id;
	m_name = type_name;
}

Usertype::~Usertype()
{
}

int Usertype::id()
{
	return m_id;
}

bool Usertype::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO usertypes(name)\
	VALUES (?)");
	query.addBindValue(m_name);
	if (!query.exec()) {
		qDebug() << "State::insertIntoDatabase(): error inserting into Table usertypes";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Usertype::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS usertypes (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{
		qDebug() << "error creating usertypes Table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Usertype::completeTable()
{
	QStringList typeNames;
	typeNames << "Модуль поиска"
		<< "Система"
		<< "Эксперт"
		<< "Пользователь";
	return insert(typeNames);
}

bool Usertype::insert(QStringList typeNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString joinedNames = typeNames.join("'),('");
	QString qryString = QString("INSERT INTO usertypes(name) VALUES ('%1')").arg(joinedNames);
	if (!query.exec(qryString)) {
			qDebug() << "Usertype::insert(QStringList typeNames): error inserting into Table states";
			qDebug() << query.lastError().text();
			db.close();
			return false;
	}
	db.close();
	return true;
}

