#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "Database.h"

/*!
*	\file
*	\brief Типы ГПИ
(таблица geodata_types)
*	\author Ильина А.
*	\date	май 2016
*/

class GeodataType {
private:
	int m_id;
	QString m_name;

public:
	/*!
	Создает таблицу geadata_types в базе.
	\return true - если таблица успешно создана
	*/
	static bool createTable();

	/*!
	Заполняет таблицу geadata_types начальными значениями
	return true - успешно */
	static bool completeTable();

	/*!
	Записывает в базу данных список типов ГПИ
	\param QStringList typeNames - список типов
	\return true - список языков записан
	\return false - возникли ошибки*/
	static bool insert(QStringList typeNames);


	/*!
	Возвращает список типов ГПИ
	\return QStringList typeNames - названия типов*/
	static QStringList getList();

	/*!
	Возвращает список типов ГПИ определенного сайта
	\param int id- id сайта
	\return QStringList typeNames - названия типов*/
	static QStringList getListForSites(int id);

	/*!
	Возвращает список типов ГПИ определенного департамента
	\param int id- id департамента
	\return QStringList typeNames - названия типов*/
	static QStringList getListForDepartments(int id);
	
	/*!
	Возвращает список id гпи по их названию
	\return QList <int> ids*/
	static QList <int> getIDs(QStringList listGPI);
};

