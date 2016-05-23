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
    \brief Представляет сущность Масштаб
		(таблица scales)
    \author Козырева О.
	\date 10 марта 2016
*/

class Scale: public QObject {
	Q_OBJECT
private:
	int m_id;
	QString m_description;

public:
		
	/*!
	Конструирует объект класса Scale
	\param QString description - масштаб 
	*/
	Scale(QString description);

	/*!
	Деструктор
	*/
	~Scale();
	
	/*!
	Возвращает машстаб
	*/
	QString description();

	/*!
	Конструирует объект класса Scale из данных в базе
	\param int id - id сайта в базе
	*/
	Scale(int id);
	
	/*!
	Возвращает id масштаба
	*/
	int id();

	/*!
	Возвращает id заданного масштаба
	\param QString description - масштаб
	*/
	static int scale_id(QString description);
	
	/*!
	Записывает информацию о масштабе в базу данных.
	\return true - если запись в БД успешно добавлена
	*/
	bool insertIntoDatabase();
	
	/*!
	Создает таблицу "scales" в базе данных.
	\return true - если таблица успешно создана
	*/
	static bool createTable();
	
	
	/*!
	Заполняет таблицу "scales" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();

	/*!
	Получает список всех масштабов из базы
	\return QStringList listScales - список масштабов
	*/
	static QStringList getDescription();

	/*!
	Записывает в базу данных список масштабов
	\param QStringList descriptions - список названий масштабов
	\return true - масштабы записаны в БД
	\return false - возникли ошибки*/
	static bool insert(QStringList descriptions);

	/*!
	Метод для получения строки в кодировке Unicode 
	\param QByteArray encodedStr - строка с кодировкой Windows-1251
	\return QString string - строка в кодировке Unicode
	*/
	static QString coded(QByteArray encodedStr);
};

