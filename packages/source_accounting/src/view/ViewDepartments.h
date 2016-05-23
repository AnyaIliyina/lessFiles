#pragma once
#include "ui_ViewDepartments.h"
#include "Item_model.h"
#include <QApplication>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

/*!
	 \file
	 \brief  Таблица ведомств и работа с ней
	 \author Козырева О.
	 \date   апрель 2016
*/

class ViewDepartments: public QMainWindow {
	Q_OBJECT
public:
	/*!
	Конструктор
	*/
	explicit ViewDepartments(QWidget *parent = 0);

	/*!
	Деструктор
	*/
	~ViewDepartments();
		
	/*!
	Устанавливает модель в таблицу
	*/
	 void setupModel();

	 /*!
	 Настраивает виджет в соответствии с текущим выделением в таблице
	 */
	 void backToTab();

private:
	Ui::ViewDepartments *ui;
	ItemModel* m_model=nullptr;
	bool m_editMode = false;
	

	 /*!
	 Метод для настройки таблицы 
	 */
	 void createTable();


//public slots:	
//	/*!
//	Слот включения/выключения кнопок
//	*/
//	void slotEnableButtons();
//
private slots:
	/*!
	Слот добавления
	*/
	void slotAdd();
	/*!
	Слот удаления
	*/
	void slotDelete();
	/*
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
	 void slotIndexStatusChanged(const QModelIndex&);
	 void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	
 signals:
	 /*!
	 Сигнал, испускается при создании нового ведомства, для очистки дерева регионов
	 */
	 void signalNewDepartment();

	 /*!
	 Сигнал, для выделения элементов дерева регионов, по выделенной записи в таблице ведомств
	 \param int !=-1, если выделена одна запись в таблице, отправляем id выделеной записи
	 */
	 void valueSelected(int);

	 /*!
	 Сигнал для включения/выключения кнопок
	 */
	 void signalEditDepartment();
	 
	 /*!
	 Сигнал, испускается при сохранении записи, для сохранения изменений в дереве регионов
	 \param int - id интернет-ресурса
	 \param bool 
	 */
	  void signalSave(int, bool);
};