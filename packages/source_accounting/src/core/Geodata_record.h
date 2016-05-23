#pragma once

#include <QString>
#include <QDebug>
#include "Database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>



/*!
	\file
	\brief    Представляет сущность Георесурс
			(таблица Geodata_records)
	\author   Козырева О., Ильина А.
	\date     15 март 2016
*/

class Geodata_record {
private:
	int m_id;
	int m_site_id;
	int m_session_id;
	int m_format_id;
	int m_scale_id = 1;
	int m_state_id;
	QString m_place_name;
	QString m_url="";
	QString m_comment="" ;
public:
	/*! Конструктор по умолчанию */
	Geodata_record();	

	/*!
	Возвращает id георесурса
	\return int record_id - id ресурса
	*/
	int id();

	/*!
	Возвращает адрес ресурса
	\return const QString& url - адрес ресурса
	*/
	const QString& url() const;

	/*!
	Возвращает значение актуальности
	\return int state_id 
	*/
	int state_id();


	/*!
	Возвращает id формата
	\return int format_id
	*/
	int format_id();


	/*!
	Возвращает название местности
	\return const QString& place_name - название местности
	*/
	const QString& place_name() const;

	/*! 
	Определяет, присвоены ли значения ключевым полям
	\return true - обязательные поля инициализированы 
	\return false - обязательные поля не инициализированы */
	bool required_fields_filled();

	/*! Устанавливает значение id записи
	\param int record_id - новый id записи*/
	void setRecordId(int record_id);

	/*! Устанавливает значение id сайта
	\param int site_id - новый id сайта*/
	void setSiteId(int site_id);

	/*! Устанавливает значение id формата
	\param int format_id - новый id формата*/
	void setFormateId(int id);

	/*! Устанавливает значение id сессии
	\param int session_id - новый id сессии*/
	void setSessionId(int session_id);

	/*! Устанавливает актуальность георесурса в state_id
	\param int state_id*/
	void setStateId(int state_id);

	/*! Устанавливает новое название местности
	\param const QString& placename - название местности*/
	void setPlacename(const QString& placename);

	/*! Устанавливает новый адрес ресурса
	\param const QString& url - адрес ресурса, */
	void setUrl(const QString& url);


	/*!
	Конструирует объект класса Geodata_record из параметров
	\param int site_id - id сайта:
	\param int session_id - id сессии;
	\param int format_id - id формата;
	\param int scale_id - id масштаба;
	\param int state_id - id состояния (актуальность);
	\param const QString& place_name - название местности;
	\param const QString& url - адрес ресурса;
	\param const QString& commant - комментарий;
	*/
	Geodata_record(int site_id, int format_id, const QString& place_name,
		int session_id=1, int scale_id=1, int state_id=1, const QString& url=" ",
		const QString& comment =" ");
	
	/*! Деструктор */
	~Geodata_record();


	//______  Методы, обращающиеся к базе: _____________

	/*!
	Создает Георесурс из данных в базе
	\param int id - id георесурса
	*/
	Geodata_record(int id);

	/*!
	Записывает геопространственную информацию в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();

	/*!
	Обновляет запись в базе данных
	*/
	void updateRecord();
	
	/*!
	Создает таблицу "geodata_records" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
		
	/*!
	Удаляет запись по ее id;
	\param id - record_id;
	\param session_id - id настоящей сессии
	*/
	static void deleteRecord(int & id, int session_id = Database::currentSessionId());

	/*!
	Удаляет записи, созданные пользователем author_id и относящиеся к сайту site_id
	\param site_id - id сайта
	\param author_id - id пользователя*/
	static void deleteRecords(int site_id, int author_id);

	/*!Удаляет записи, относящиеся к сайту site_id
	и созданные модулем поиска до текущей сессии
	\param int site_id - id сайта
	\param session_id - id настоящей сессии*/
	static void deleteOldSmRecords(int site_id, int session_id = Database::smSessionId());
};
