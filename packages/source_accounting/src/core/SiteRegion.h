#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "Database.h"
#include "Log.h"

/*!
\file
\brief    Регионы, ГП-информацию о которых предоставляет интернет-ресурс
(связующая таблица site_regions)
\author   Ильина А.
\date     май 2016
*/


class SiteRegion {
private:
	int m_site_id;
	int m_region_id;
public:	
	/*! Конструктор. Создает пару site_id -  region_id.
	\param int site_id - id интернет-ресурса
	\param int region_id - id региона*/
	SiteRegion(int site_id, int region_id);

	/*! Деструктор*/
	~SiteRegion();

	/*! Записывает в базу данных информацию о паре site_id - region_id
	*/
	void insertIntoDatabase(int session_id = Database::currentSessionId());


	/*! Возвращает список интернет-ресурсов,
	предоставляющих информацию о регионе
	\param int region_id - id региона
	\return QList<int>siteIds - список id интернет-ресурсов*/
	static QList<int>sitesByRegion(int region_id);

	/*! Возвращает список регионов, информацией о
	которых располагает интернет-ресурс
	\param int site_id - id интернет-ресурса
	\return QList<int> regionIds - список из id регионов*/
	static QList<int>regionsBySite(int site_id);

	/*! Удаляет из базы запись
	*/
	void deleteRecord(int session_id = Database::currentSessionId());

	/*! Создает таблицу site_regions*/
	static bool createTable();
};

