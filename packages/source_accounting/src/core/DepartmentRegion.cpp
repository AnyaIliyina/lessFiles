#include "DepartmentRegion.h"

DepartmentRegion::DepartmentRegion(int department_id, int region_id):
	m_department_id(department_id), m_region_id(region_id)
{
	m_department_id = department_id;
	m_region_id = region_id;
}

DepartmentRegion::~DepartmentRegion()
{
}

void DepartmentRegion::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO department_regions( department_id, region_id)\
		VALUES (?, ?)");
	query.addBindValue(m_department_id);
	query.addBindValue(m_region_id);
	if (!query.exec()) {
		qDebug() << "departmentRegion::insertIntoDatabase():  error ";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "DepartmentRegion: insert", 0, errorString);
	}
	else {
		int id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "DepartmentRegion: insert", id);
	}
}

QList<int> DepartmentRegion::regionsByDepartment(int department_id)
{
	QList<int>regionIds;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT region_id FROM department_regions WHERE department_id =?");
	query.addBindValue(department_id);
	if (!query.exec())
	{
		qDebug() << "DepartmentRegion::regionsByDepartment(int department_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	while (query.next())
		regionIds << query.value(0).toInt();

	db.close();
	return regionIds;
}

bool DepartmentRegion::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  department_regions (\
		department_id INTEGER,		\
		region_id INTEGER,   \
		FOREIGN KEY(department_id) REFERENCES departments(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(region_id) REFERENCES regions(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating Department_regions table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	DepartmentRegion(1, 1).insertIntoDatabase();
	return true;
}

void DepartmentRegion::deleteRecord(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	QString depart_idstr = QString::number(m_department_id);
	QString region_idstr = QString::number(m_region_id);
	if (!query.exec("DELETE FROM department_regions WHERE ((department_id=\'" + depart_idstr + "\') AND (region_id=\'" +
		region_idstr + "\'))"))
	{
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		Log::create(session_id, "DepartmentRegion::deleteRecord", m_department_id, errorString);
	}
	else
	{
		Log::create(session_id, "DepartmentRegion::deleteRecord", m_department_id);
	}
}
