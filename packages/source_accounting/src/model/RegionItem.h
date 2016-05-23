#pragma once

#include "Base_item.h"
#include "Region.h"

/*!

\file
\brief	Элемент Item_model.
Содержит информацию о регионе, отображается в TreeRegions.
\author Ильина А.
\date май 2016

*/

class RegionItem : public BaseItem
{
public:
	virtual ~RegionItem();
	
	/*! Конструктор */
	RegionItem();

	/*!
	Возвращает количество колонок
	\return int - количество колонок
	*/
	virtual int columnCount() const Q_DECL_OVERRIDE;

	/*! Удаляет из модели элемент child
	\param BaseItem* child*/
	void removeChild(BaseItem* child) Q_DECL_OVERRIDE;

	/*! Возвращает информацию, хранимую в столбике column для роли role
	\param int column - номер колонки
	\param int role
	\return QVariant data*/
	virtual QVariant data(int column, int role) const Q_DECL_OVERRIDE;

	/*! Записывает информацию для роли role в data колонки column
	\param int column - номер колонки
	\param onst QVariant& value -  информация, новое значение data
	\param int role - роль
	\return bool - значение для data присвоено*/
	virtual bool setData(int column, const QVariant& value, int role);
	
	/*! Возвращает заголовки
	\param int section - число столбцов
	\param int role - роль, для которой прописаны заголовки
	\return QVariant*/
	virtual QVariant headerData(int section, int role) const Q_DECL_OVERRIDE;
	
	/*! Валидность элемента 
	\return bool isValid*/
	bool isValid() const Q_DECL_OVERRIDE;

	/*! Новый ли элемент
	\return bool isNew*/
	bool isNew() const Q_DECL_OVERRIDE;

	/*! Определяет, есть ли в модели столбец с чек-боксом (для роли Qt::CheckStateRole.)
	\return true - столбец есть*/
	virtual bool isCheckable() const Q_DECL_OVERRIDE;

	/*!	Сохраняет изменения */
	bool save() Q_DECL_OVERRIDE;

	/*! Отменяет изменения*/
	bool cancel() Q_DECL_OVERRIDE;

	/*! Загружает данные из БД
	\return QList<BaseItem*> - список загруженных элементов*/
	virtual QList<BaseItem*> loadItemsFromDb(QVariant id = QVariant()) Q_DECL_OVERRIDE;

protected:
	int m_id=0;				// id региона
	QString m_name;			// название региона
	QString m_comment="";	// комментарий 
	//int m_type = 3;
};