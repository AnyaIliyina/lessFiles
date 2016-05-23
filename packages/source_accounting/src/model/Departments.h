#pragma once

#include "Base_item.h"

/*!

\file
\brief	Наследник Item_model
\author Козырева О.А.
\date Апрель 2016

*/

class Departments : public BaseItem
{
public:
	int id=0;
	virtual ~Departments();
	Departments();

	/*!
	Возвращает количество колонок
	\return int - количество колонок
	*/
	int columnCount() const Q_DECL_OVERRIDE;

	void removeChild(BaseItem* child) Q_DECL_OVERRIDE;

	QVariant data(int column, int role) const Q_DECL_OVERRIDE;

	bool setData(int column, const QVariant& value, int role);

	/*!
	Метод устанавливает названия колонок
	\return QVariant
	*/
	QVariant headerData(int section, int role) const Q_DECL_OVERRIDE;


	bool isValid() const Q_DECL_OVERRIDE;
	bool isNew() const Q_DECL_OVERRIDE;
	bool hasChildren() const Q_DECL_OVERRIDE;

	bool save() Q_DECL_OVERRIDE;
	bool cancel() Q_DECL_OVERRIDE;


	QList<BaseItem*> loadItemsFromDb(QVariant id = QVariant()) Q_DECL_OVERRIDE;

private:

	int m_id=0;
	int m_session_id = 0;
	QString m_name;
	QString m_country;
	QString m_address;
	QString m_phone;
	QString m_fax;
	QString m_email;
	QString m_gpi;
	QString m_url;
	QString m_comment;
	int m_type = 0;

};