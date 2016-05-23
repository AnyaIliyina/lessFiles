#include "DepartmentType.h"

DepartmentType::DepartmentType(int department_id, int type_id):
	m_department_id(department_id), m_type_id(type_id)
{
}

DepartmentType::~DepartmentType()
{
}

void DepartmentType::insertIntoDatabase(int session_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("INSERT INTO department_types( department_id, type_id)\
		VALUES (?, ?)");
	query.addBindValue(m_department_id);
	query.addBindValue(m_type_id);
	if (!query.exec()) {
		qDebug() << "departmentType::insertIntoDatabase():  error ";
		QString errorString = query.lastError().text();
		qDebug() << errorString;
		db.close();
		Log::create(session_id, "DepartmentType: insert", 0, errorString);
	}
	else {
		int id = query.lastInsertId().toInt();
		db.close();
		Log::create(session_id, "DepartmentType: insert", id);
	}
}

bool DepartmentType::createTable()
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if ((!query.exec("CREATE TABLE IF NOT EXISTS  department_types (\
		department_id INTEGER,		\
		type_id INTEGER,   \
		FOREIGN KEY(department_id) REFERENCES departments(id) ON UPDATE CASCADE ON DELETE CASCADE,\
		FOREIGN KEY(type_id) REFERENCES geodata_types(id) ON UPDATE CASCADE ON DELETE CASCADE\
		)"
		)))
	{
		qDebug() << "error creating Department_types table in database.";
		qDebug() << query.lastError().text();
		db.close();
		return false;
	}
	db.close();
	DepartmentType(1, 1).insertIntoDatabase();
	return true;
}

void DepartmentType::deleteByDepartment(int department_id)
{
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("DELETE FROM department_types WHERE department_id =?");
	query.addBindValue(department_id);
	if (!query.exec())
	{
		qDebug() << "DepartmentType::deleteByDepartment(int department_id) error";
		qDebug() << query.lastError().text();
		db.close();
	}
	db.close();
}

