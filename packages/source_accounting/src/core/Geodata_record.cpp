#include "Geodata_record.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Database.h"
#include "Log.h"
#include <QTextCodec>

Geodata_record::Geodata_record(int site_id, int format_id, const QString& place_name,
	int session_id, int state_id, int scale_id, const QString& url, const QString& comment)
{
	m_id = 0;
	m_site_id = site_id;
	m_session_id = session_id;
	m_format_id = format_id;
	m_scale_id = scale_id;
	m_state_id = state_id;
	m_url = url;
	m_place_name = place_name;
	m_comment = comment;
}


Geodata_record::Geodata_record()
{
}

const QString & Geodata_record::url() const
{
	return m_url;
}

int Geodata_record::state_id()
{
	return m_state_id;
}

int Geodata_record::format_id()
{
	return m_format_id;
}


bool Geodata_record::required_fields_filled()
{
	if (m_site_id <= 0)
		return false;
	if (m_format_id<=0)
		return false;
	if (m_session_id <= 0)
			return false;
	if (m_place_name == "")
		return false;

	return true;

}

void Geodata_record::setRecordId(int record_id)
{
	m_id = record_id;
}

void Geodata_record::setSiteId(int site_id)
{
	m_site_id = site_id;
}

void Geodata_record::setFormateId(int id)
{
	m_format_id = id;
}

void Geodata_record::setSessionId(int session_id)
{
	m_session_id = session_id;
}

void Geodata_record::setStateId(int state_id)
{
	m_state_id = state_id;
}

void Geodata_record::setPlacename(const QString& placename)
{
	m_place_name = placename;
}

void Geodata_record::setUrl(const QString & url)
{
	m_url = url;
}


const QString& Geodata_record::place_name() const
{
	return m_place_name;
}

Geodata_record::Geodata_record(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("geodata_records");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString place_name = model.record(0).value("place_name").toString();
	int site_id = model.record(0).value("site_id").toInt();
	int session_id = model.record(0).value("session_id").toInt();
	int format_id = model.record(0).value("format_id").toInt();
	int scale_id = model.record(0).value("scale_id").toInt();
	int state_id = model.record(0).value("state_id").toInt();
	QString comment = model.record(0).value("comment").toString();
	QString url = model.record(0).value("url").toString();
	db.close();
	
	m_id = id;
	m_site_id = site_id;
	m_session_id = session_id;
	m_format_id = format_id;
	m_scale_id = scale_id;
	m_state_id = state_id;
	m_place_name = place_name;
	m_comment = comment;
	m_url = url;
}

Geodata_record::~Geodata_record()
{
}

int Geodata_record::id()
{
	return m_id;
}


bool Geodata_record::insertIntoDatabase()
{
	if (!required_fields_filled())
	{
		qDebug() << "Geodata_record::insertIntoDatabase(): record not valid";
		return false;
	}
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO geodata_records ( site_id, session_id, format_id, scale_id, state_id, place_name, comment, url)\
		VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
	query.addBindValue(m_site_id);
	query.addBindValue(m_session_id);
	query.addBindValue(m_format_id);
	query.addBindValue(m_scale_id);
	query.addBindValue(m_state_id);
	query.addBindValue(m_place_name);
	query.addBindValue(m_comment);
	query.addBindValue(m_url);
	if (!query.exec()) {
		qDebug() << "Geodata_record::insertIntoDatabase():  error inserting into Table geodata_records";
		QString error_string = query.lastError().text();
		qDebug() << error_string;
		db.close();
		Log::create(m_session_id, "Geodata_record: insert",  0, error_string);
		return false;
	}
	m_id = query.lastInsertId().toInt();
	db.close();
	Log::create(m_session_id, "Geodata_record: insert", m_id);	
	return true;
}

void Geodata_record::updateRecord()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("UPDATE geodata_records\
					SET\
					site_id =:site_id, session_id=:session_id, format_id=:format_id,\
					scale_id=:scale_id, state_id=:state_id, place_name=:place_name,\
					comment=:comment, url=:url WHERE id=:id");
	query.bindValue(":site_id", m_site_id);
	query.bindValue(":session_id", m_session_id);
	query.bindValue(":format_id", m_format_id);
	query.bindValue(":scale_id", m_scale_id);
	query.bindValue(":state_id", m_state_id);
	query.bindValue(":place_name", m_place_name);
	query.bindValue(":comment", m_comment);
	query.bindValue(":url", m_url);
	query.bindValue(":id", m_id);
	if (!query.exec()) {
		qDebug() << "Geodata_record::updateRecord():  error updating geodata_records";
		QString errorString = query.lastError().text();
		qDebug() << errorString;		
		db.close();
		Log::create(m_session_id, "Geodata_record: update", m_id, errorString);
	}
	db.close();
	Log::create(m_session_id, "Geodata_record: update", m_id);
}

bool Geodata_record::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (!query.exec("CREATE TABLE IF NOT EXISTS  geodata_records (\
		id  INTEGER         PRIMARY KEY AUTOINCREMENT, \
		site_id INTEGER,		\
		session_id INTEGER,		\
		format_id INTEGER,		\
		scale_id INTEGER,		\
		state_id INTEGER,		\
		place_name     TEXT   NOT NULL,\
		comment TEXT,   \
		url TEXT,   \
		FOREIGN KEY(site_id) REFERENCES sites(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(session_id) REFERENCES sessions(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(format_id) REFERENCES formats(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(scale_id) REFERENCES scales(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(state_id) REFERENCES states(id) ON UPDATE CASCADE ON DELETE CASCADE\
		 )"
		))
	{
		qDebug() << "error creating geodata_records Table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;

}

void Geodata_record::deleteRecord(int& id, int session_id)
 {
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
 	qDebug() << id;
 	QString idstr = QString::number(id);
 	if (!query.exec("DELETE FROM geodata_records WHERE id=\'" + idstr + "\'"))
	{
  		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "Geodata_record: delete", id, errorString);
 	}
 	else
 	{
 		Log::create(session_id, "Geodata_record: delete", id);
 	}
 }

void Geodata_record::deleteRecords(int site_id, int author_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("DELETE FROM geodata_records \
	WHERE site_id=:site_id\
	AND session_id = (\
					SELECT id FROM sessions\
					WHERE user_id =:user_id\
					)"
		);
	query.bindValue(":site_id", site_id);
	query.bindValue(":user_id", author_id);
	if (!query.exec()) {
		qDebug() << "Geodata_record::deleteRecords(int site_id, int author_id):  error";
		qDebug() << query.lastError().text();
		db.close();
	}
	db.close();
}

void Geodata_record::deleteOldSmRecords(int site_id, int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("DELETE FROM geodata_records \
	WHERE geodata_records.site_id=:site_id\
	AND geodata_records.session_id = (\
					SELECT id FROM sessions\
					WHERE sessions.user_id = 1 AND sessions.id<:session_id\
					)"
		);
	query.bindValue(":site_id", site_id);
	query.bindValue(":session_id", Database::smSessionId());
	if (!query.exec()) {
		qDebug() << "Geodata_record::deleteOldSmRecords(int site_id):  error";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "Geodata_records: delete (Old, Sm, by site_id)", site_id, errorString);
		db.close();
	}
	db.close();
	Log::create(session_id, "Geodata_records: delete (Old, Sm, by site_id)", site_id);
}
