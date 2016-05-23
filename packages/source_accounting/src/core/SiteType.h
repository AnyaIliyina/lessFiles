#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Database.h"
#include "Log.h"
#include <QSqlError>

/*!
\file
\brief    Типы ГПИ, предоставляемой интернет-ресурсом
(связующая таблица syte_types)
\author   Ильина А.
\date     май 2016
*/


class SiteType {
private:
	int m_site_id;
	int m_type_id;
public:
	/*! Конструктор. Создает пару site_id - type_id.
	\param int site_id - id интернет-ресурса
	\param int type_id - id типа ГПИ*/
	SiteType(int site_id, int type_id);
	
	/*! Деструктор */
	~SiteType();

	/*! Записывает в базу данных информацию
	о паре site_id - type_id
	\param int session_id - id текущей сессии
	*/
	void insertIntoDatabase(int session_id = Database::currentSessionId());

	/*! Возвращает список интернет-ресурсов,
	поддерживающих тип type_id
	\param int type_id - id типа ГПИ
	\return QList<int>siteIds - список id интернет-ресурсов*/
	static QList<int>sitesByType(int type_id);

	/*! Возвращает список типов ГПИ  интернет-ресурса
	\param int site_id - id интернет-ресурса
	\return QList<int> typeIds - список из id поддерживаемых языков*/
	static QList<int>typesBySite(int site_id);


	/*! Создает таблицу site_types
	\return true - таблица создана*/
	static bool createTable();

	/*! Удаляет все пары site_id - type_id,
	в которых упоминается интернет-ресурс site_id
	\param int site_id - id интернет-ресурса
	*/
	static void deleteBySite(int site_id);
};

