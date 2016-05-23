#include "SiteLang.h"
#include "Log.h"
#include <QSqlError>

SiteLang::SiteLang(int site_id, int language_id):
	m_site_id(site_id), m_language_id(language_id)
{
}

SiteLang::~SiteLang()
{
}

void SiteLang::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO site_langs( site_id, language_id)\
		VALUES (?, ?)");
	query.addBindValue(m_site_id);
	query.addBindValue(m_language_id);
	if (!query.exec()) {
		qDebug() << "SiteLang::insertIntoDatabase():  error ";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "SiteLang: insert", 0, errorString);
	}
	else {
		int id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "SiteLang: insert", id);
	}
}

QList<int> SiteLang::sitesByLanguage(int language_id)
{
	QList<int>siteIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT site_id FROM site_langs WHERE language_id =?");
	query.addBindValue(language_id);
	if (!query.exec())
	{
		qDebug() << "SiteLang::sitesByLanguage(int language_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		siteIds << query.value(0).toInt();
	
	db.close();	
	return siteIds;
}

QList<int> SiteLang::languagesBySite(int site_id)
{
	QList<int>langIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT language_id FROM site_langs WHERE site_id =?");
	query.addBindValue(site_id);
	if (!query.exec())
	{
		qDebug() << "SiteLang::languagesBySite(int site_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		langIds << query.value(0).toInt();

	db.close();
	return langIds;
	
}

void SiteLang::deleteBySite(int site_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("DELETE FROM site_langs WHERE site_id =?");
	query.addBindValue(site_id);
	if (!query.exec())
	{
		qDebug() << "SiteLang::deleteBySite(int site_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	db.close();
}

bool SiteLang::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  site_langs (\
		site_id INTEGER,		\
		language_id INTEGER,   \
		FOREIGN KEY(site_id) REFERENCES sites(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(language_id) REFERENCES languages(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating Site_langs table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();

	SiteLang(1, 1).insertIntoDatabase();
	SiteLang(2, 1).insertIntoDatabase();
	SiteLang(3, 2).insertIntoDatabase();
	return true;
}
