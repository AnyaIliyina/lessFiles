#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QUrl>
#include <QDateTime>
#include "Database.h"


/*!
	\file
	\brief    Ведомства
(таблица departments)
	\author  Ильина А.
	\date    май 2016
*/


class Department: public QObject {
	Q_OBJECT
private:
	int m_id;
	QString m_name;
	QString m_country;
	QString m_address;
	QString m_mail;
	QString m_fax = "";
	QString m_phone;
	QString m_comment = "";
	static QStringList languages;
public:
	Department();
	~Department();
	
	/*!
	Возвращает id ведомства
	\return int id
	*/
	int id() const;

	/*!
	Возвращает название ведомства
	\return const QString& name - название*/
	const QString & name() const;

	/*! Возвращает страну, в которой находится ведомство
	return const QString& country - страна*/
	const QString & country() const;

	/*!
	Возвращает адрес ведомства
	\return const QString& address - адрес*/
	const QString & address() const;

	/*!
	Возвращает e-mail ведомства
	\return const QString& mail - адрес*/
	const QString & mail() const;

	/*!
	Возвращает телефон ведомства
	\return const QString&  phone - телефон*/
	const QString & phone() const;

	/*!
	Возвращает факс ведомства
	\return const QString& fax - факс*/
	const QString & fax() const;

	/*!
	Возвращает комментарий
	\return const QString& comment - комментарий*/
	const QString & comment() const;

	/*!
	Присваивает ведомству название
	\param const QString& name - имя*/
	void setName(const QString& name);

	/*!
	Устанавливает значение поля Страна.
	\param const QString& name - название страны, в которй находится ведомство*/
	void setCountry(const QString& country);

	/*!
	Присваивает телефон ведомству.
	\param const QString& number - телефонный номер*/
	void setPhone(const QString& phone);

	/*!
	Присваивает адрес ведомству.
	\param const QString& address - адрес*/
	void setAdress(const QString& address);

	/*!
	Присваивает ведомству факс
	\param const QString& fax */
	void setFax(const QString& fax);

	/*!
	Присваивает email ведомству
	\param const QString& mail */
	void setMail(const QString& mail);

	/*!
	Добавляет комментарий
	\param const QString& comment */
	void setComment(const QString& comment);


	// ----------- Методы, обращающиеся к базе ----------
	/*!
	Конструирует объект класса Department из параметров
	\param const QString& name - название департамента;
	\param const QString& country - страна  департамента;
	\param const QString& address - адрес  департамента;
	\param const QString& mail - электронная почта департамента;
	\param const QString& fax - факс департамента;
	\param const QString& phone -телефон  департамента;
	\param const QString& comment;
	*/
	Department(const QString& name, const QString& country, const QString& address, const QString& mail,
	 const QString& phone, const QString& fax="", const QString& comment = "");

	/*! Конструктор
	Получает информацию из БД 
	\param int id - id ведомства*/
	Department(int id);
	
	/*! Записывает в БД информацию о ведомстве
	\param session_id - id текущей сессии
	\return true - информация записана*/
	bool insertIntoDatabase(int session_id = Database::currentSessionId());

	/*! Изменяет информацию о ведомстве в базе
	\param session_id - id текущей сессии
	\return true - запись успешно изменена*/
	bool update(int session_id=Database::currentSessionId());

	/*! Удаляет ведомство из базы
	\param department_id - id ведомства
	\param session_id - id текущей сессии*/
	static bool deleteDepartment(int department_id, 
				int session_id = Database::currentSessionId());

	/*! Создает в БД таблицу Ведомства
	\return true - таблица создана*/
	static bool createTable();

	/*!
	Заполняет таблицу "departments" в БД начальными значениями.
	\return true - если таблица успешно заполнена
	*/
	static bool completeTable();


	/*! Устанавливает значение id департамента
	\param int id - id департамента*/
	void setId(int id);
};
