#include "User.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Database.h"

QString User::login()
{
	return m_login;
}

User::User(int type_id, const QString& login, const QString& password)
{
	m_id = 0;
	m_type_id = type_id;
	m_login = login;
	m_password = password;
}

User::User(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("users");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString login = model.record(0).value("login").toString();
	QString password = model.record(0).value("password").toString();
	int type_id = model.record(0).value("type_id").toInt();
	db.close();

	m_id = id;
	m_type_id = type_id;
	m_login = login;
	m_password = password;

}

User::~User()
{
}

int User::type_id() const
{
	return m_type_id;
}

const QString & User::login() const
{
	return m_login;
}

const QString & User::password() const
{
	return m_password;
}


int User::type_id(QString & login)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	int id = -1;
	if (!query.exec("SELECT type_id FROM users WHERE login=\'" + login + "\'"))
	{
		qDebug() << "User::type_id(QString & login) - SQL error";
	}
	if (query.next())
			id = query.value(0).toInt();
	return id;
}


int User::user_id(const QString & login)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("SELECT id FROM users WHERE login=\'" + login + "\'"))
		return -1;	
	if (query.next())
		return query.value(0).toInt();		
	else
		return -1;
}



bool User::insert(QList<User> users)
{
	QString queryStr = "INSERT INTO users(type_id, login, password) VALUES ";
	for (int u = 0; u < users.count(); u++)
	{
		queryStr += "(";
		queryStr += QString::number(users.at(u).type_id());
		queryStr += ",'";
		queryStr += users.at(u).login();
		queryStr += "','";
		queryStr += users.at(u).password();
		queryStr += "'),";
	}
	queryStr.chop(1);
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec(queryStr)) {
			qDebug() << "User::insert(QList<User> users):  error inserting into Table users";
			qDebug() << query.lastError().text();
			db.close();
			return false;
	}
	db.close();
	return false;
}

int User::id()
{
	return m_id;
}

bool User::insertIntoDatabase()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO users ( type_id, login, password)\
		VALUES (?, ?, ?)");
	query.addBindValue(m_type_id);
	query.addBindValue(m_login);
	query.addBindValue(m_password);
	if (!query.exec()) {
		qDebug() << "User::insertIntoDatabase():  error inserting into Table users";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool User::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS  users (\
		id  INTEGER         PRIMARY KEY AUTOINCREMENT, \
		type_id     integer     NOT NULL,\
		login NVARCHAR(16) UNIQUE NOT NULL,\
		password NVARCHAR(16),\
		foreign key(type_id)  references usertypes(id) ON UPDATE CASCADE ON DELETE CASCADE\
		 )"
		))
	{
		qDebug() << "error creating users Table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool User::completeTable()
{
	QList<User> users;
	users << User(1, "search_module", "111")
		<< User(2, "system", "111")
		<< User(3, "Olga", "iamolga")
		<< User(8, "uservasya", "iamvasya");
	return insert(users);
}

bool User::userIsValid(const QString& login, const QString& password)
{
	QSqlDatabase db = Database::database();	
	QSqlQuery qry(db);
	if (!qry.exec("SELECT login FROM users WHERE login=\'" + login + "\' AND password=\'" + password + "\' AND type_id > 2"))
	{
		qDebug() << "error while checking if user is valid.";
		qDebug() << qry.lastError().text();
		db.close();
		return false;
	}
		if (qry.next())
		{
			db.close();
			return true;
		}
		else
		{
			db.close();
			return false;
		}	
}
