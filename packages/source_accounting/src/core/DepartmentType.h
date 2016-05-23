#pragma once
#include <QObject>
#include <QString>
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Database.h"
#include "Log.h"

/*!
\file
\brief    Тип ГПИ, предоставляемой ведомством
(связующая таблица department_types)
\author   Ильина А.
\date     май 2016
*/

class DepartmentType {
private:
	int m_department_id;
	int m_type_id;
public:
	DepartmentType(int department_id, int type_id);

	~DepartmentType();

	/*! Записывает в базу данных информацию
	о паре department_id - type_id
	\param int session_id - id текущей сессии*/
	void insertIntoDatabase(int session_id = Database::currentSessionId());


	/*! Создает таблицу department_types*/
	static bool createTable();

	/*! Удаляет все пары department_id - type_id,
	в которых упоминается департамент department_id
	\param int department_id - id департамента
	*/
	static void deleteByDepartment(int department_id);
};

