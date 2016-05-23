#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>

/*!
*  \brief Представляет сущность Пользователь
			(таблица users)
*  \author Козырева О.
*  \date		15 марта 2016
*/

class User {
	private:
	int m_id;
	int m_type_id;
	QString m_login;
	QString m_password;

public:
	QString login();
	
	/*!
	Конструирует объект класса User из параметров
	\param int type_id - идентификатор типа пользователя 
	\param QString login - логин пользователя
	\param  QString password - пароль пользователя
	*/
	User(int type_id, const QString& login, const QString& password);
	
	/*!
	Конструирует объект класса User из данных в базе
	\param int id - id пользователя в базе
	*/
	User(int id);
	
	~User();

	/*!
	Возвращает идентификатор типа
	\return int type_id*/
	int type_id() const;
	
	/*!
	Возвращает логин
	\return const QString& login*/
	const QString& login() const;

	/*!
	Возвращает пароль
	\return const QString& password
	*/
	const QString& password() const;

	/*!
	Возвращает идентификатор типа пользователя
	*/
	static int type_id(QString & login);

	/*!
	Возвращает id пользователя по логину
	\param const QString& login - логин
	\return int user_id - id пользователя
		либо -1, если пользователь не найден*/
	static int user_id(const QString& login);

	/*!
	Записывает в БД информацию о пользователях из списка
	\param QList<User>users - список пользователей;
	\return true - информация записана в БД;
	\return false - произошла ошибка;
	*/
	static bool insert(QList<User>users);

	/*!
	Возвращает id пользователя
	\return int user_id - id пользователя в базе
	*/
	int id();
	
	/*!
	Записывает информацию о пользователе в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();
	
	/*!
	Создает таблицу "users" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!
	Заполняет таблицу "users" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();
	
	/*!

	*/
	static bool userIsValid(const QString& login, const QString& password);
};

