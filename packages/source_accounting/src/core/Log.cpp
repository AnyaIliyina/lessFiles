#include "Log.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Database.h"
#include "Log.h"

/*!
\file
\brief  
*/

Log::Log(int session_id, QString operation_name, int object_id, QString error_string):
	m_session_id(session_id), m_operation_name(operation_name),
	m_object_id(object_id), m_error_string(error_string)
{
}

Log::~Log()
{
}


bool Log::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS logs (\
		id   INTEGER         PRIMARY KEY AUTOINCREMENT, \
		session_id INTEGER NOT NULL,\
		operation_name TEXT NOT NULL,\
		object_id INTEGER,\
		error_string TEXT,\
		FOREIGN KEY(session_id) REFERENCES sessions(session_id) ON UPDATE CASCADE ON DELETE CASCADE \
		)"
		))
	{
		qDebug() << "error creating logs Table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

void Log::create(int session_id, QString operation_name, int object_id, QString error_string)
{
	Log *l = new Log(session_id, operation_name, object_id, error_string);
	l->insertIntoDatabase();
	delete l;
}


bool Log::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO logs ( session_id, operation_name, object_id, error_string)\
		VALUES (?, ?, ?, ?)");
	query.addBindValue(m_session_id);
	query.addBindValue(m_operation_name);
	query.addBindValue(m_object_id);
	query.addBindValue(m_error_string);
	if (!query.exec()) {
		qDebug() << "Log::insertIntoDatabase(): error inserting into table Logs";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}
