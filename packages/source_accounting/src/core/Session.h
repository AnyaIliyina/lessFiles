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
*  \brief Представляет сущность Сессия
			(таблица sessions)
*  \author Козырева О.
*  \date	15 марта 2016
*/

class Session: public QObject  {
	Q_OBJECT
private:
	int m_id;
	int m_user_id;
	QDateTime m_date;

public:
	
	Session(int user_id, QDateTime date);
	
	/*!
	Конструирует объект, используя информацию в базе
	\param int id- id сессии
	*/
	Session(int id);
	
	~Session();
	
	/*!
	Возвращает id сессии
	\return int session_id - id сессии
	*/
	int id();
	
	/*!
	Записывает информацию о сессии в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();
	
	/*!
	Создает таблицу "sessions" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!
	Создает сессию для пользователя user_id
	\param int user_id - id пользователя
	\return true - сессия создана*/
	static bool createSession(int user_id);

	/*!
	Создает сессию для модуля поиска
	\return true - сессия создана*/
	static bool createSMsession();

	/*!
	Создает сессию для Системы
	\return true - сессия создана*/
	static bool createSystemSession();

	/*!
	Заполняет таблицу "sessions" начальными значениями
	\return true - если записи успешно созданы
	*/
	static bool completeTable();

};


