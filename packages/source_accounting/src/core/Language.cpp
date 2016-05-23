#include "Language.h"
#include "Database.h"
#include "Scale.h"
#include <QString>

bool Language::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS languages (\
		id INTEGER PRIMARY KEY AUTOINCREMENT, \
		name TEXT UNIQUE NOT NULL\
		)"
		))
	{
		qDebug() << "error creating Languages table in database";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Language::completeTable()
{
	QStringList languages;
	languages << "русский" << "английский" << "французский" << "немецкий";
	return insert(languages);
}

bool Language::insert(QStringList languageNames)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString joinedNames = languageNames.join("'),('");
	QString qryString = QString("INSERT INTO languages(name)\
	VALUES ('%1')").arg(joinedNames);
	if (!query.exec(qryString)) {
			qDebug() << "Language::insert(QStringList languageNames): error inserting into table Languages";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	db.close();
}

QStringList Language::getList()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QStringList languages;
	if (!query.exec("SELECT name FROM languages"))
	{
		qDebug() << query.lastError().text();
		db.close();
		return languages;
	}
	while (query.next()) {
		QString name = query.value(0).toString();
		languages.push_back(name);
	}
	return languages;
}

QStringList Language::getList(int id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery queryLang(db);
	QStringList languages;
	queryLang.prepare((" SELECT name FROM languages WHERE id in (SELECT site_langs.language_id FROM site_langs WHERE site_langs.site_id = :id )"));
	queryLang.bindValue(":id", id);
	if (!queryLang.exec())
	{
		qDebug() << queryLang.lastError().text();
	}
	while (queryLang.next())
	{
		languages.push_back(queryLang.value(0).toString());
	}
	return languages;
}

QList<int> Language::getIDs(QStringList listLang)
{
	QSqlDatabase db = Database::database();
	QList<int> listIDs;
	QSqlQuery queryLang(db);
	QString str = listLang.join("','");
	QString qry = QString("SELECT id FROM languages WHERE name IN ('%1')").arg(str);
	queryLang.prepare(qry);
	if (!queryLang.exec())
	{
		qDebug() << queryLang.lastError().text();
	}
	while (queryLang.next())
	{
		int gr = queryLang.value(0).toInt();
		listIDs.push_back(gr);
	}
	
	return listIDs;
}



