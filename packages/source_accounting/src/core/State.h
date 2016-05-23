#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>

/*!
*  \brief Представляет сущность Состояние
			(таблица states)
*  \author Козырева О.
*  \date	9 марта 2016
*/

class State : public QObject {
	Q_OBJECT
private:
	int m_id;
	QString m_name;

public:
	QString state_name();
	
	/*!
	Конструирует объект класса State из параметров
	\param const QString state_name - название создаваемого статуса
	*/
	State(QString state_name);
	
	/*!
	Конструирует объект класса State из данных в базе
	\param int id - id статуса в базе
	*/
	State(int id);
	
	/*!
	Деструктор
	*/
	~State();
	
	/*!
	Возвращает id статуса
	\return int state_id - id статуса
	*/
	int id();
	
	/*!
	Возвращает id статуса
	\return int state_id - id статуса
	*/
	static int state_id(QString state_name);

	/*!
	Записывает информацию о статусе в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();
	
	/*!
	Создает таблицу "states" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	/*!
	Заполняет таблицу "states" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();

	/*!
	Записывает в базу данных список состояний
	\param QStringList stateNames - список названий состояний
	\return true - статусы записаны в БД
	\return false - возникли ошибки*/
	static bool insert(QStringList stateNames);
	
	/*!
	Получает список всех статусов из базы
	\return QStringList listStates - список статусов
	*/
	static QStringList getStates();
};
