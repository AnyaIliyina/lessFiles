#include "Department.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Database.h"
#include "Log.h"


/*!
\file
\brief  
*/

Department::Department()
{
}

Department::~Department()
{
}

int Department::id() const
{
	return m_id;
}

const QString & Department::name() const
{
	return m_name;
}

const QString & Department::country() const
{
	return m_country;
}

const QString & Department::address() const
{
	return m_address;
}

const QString & Department::mail() const
{
	return m_mail;
}

const QString & Department::phone() const
{
	return m_phone;
}

const QString & Department::fax() const
{
	return m_fax;
}

const QString & Department::comment() const
{
	return m_comment;
}

void Department::setName(const QString & name)
{
	m_name = name;
}

void Department::setCountry(const QString & country)
{
	m_country = country;
}

void Department::setPhone(const QString & phone)
{
	m_phone = phone;
}

void Department::setAdress(const QString & address)
{
	m_address = address;
}

void Department::setFax(const QString & fax)
{
	m_fax = fax;
}

void Department::setMail(const QString & mail)
{
	m_mail = mail;
}

void Department::setComment(const QString & comment)
{
	m_comment = comment;
}

Department::Department(const QString & name, const QString & country, const QString & address, const QString & mail, const QString & phone, const QString & fax,  const QString & comment)
{
	m_id = 0;
	m_name = name;
	m_country = country;
	m_address = address;
	m_mail = mail;
	m_fax = fax;
	m_phone = phone;
	m_comment = comment;
}

Department::Department(int id)
{
	QSqlDatabase db = Database::database();
	QSqlTableModel model(nullptr, db);
	model.setTable("departments");
	const QString filter = QString("id == %1").arg(id);
	model.setFilter(filter);
	model.select();
	QString name = model.record(0).value("name").toString();
	QString country = model.record(0).value("country").toString();
	QString address = model.record(0).value("address").toString();
	QString mail = model.record(0).value("mail").toString();
	QString fax = model.record(0).value("fax").toString();
	QString phone = model.record(0).value("phone").toString();
	QString comment = model.record(0).value("comment").toString();
	db.close();

	m_id = id;
	m_name = name;
	m_country = country;
	m_address = address;
	m_mail = mail;
	m_fax = fax;
	m_phone = phone;
	m_comment = comment;
}

bool Department::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO departments ( name, country, address, mail, fax, phone, comment)\
		VALUES (?, ?, ?, ?, ?, ?, ?)");
	query.addBindValue(m_name);
	query.addBindValue(m_country);
	query.addBindValue(m_address);
	query.addBindValue(m_mail);
	query.addBindValue(m_fax);
	query.addBindValue(m_phone);
	query.addBindValue(m_comment);
	if (!query.exec()) {
		qDebug() << "Department::insertIntoDatabase():  error inserting into table Departments";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Department: insert", 0, errorString);
		return false;
	}
	else {
		m_id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "Department: insert", m_id);
		return true;
	}
}

bool Department::update(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("UPDATE departments\
					SET\
					name=:name, country=:country, \
					address=:address, mail=:mail, fax=:fax,\
					phone=:phone, comment=:comment \
						 WHERE id=:id");
	query.bindValue(":name", m_name);
	query.bindValue(":country", m_country);
	query.bindValue(":address", m_address);
	query.bindValue(":mail", m_mail);
	query.bindValue(":fax", m_fax);
	query.bindValue(":phone", m_phone);
	query.bindValue(":comment", m_comment);
	query.bindValue(":id", m_id);
	if (!query.exec()) {
		qDebug() << "Department::update():  error";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "Department: update", m_id, errorString);
		return false;
	}
	db.close();
	Log::create(session_id, "Department: update", m_id);
	return true;
}

bool Department::deleteDepartment(int department_id, int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString idstr = QString::number(department_id);
	if (!query.exec("DELETE FROM departments WHERE id=\'" + idstr + "\'"))
	{
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "Department: delete", department_id, errorString);
		return false;
	}
	else
	{
		Log::create(session_id, "Department: delete", department_id);
		return true;
	}
}

bool Department::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  departments (\
		id  INTEGER         PRIMARY KEY AUTOINCREMENT, \
		name     TEXT    UNIQUE NOT NULL,\
		country TEXT NOT NULL,\
		address TEXT NOT NULL,\
		mail     TEXT  NOT NULL ,\
		fax     TEXT   ,\
		phone    TEXT NOT NULL  ,\
		comment     TEXT   \
		)"
		)))
	{
		qDebug() << "error creating Departments table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	return true;
}

bool Department::completeTable()
{
	Department* d = new Department();
	d->setAdress("190031, г. Санкт-Петербург, Московский пр., д. 12");
	d->setCountry("Россия");
	d->setMail("hydep@hydrograph.spb.su");
	d->setName("ФГУП \"Гидрографическое предприятие\"");
	d->setPhone("(812) 310-37-68");
	d->setFax("(812) 310-37-68");
	bool succeded = d->insertIntoDatabase();
	delete d;
	return succeded;
}

void Department::setId(int id)
{
		m_id = id;
}
