#pragma once


#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>
#include "Database.h"


/*!
	\file
	\brief    Представляет сущность Регион
(таблица regions)
	\author  Ильина А.
	\date     май 2016
*/


class Region{
private:
	int m_id;
	int m_parent_id;
	QString m_name;
	QString m_comment;
public:
	
	/*! Конструктор по умолчанию*/
	Region();

	/*! Конструктор
	\param int parent_id - id региона-предка
	\param const QString& name - название региона
	\param const QString& comment - комментарий*/
	Region(int parent_id, const QString& name, const QString& comment = "");
	
	/*! Деструктор*/
	~Region();

	/*! Возвращает id региона
	\return int id*/
	int id() const;

	/*! Возвращает id региона-предка
	\return int parent_id*/
	int parent_id() const;

	/*! Возвращает название региона
	\return const QString& name - название*/
	const QString& name() const;

	/*! Возвращает комментарий
	\return const QString& comment - комментарий*/
	const QString& comment() const;

	/*! Устанавливает id региона
	\param int id */
	void setId(int id);

	/*! Устанавливает id региона-предка
	\param int parent_id */
	void setParentId(int parent_id);

	/*! Устонавливает название региона 
	\param const QString & name - название*/
	void setName(const QString & name); 

	/*! Устонавливает комментарий
	\param const QString & comment - комментарий*/
	void setComment(const QString & comment);

	// ----------- Методы, обращающиеся к базе ----------
	/*! Конструктор
	Получает информацию из БД
	\param int id - id региона*/
	Region(int id);

	/*! Записывает в БД информацию о регионе
	\param session_id - id текущей сессии
	\return true - информация записана*/
	bool insertIntoDatabase(int session_id = Database::currentSessionId());

	/*! Изменяет информацию о регионе в базе
	\param session_id - id текущей сессии
	\return true - запись успешно изменена*/
	bool update(int session_id = Database::currentSessionId());

	/*! Удаляет регион из базы
	\param department_id - id ведомства
	\param session_id - id текущей сессии*/
	static bool deleteRegion(int region_id,
		int session_id = Database::currentSessionId());

	/*! Создает в БД таблицу Регионы
	\return true - таблица создана*/
	static bool createTable();

	/*!
	Заполняет таблицу "regions" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();

	/*!
	Записывает информацию о регионах из списка в базу
	\param QList<Site> regions - список регионов
	\return true - информация записана
	\false -  возникли ошибки
	*/
	static bool insert(QList<Region> regions);

};
