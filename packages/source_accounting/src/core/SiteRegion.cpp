#include "SiteRegion.h"

void SiteRegion::deleteRecord(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString site_idstr = QString::number(m_site_id);
	QString region_idstr = QString::number(m_region_id);
	if (!query.exec("DELETE FROM site_regions WHERE ((site_id=\'" + site_idstr + "\') AND (region_id=\'" +
		region_idstr + "\'))"))
	{
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "SiteRegion: delete", m_site_id, errorString);
	}
	else
	{
		Log::create(session_id, "SiteRegion: delete", m_site_id);
	}
}

bool SiteRegion::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  site_regions (\
		site_id INTEGER,		\
		region_id INTEGER,   \
		FOREIGN KEY(site_id) REFERENCES sites(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(region_id) REFERENCES regions(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating Site_regions table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	SiteRegion(1, 1).insertIntoDatabase();
	return true;
}


SiteRegion::SiteRegion(int site_id, int region_id):
	m_site_id(site_id), m_region_id(region_id)
{
}

SiteRegion::~SiteRegion()
{
}

void SiteRegion::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO site_regions( site_id, region_id)\
		VALUES (?, ?)");
	query.addBindValue(m_site_id);
	query.addBindValue(m_region_id);
	if (!query.exec()) {
		qDebug() << "SiteRegion::insertIntoDatabase():  error ";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "SiteRegion: insert", 0, errorString);
	}
	else {
		int id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "SiteRegion: insert", id);
	}
}



QList<int> SiteRegion::sitesByRegion(int region_id)
{
	QList<int>siteIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT site_id FROM site_regions WHERE region_id =?");
	query.addBindValue(region_id);
	if (!query.exec())
	{
		qDebug() << "SiteRegion::sitesByRegion(int region_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		siteIds << query.value(0).toInt();

	db.close();
	return siteIds;
}

QList<int> SiteRegion::regionsBySite(int site_id)
{
	QList<int>regionIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT region_id FROM site_regions WHERE site_id =?");
	query.addBindValue(site_id);
	if (!query.exec())
	{
		qDebug() << "SiteRegion::regionsBySite(int site_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		regionIds << query.value(0).toInt();

	db.close();
	return regionIds;
}
