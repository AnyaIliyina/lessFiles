#include "State.h"
#include "Database.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTextCodec>

QString State::state_name()
{
	return m_name;
}

State::State(QString state_name)
{
	m_name = state_name;
	m_id = 0;
}

State::State(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("states");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString state_name = model.record(0).value("name").toString();
	db.close();

	m_id = id;
	m_name= state_name;
}

State::~State()
{
}

int State::id()
{
	return m_id;
}

int State::state_id(QString state_name)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("SELECT id FROM states WHERE name=\'" + state_name + "\'"))
	{
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

bool State::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO states(name)\
	VALUES (?)");
	query.addBindValue(m_name);
	if (!query.exec()) {
		qDebug() << "State::insertIntoDatabase(): error inserting into Table states";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool State::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS states (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{
		qDebug() << "error creating States table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool State::insert(QStringList stateNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString joinedNames = stateNames.join("'),('");
	QString qryString = QString("INSERT INTO states(name) VALUES ('%1')").arg(joinedNames);
	if (!query.exec(qryString))
	{
			qDebug() << "State :: insert(QStringList stateNames): error inserting into Table states";
			qDebug() << query.lastError().text();
			db.close();
			return false;
	}
	db.close();
	return true;
}


bool State::completeTable()
{
	QStringList stateNames;
	stateNames << "Не установлено"
		<< "Актуально"
		<< "Неактуально";
	return insert(stateNames);
}

QStringList State::getStates()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QStringList listStates;
	if (!query.exec("SELECT name FROM states"))
	{
		qDebug() << query.lastError().text();
		db.close();
		return listStates;
	}
	while (query.next()) {
		QString name = query.value(0).toString();
		listStates.push_back(name);
	}
	return listStates;
}

