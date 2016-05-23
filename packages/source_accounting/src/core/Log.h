#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>

/*!
\file
*/

/*!
*  \brief    Краткая запись истории действий с базой
(таблица Logs)
*  \author   Ильина А.
*  \date     май 2016
*/


class Log: public QObject {
	Q_OBJECT
private:
	int m_id = 0;
	int m_session_id;
	QString m_operation_name;
	int m_object_id = 0;
	QString m_error_string = "";
	
	/*!
	Записывает лог в базу.
	\return true - лог записан*/
	bool insertIntoDatabase();

public:
	/*! Конструктор
	\param int session_id - id сессии;
	\param QString operation_name - название операции
	\param int object_id - id измененного/добавленного/удаленного объекта
	\param QString error_string - сообщение об ошибке*/
	Log(int session_id, QString operation_name, int object_id = 0, QString error_string = "");

	/*!Деструктор*/
	~Log();
	
	/*!
	Создает таблицу "logs" в базе.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!Создает лог и записывает его в БД
	\param int session_id - id сессии;
	\param QString operation_name - название операции
	\param int object_id - id измененного/добавленного/удаленного объекта
	\param QString error_string - сообщение об ошибке*/
	static void create(int session_id, QString operation_name, int object_id = 0, QString error_string = "");
};
