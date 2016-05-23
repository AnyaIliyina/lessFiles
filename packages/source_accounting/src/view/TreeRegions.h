#pragma once
#include "ui_TreeRegions.h"
#include "Item_model.h"
#include <QApplication>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

/*!
	 \file
	 \brief  Вывод дерева регионов (с кнопками для редактирования, без чек-боксов)
	 \author Козырева О., Ильина А.
	 \date   апрель 2016
*/

class TreeRegions : public QMainWindow {
	Q_OBJECT
public:
	/*!
	Конструктор
	*/
	explicit TreeRegions(QWidget *parent = 0);

	/*!
	Деструктор
	*/
	~TreeRegions();
		
	/*!
	Устанавливает модель в таблицу
	*/
	 void setupModel();

	 /*! Возвращает модель дерева регионов
	 \return ItemModel* model - модель*/
	 ItemModel* model() const;

private:
	Ui::TreeRegions *ui;		// экземпляр класса, описывающего форму	
	ItemModel* m_model=nullptr;	// модель дерева
	bool m_editMode = false;	// режим редактирования
	

	/*!
	Отключает кнопки (исключая кнопку "Добавить")
	*/
	void setDisabled();

		
private slots:
	/*!
	Слот добавления
	*/
	void slotAdd();
	
	/*!
	Слот добавления
	*/
	void slotAddRoot();


	/*!
	Слот удаления
	*/
	void slotDelete();
	
	/*!
	 Слот редактирования
	 */
	 void slotEdit();
	
	 /*!
	 Слот применения изменений
	 */
	 void slotSave();

	 /*!
	 Слот отмены изменений
	 */
	 void slotCancel();
	
	 /*!
	 Слоты включения/выключения кнопок 
	 */
     void slotEnableButtons(const QItemSelection &, const QItemSelection &);
	 void slotEnableButtons();

	 
 signals:
	 /*!
	 Сигнал для включения/выключения кнопок
	 */
	 void signalChangeEditMode();

	 /*!
	 Сигнал о том, что данные в модели изменились
	 */
	 void dataChanged();

	 
};