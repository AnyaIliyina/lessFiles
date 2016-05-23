/*!

	\file
	\brief Базовый элемент
	\author Чернышев Д.Р.
	\date Апрель 2016

*/

#pragma once

#include <QList>
#include <QVariant>

class BaseItem
{
public:
	virtual ~BaseItem();
	BaseItem();

	virtual int rowCount() const;
	virtual int columnCount() const = 0;

	BaseItem* parent() const;
	virtual BaseItem* child(int row) const;

	virtual void appendChild(BaseItem* child);
	virtual void removeChild(BaseItem* child) = 0;

	int row() const;

	virtual QVariant data(int column, int role) const = 0;
	virtual bool setData(int column, const QVariant& value, int role) = 0;

	virtual QVariant headerData(int section, int role) const = 0;

	virtual bool isValid() const = 0;
	virtual bool isNew() const = 0;
	virtual bool hasChildren() const;


	/*! Определяет, есть ли в модели столбец с чек-боксом (для роли Qt::CheckStateRole.)
	\return true - столбец есть*/
	virtual bool isCheckable() const;		

	virtual bool save() = 0;
	virtual bool cancel() = 0;

	virtual QList<BaseItem*> loadItemsFromDb(QVariant id = QVariant()) = 0;

protected:
	BaseItem* m_parent = NULL;
	QList<BaseItem*> m_children;
};