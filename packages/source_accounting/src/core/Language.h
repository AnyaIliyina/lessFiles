#pragma once

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

/*!
	\file
    \brief Языки
		(таблица languages)
    \author Ильина А.
	\date май 2016
*/

class Language {
private:
	int m_id;
	QString m_name;
	

public:
	/*!
	Создает таблицу "languages" в базе.
	\return true - если таблица успешно создана
	*/
	static bool createTable();

	/*!
	Заполняет таблицу languages начальными значениями
	return true - успешно */
	static bool completeTable();

	/*!
	Записывает в базу данных список языков
	\param QStringList languageNames - список языков
	\return true - список языков записан
	\return false - возникли ошибки*/
	static bool insert(QStringList languageNames);


	/*!
	Возвращает список языков
	\return QStringList languages*/
	static QStringList getList();
	
	/*!
	Возвращает список языков определенного сайта
	\return QStringList languages*/
	static QStringList getList(int id);

	/*!
	Возвращает список id языков по их названию
	\return QList <int> ids*/
	static QList <int> getIDs(QStringList listLang);
};

