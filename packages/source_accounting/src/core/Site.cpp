#include "Site.h"
#include "Log.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Language.h"
#include "SiteLang.h"



const QString & Site::name() const
{
	return m_name;
}

const QString & Site::comment() const
{
	return m_comment;
}

int Site::id() const
{
	return m_id;
}


const QString&  Site::url() const
{
	return m_url;
}

Site::Site(const QString& url, const QString& name, int status, const QString& comment)
{
	m_id = 0;
	m_url = url;
	m_name = name;
	m_status_id = status;
	m_comment = comment;
}

Site::Site(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("sites");
	//const QString filter("siteId == " + QString::number(site_id));
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString url = model.record(0).value("url").toString();
	QString name = model.record(0).value("name").toString();
	int status_id= model.record(0).value("status_id").toInt();
	QString comment = model.record(0).value("comment").toString();
	db.close();
		
	m_id = id;
	m_url = url;
	m_name = name;
	m_status_id = status_id;
	m_comment = comment;
}

Site::~Site()
{
}


int Site::status_id() const
{
	return m_status_id;
}

//QStringList Site::languages()
//{
//	QStringList languages;
//	QList<int> indexes = SiteLang::languagesOfSite(m_id);
//	for (int i = 0; i < indexes.count(); i++)
//		languages << langList.at(i);
//	return languages;
//}

bool Site::setStatusId(int site_id, int status_id, int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("UPDATE sites\
		SET status_id = ?\
		WHERE id = ?");
	query.addBindValue(status_id);
	query.addBindValue(site_id);
	if (!query.exec()) {
		qDebug() << "Site::setStatusId(int site_id, int status_id):  error inserting into Table sites";
		QString errorString =  query.lastError().text();
		db.close();
		Log::create(session_id, "Site: setStatusId", site_id, errorString);
		return false;
	}
	db.close();
	Log::create(session_id, "Site: setStatusId", site_id);
	return true;
}


int Site::site_id(QString & site_name) 
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("SELECT id FROM sites WHERE name=\'" + site_name + "\'"))
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


int Site::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO sites ( url, name, status_id, comment)\
		VALUES (?, ?, ?, ?)");
	query.addBindValue(m_url);
	query.addBindValue(m_name);
	query.addBindValue(m_status_id);
	query.addBindValue(m_comment);
	if (!query.exec()) {
		qDebug() << "Site::insertIntoDatabase():  error inserting into Table sites";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Site: insert", 0, errorString);
		return -1;
	}
	else {
		m_id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "Site: insert", m_id);
		return m_id;
	}
	
}


bool Site::insert(QList<Site> sites)
{	
	QString queryStr = "INSERT INTO sites(url, name, status_id, comment) VALUES ";
	for (int s = 0; s < sites.count(); s++)
	{
		queryStr += "('";
		queryStr += sites.at(s).url();
		queryStr += "','";
		queryStr += sites.at(s).name();
		queryStr += "',";
		queryStr += QString::number(sites.at(s).status_id());
		queryStr += ",'";
		queryStr += sites.at(s).comment();
		queryStr += "'),";
	}
	queryStr.chop(1);

	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec(queryStr)) {
			qDebug() << "Site::insertIntoDatabase(QList<Site> sites):  error inserting into Table sites";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	db.close();
	return true;
}

bool Site::urlIsNew(const QString & url)
{
	// TODO
	return false;
}

bool Site::uncheckedSitesFound()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("SELECT id FROM sites WHERE status_id=1"))
	{
		qDebug() << "Site::uncheckedSitesFound() error";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}

	if (query.next())	{
		db.close();
		return true;
	}
	else	{
		db.close();
		return false;
	}
}



bool Site::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  sites (\
		id  INTEGER         PRIMARY KEY AUTOINCREMENT, \
		url     TEXT    UNIQUE NOT NULL,\
		name TEXT UNIQUE NOT NULL,\
		status_id INTEGER,		\
		comment TEXT,   \
		FOREIGN KEY(status_id) REFERENCES statuses(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)) )
	{
		qDebug() << "error creating sites Table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
	
}

bool Site::completeTable()
{
		QList<Site> sites;
	sites << Site("http://beryllium.gis-lab.info/project/osmshp", "gis-lab.info")
		<< Site("http://www.afanas.ru/mapbase/", "afanas.ru")
		<< Site("http://download.geofabrik.de/index.html", "geofabrik.de");
	return insert(sites);
}

QList<int> Site::languageIDs()
{
	return SiteLang::languagesBySite(m_id);
}

QList<Site> Site::sitesByStatus(int statusId)
{
	QList<Site> siteList;
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("sites");
	const QString filter = QString("status_id == %1").arg(statusId);
	model.setFilter(filter);
	model.select();
	for (int i = 0; i < model.rowCount(); i++)
	{
		Site *s = new Site(model.record(i).value("id").toInt());
		siteList.append(*s);
		delete s;
	}
	db.close();
	return siteList;
}

QStringList Site::getSiteNames()

{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QStringList listSites;
	if (!query.exec("SELECT name FROM sites"))
	{
		qDebug() << query.lastError().text();
		db.close();
		return listSites;
	}
	while (query.next()) {
		QString name = query.value(0).toString();
		listSites.push_back(name);
	}
	return listSites;
}

bool Site::urlFromString(QString & string)
{
	const QString urlStr = string.trimmed();
	const QRegExp qualifiedUrl(QLatin1String("(http|ftp|https):\/\/[\w\-_]+(\.[\w\-_]+)+([\w\-\.,@?^=%&amp;:/~\+#]*[\w\-\@?^=%&amp;/~\+#])?"));

	// Check if it looks like a qualified URL. Try parsing it and see.
	const bool hasSchema = qualifiedUrl.exactMatch(urlStr);
	if (hasSchema) {
		const QUrl url(urlStr, QUrl::TolerantMode);
		if (url.isValid())
			return true;
	}
	return false;
}

void Site::deleteRecord(int & id, int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString idstr = QString::number(id);
	if (!query.exec("DELETE FROM sites WHERE id=\'" + idstr + "\'"))
	{
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "Site: delete", id, errorString);
	}
	else
	{
		Log::create(session_id, "Site: delete", id);
	}
}

void Site::setId(int id)
{
	m_id = id;
}


void Site::updateRecord(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("UPDATE sites\
					SET\
					url =:url, name=:name, comment=:comment WHERE id=:id");
	query.bindValue(":url", m_url);
	query.bindValue(":name", m_name);
	query.bindValue(":comment", m_comment);
	query.bindValue(":id", m_id);
	if (!query.exec()) {
		qDebug() << "Site::updateRecord():  error updating sites";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Site: update", m_id, errorString);
	}
	db.close();
	Log::create(session_id, "Site: update", m_id);
}
