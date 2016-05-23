#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>

/*!
*  \brief Представляет сущность Тип пользователя
			(таблица usertypes)
*  \author Козырева О.
*  \date		15 марта 2016
*/

class Usertype : public QObject{
	Q_OBJECT
private:
	int m_id;
	QString m_name;

public:
	QString type_name();
	
	/*!
	Конструирует объект класса Usertype из параметров
	\param QString type_name - название создаваемого типа
	*/
	Usertype(QString type_name);
	
	/*!
	Конструирует объект класса Usertype из данных в базе
	\param int id - id статуса в базе
	*/
	Usertype(int id);
	
	/*!
	Деструктор
	*/
	~Usertype();
	
	/*!
	Возвращает id типа пользователя
	\return int type_id - id типа пользователя
	*/
	int id();
	
	/*!
	Записывает информацию о типе пользователя в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();
	
	/*!
	Создает таблицу "usertypes" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!
	Заполняет таблицу "usertypes" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();

	/*!
	Записывает в базу данных список типов пользователей
	\param QStringList typeNames - список названий типов
	\return true - статусы записаны в БД
	\return false - возникли ошибки*/
	static bool insert(QStringList typeNames);
};

