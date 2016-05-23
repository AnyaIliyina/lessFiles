#pragma once

#include "Base_item.h"
#include "RegionItem.h"
#include "Item_factory.h"
#include <QModelIndex>
/*!

\file
\brief	Элемент Item_model. 
Содержит информацию о регионе, отображается QTreeView с чек-боксами.
\author Ильина А.
\date май 2016
*/

class RegionItemChecked : public RegionItem
{

public:
	RegionItemChecked();
	virtual ~RegionItemChecked();
	int id = 0;

	/*! Возвращает информацию, хранимую в столбике column для роли role
	\param int column - номер колонки
	\param int role
	\return QVariant data*/
	QVariant data(int column, int role) const Q_DECL_OVERRIDE;

	/*! Записывает информацию для роли role в data колонки column
	\param int column - номер колонки 
	\param onst QVariant& value -  информация, новое значение data
	\param int role - роль
	\return bool - значение для data присвоено*/
	bool setData(int column, const QVariant& value, int role) Q_DECL_OVERRIDE;
	
	/*! Возвращает число столбцов
	\return int columnCount - число столбцов*/
	int columnCount() const Q_DECL_OVERRIDE;

	/*! Определяет, есть ли в модели столбец с чек-боксом (для роли Qt::CheckStateRole.)
	\return true - столбец есть*/
	bool isCheckable() const Q_DECL_OVERRIDE;

	/*! Загружает данные из БД
	\return QList<BaseItem*> - список загруженных элементов*/
	QList<BaseItem*> loadItemsFromDb(QVariant id = QVariant()) Q_DECL_OVERRIDE;

	/*! Устанавливает заголовки
	\param int section - число столбцов
	\param int role - роль, для которой прописаны заголовки
	\return QVariant*/
	QVariant headerData(int section, int role) const Q_DECL_OVERRIDE;
	
	/*! Возвращает QMap <m_id региона, RegionItemChecked*>, содержащую текущие айтемы 
	\return QMap<int, RegionItemChecked*>*/
	static QMap<int, RegionItemChecked*> getMap();

	/*! Устанавливает значение m_checked
	\param bool checked - новое значение m_checked*/
	void setChecked(bool checked);
	
	/*! Возвращает значение m_checked
	\return bool m_checked */
	bool isChecked();

	/*! Выводит элементы из режима редактирования
	\return true - значение m_checked изменилось за время редактирования
	\return false - значение m_checked не изменилось*/
	bool save() Q_DECL_OVERRIDE;

	/*! Присваивает полю m_checked всех айтемов значение false*/
	void uncheckAll();

private:
	/*! Присваивает потомкам элемента parent новое значение m_checked
	\param RegionItemChecked* parent - элемент
	\param bool newCheckState - новое значение m_checked*/
	void checkChildren(RegionItemChecked* parent, bool newCheckState);
	
	bool m_checked=true;	// Элемент отмечен/не отмечен - состояние чек-бокса
	bool m_old_checked;		// Предыдущее состояние чек-бокса
	static QMap<int, RegionItemChecked*> map;	// QMap вида <"m_id региона", "указатель на item региона">
	
	RegionItemChecked* rootItem();	
};