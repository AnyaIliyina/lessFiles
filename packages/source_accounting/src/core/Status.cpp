#include "Status.h"
#include "Database.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTextCodec>

Status::Status(QString status_name)
{
	m_id = 0;
	m_name = status_name;
}

Status::Status(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(this, db);
	model.setTable("statuses");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString status_name = model.record(0).value("name").toString();
	db.close();

	m_id = id;
	m_name = status_name;
}

Status::~Status()
{
}

const QString& Status::name()
{
	return m_name;
}

int Status::id()
{
	return m_id;
}

bool Status::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO statuses(name)\
	VALUES (?)");
	query.addBindValue(m_name);
	if (!query.exec()) {
		qDebug() << "Status::insertIntoDatabase(): error inserting into Table statuses";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Status::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS statuses (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{
		qDebug() << "error creating statuses Table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Status::completeTable()
{
	QStringList statusNames;
	statusNames << "Не проверено"
		<< "Проверено модулем поиска"
		<< "Проверено экспертом";
	return insert(statusNames);
}

bool Status::insert(QStringList statusNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString joinedNames = statusNames.join("'),('");
	QString qryString = QString("INSERT INTO statuses(name) VALUES ('%1')").arg(joinedNames);
	if (!query.exec(qryString))
		{
			qDebug() << "Status :: insert(QStringList statusNames): error inserting into Table statuses";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	db.close();
	return true;
}


