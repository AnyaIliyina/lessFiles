#include "Region.h"
#include "Log.h"
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlError>

/*!
\file
\brief  
*/

Region::Region()
{
}

Region::Region(int parent_id, const QString & name, const QString & comment):
	m_parent_id(parent_id), m_name(name), m_comment(comment)
{
}

Region::~Region()
{
}

int Region::id() const
{
	return m_id;
}

int Region::parent_id() const
{
	return m_parent_id;
}

const QString & Region::name() const
{
	return m_name;
}

const QString & Region::comment() const
{
	return m_comment;
}

void Region::setId(int id)
{
	m_id = id;
}

void Region::setParentId(int parent_id)
{
	m_parent_id = parent_id;
}

void Region::setName(const QString & name)
{
	m_name = name;
}

void Region::setComment(const QString & comment)
{
	m_comment = comment;
}

Region::Region(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("regions");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	int parent_id = model.record(0).value("parent_id").toInt();
	QString name = model.record(0).value("name").toString();
	QString comment = model.record(0).value("comment").toString();
	db.close();

	m_id = id;
	m_parent_id = parent_id;
	m_name = name;
	m_comment = comment;
}

bool Region::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO regions (parent_id, name, comment)\
		VALUES (?, ?, ?)");
	query.addBindValue(m_parent_id);
	query.addBindValue(m_name);
	query.addBindValue(m_comment);
	if (!query.exec()) {
		qDebug() << "Region::insertIntoDatabase():  error inserting into table Departments";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Region: insert", 0, errorString);
		return false;
	}
	else {
		m_id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "Region: insert", m_id);
		return true;
	}
}

bool Region::update(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("UPDATE regions\
					SET\
					parent_id=:parent_id, name=:name, comment=:comment \
						WHERE id=:id");
	query.bindValue(":name", m_name);
	query.bindValue(":parent_id", m_parent_id);
	query.bindValue(":comment", m_comment);
	query.bindValue(":id", m_id);
	if (!query.exec()) {
		qDebug() << "Region::update():  error";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Region: update", m_id, errorString);
		return false;
	}
	db.close();
	Log::create(session_id, "Region: update", m_id);
	return true;
}

bool Region::deleteRegion(int region_id, int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if(!query.exec("PRAGMA foreign_keys = ON"))
		qDebug() << query.lastError().text();
	QString idstr = QString::number(region_id);
	if (!query.exec("DELETE FROM regions WHERE id=\'" + idstr + "\'"))
	{
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "Region: delete", region_id, errorString);
		return false;
	}
	else
	{
		Log::create(session_id, "Region: delete", region_id);
		return true;
	}
}


bool Region::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  regions (\
		id  INTEGER         PRIMARY KEY AUTOINCREMENT, \
		parent_id  INTEGER  ,\
		name     TEXT    UNIQUE NOT NULL,\
		comment     TEXT   ,   \
		FOREIGN KEY(parent_id) REFERENCES regions(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating Regions table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Region::completeTable()
{
	QList<Region> regions;
	regions << Region(0, "Европа")
		<< Region(0, "Азия")
		<< Region(0, "Африка")
		<< Region(0, "Северная Америка")
		<< Region(0, "Южная Америка")
		<< Region(1, "Австрия")
		<< Region(1, "Бельгия")
		<<Region(1, "Россия")
		<< Region(8, "Центральный федеральный округ");
	return insert(regions);
}

bool Region::insert(QList<Region> regions)
{
	QString queryStr = "INSERT INTO regions(parent_id, name, comment) VALUES ";
	for (int r = 0; r < regions.count(); r++)
	{
		queryStr += "(";
		queryStr += QString::number(regions.at(r).parent_id());
		queryStr += ",'";
		queryStr += regions.at(r).name();
		queryStr += "','";
		queryStr += regions.at(r).comment();
		queryStr += "'),";
	}
	queryStr.chop(1);
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec(queryStr)) {
			qDebug() << "Region::insertIntoDatabase(QList<Region> regions):  error inserting into table Regions";
			qDebug() << query.lastError().text();
			db.close();
			return false;
		}
	db.close();
	return true;
}


