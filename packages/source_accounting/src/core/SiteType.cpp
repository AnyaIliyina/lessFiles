#include "SiteType.h"

SiteType::SiteType(int site_id, int type_id):
	m_site_id(site_id), m_type_id(type_id)
{
}

SiteType::~SiteType()
{
}

void SiteType::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO site_types( site_id, type_id)\
		VALUES (?, ?)");
	query.addBindValue(m_site_id);
	query.addBindValue(m_type_id);
	if (!query.exec()) {
		qDebug() << "SiteType::insertIntoDatabase():  error ";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "SiteType: insert", 0, errorString);
	}
	else {
		int id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "SiteType: insert", id);
	}
}

QList<int> SiteType::sitesByType(int type_id)
{
	QList<int>siteIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT site_id FROM site_types WHERE type_id =?");
	query.addBindValue(type_id);
	if (!query.exec())
	{
		qDebug() << "SiteType::sitesByType(int type_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		siteIds << query.value(0).toInt();

	db.close();
	return siteIds;
}

QList<int> SiteType::typesBySite(int site_id)
{
	QList<int>typeIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT type_id FROM site_types WHERE site_id =?");
	query.addBindValue(site_id);
	if (!query.exec())
	{
		qDebug() << " SiteType::typesBySite(int site_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		typeIds << query.value(0).toInt();

	db.close();
	return typeIds;
}

bool SiteType::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  site_types (\
		site_id INTEGER,		\
		type_id INTEGER,   \
		FOREIGN KEY(site_id) REFERENCES sites(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(type_id) REFERENCES geodata_types(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating  SiteType table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();

	SiteType(1, 2).insertIntoDatabase();
	SiteType(2, 2).insertIntoDatabase();
	SiteType(3, 2).insertIntoDatabase();
	return true;
}

void SiteType::deleteBySite(int site_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("DELETE FROM site_types WHERE site_id =?");
	query.addBindValue(site_id);
	if (!query.exec())
	{
		qDebug() << "SiteType::deleteBySite(int site_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	db.close();
}