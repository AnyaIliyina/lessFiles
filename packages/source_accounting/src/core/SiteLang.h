#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Database.h"

/*!
\file
\brief    Языки, на которых работают сайты
(связующая таблица site_langs)
\author   Ильина А.
\date     май 2016
*/


class SiteLang {
private:
	int m_site_id;
	int m_language_id;
public:
	/*! Конструктор. Создает пару site_id - language_id.
	\param int site_id - id интернет-ресурса
	\param int language_id - id языка*/
	SiteLang(int site_id, int language_id);

	/*! Деструктор*/
	~SiteLang();

	/*! Записывает в базу данных информацию о паре site_id - language_id
	*/
	void insertIntoDatabase(int session_id = Database::currentSessionId());

	
	/*! Возвращает список интернет-ресурсов, 
	поддерживающих язык language_id
	\param int language_id - id языка
	\return QList<int>siteIds - список id интернет-ресурсов*/
	static QList<int>sitesByLanguage(int language_id);

	/*! Возвращает список языков интернет-ресурса
	\param int site_id - id интернет-ресурса
	\return QList<int> langIds - список из id поддерживаемых языков*/
	static QList<int>languagesBySite(int site_id);

	/*! Удаляет все пары site_id - language_id, 
	в которых упоминается интернет-ресурс site_id
	\param int site_id - id интернет-ресурса
	*/
	static void deleteBySite(int site_id);

	/*! Создает таблицу site_langs
	\return true - таблица создана*/
	static bool createTable();
};

