#pragma once

#include "Database.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>


/*! 
   \file
   \brief    Интернет-ресурс (сайт)
			(таблица sites)
   \author    Ильина А., Козырева О.
   \date      март 2016
 */

class Site
{
private:
	int m_id;
	QString m_url;
	int m_status_id;
	QString m_name;
	QString m_comment;
	
public:
	
	/*!
	Возвращает адрес сайта
	\return QString& url - адрес сайта
	*/
	const QString& url() const;
	
	/*!
	Возвращает название сайта
	\return QString& site_name - название сайта
	*/
	const QString& name() const;
	
	/*!
	Возвращает комментарий
	\return QString& comment - комментарий
	*/
	const QString& comment() const;

	/*!
	Возвращает id сайта
	\return int site_id - id сайта;
	*/
	int id() const;

	/*!
	Возвращает id статуса
	\return int status_id - id статуса;
	*/
	int status_id() const;

	/*! Возвращает список языков сайта
	\return QStringList language - список языков*/
//	QStringList languages();

	/*!
	Изменяет статус сайта в базе
	\param int site_id - id сайта, статус которого нужно изменить;
	\param int status_id - id нового статуса
	\return true - статус изменен
	\return false - произошла ошибка*/
	static bool setStatusId(int site_id, int status_id, int session_id = Database::smSessionId());

	
	/*!
	Конструирует объект класса Site из параметров
	\param const QString& url - адрес(url) сайта;
	\param const QString& name - название сайта;
	\param int status;
	\param const QString& comment;
	*/
	Site(const QString& url, const QString& name, int status = 1, const QString& comment="");
	
	/*!
	Конструирует объект класса Site из данных в базе
	\param int id - id сайта
	*/
	Site(int id);
	
	~Site();
	
	/*!
	Возвращает id сайта
	\return int site_id - id сайта;
	*/
	static int site_id(QString &site_name);
		
	/*!
	Записывает информацию о сайте в базу данных.
	\return site_id
	*/
	 int insertIntoDatabase(int session_id = Database::currentSessionId());
	
	/*!
	Создает таблицу "sites" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!
	Заполняет таблицу "sites" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();

	/*! Возвращает id языков, поддерживаемых сайтом */
	QList<int> languageIDs();
	
	/*!
	\brief Возвращает список сайтов с определенным status_id
	\param int statusId - id статуса
	\return QList<Site> siteList - список сайтов со статусом statusId
	*/
	static QList<Site> sitesByStatus(int statusId);
		
	/*!
 	 Записывает информацию о сайтах из списка в базу
 	 \param (QList<Site> sites - список сайтов
 	 \return true - информация записана
 	 \false -  возникли ошибки
 	*/
 	 static bool insert(QList<Site> sites);

	 /*!
	 Проверяет, встречается ли адрес в таблице sites, колонке url
	 \param const QString& url - адрес
	 \return true - адрес не встречается
	 \return false - адрес занят*/
	 static bool urlIsNew(const QString& url);

	 /*!
	 Проверяет, есть ли в базе сайты со статусом 1
	 \return true - найден хотя бы один сайт со статусом 1*/
	 static bool uncheckedSitesFound();

	 /*!
	 Возращает список сайтов
	 \return QStringList listSites - список названий сайтов
	 */
	 static QStringList getSiteNames();

	 /*!
	 Проверяет url на валидность
	 */
	 static bool urlFromString(QString &string);

	 /*!
	 Удаляет запись по ее id;
	 \param id - id сайта;
	 \param session_id - id настоящей сессии
	 */
	 static void deleteRecord(int & id, int session_id = Database::currentSessionId());

	 /*! Устанавливает значение id сайта
	 \param int id - id сайта*/
	 void setId(int id);

	 /*!
	 Обновляет запись в базе данных
	 */
	 void updateRecord(int session_id= Database::currentSessionId());
};
