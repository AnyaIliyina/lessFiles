#pragma once
#include "ui_ViewSites.h"
#include "Item_model.h"
#include "Combo_delegate.h"
#include <QApplication>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

/*!
	 \file
	 \brief  Таблица интернет-ресурсов и работа с ней
	 \author Козырева О.
	 \date   апрель 2016
*/

class ViewSites : public QMainWindow {
	Q_OBJECT
public:
	/*!
	Конструктор
	*/
	explicit ViewSites(QWidget *parent = 0);

	/*!
	Деструктор
	*/
	~ViewSites();
		
	/*!
	Устанавливает модель в таблицу
	*/
	 void setupModel();

	 /*! 
	 Настраивает виджет в соответствии с текущим выделением в таблице
	 */
	 void backToTab();
	
private:
	Ui::ViewSites *ui;
	ItemModel* m_model=nullptr;
	bool m_editMode = false;
	ComboDelegate *comboDelegateLanguage;
	ComboDelegate *comboDelegateGPI;

	/*!
	Метод для настройки таблицы 
	*/
	 void createTable();

//public slots:
	///*!
	//Слот включения/выключения кнопок
	//*/
	//void slotEnableButtons();
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
	 Слот открытия ссылки
	 */
	 void slotOpenUrl();

	 /*!
	 Слоты включения/выключения кнопок
	 */
	 void slotIndexStatusChanged(const QModelIndex&);
	 void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
 signals:
	 /*!
	 Сигнал, испускается при создании нового сайта, для очистки дерева регионов
	 */
	 void signalNewSite();

	 /*!
	 Сигнал, испускается при редактировании записи, для включения режима редактирования дерева регионов
	 */
	 void signalEditSite();

	 /*!
	 Сигнал, испускается при сохранении записи, для сохранения изменений в дереве регионов
	\param int - id интернет-ресурса
	\param bool 
	*/
	 void signalSave(int, bool);

	 /*!
	 Сигнал, для выделения элементов дерева регионов, по выделенной записи в таблице интернет-ресурсов
	 \param int !=-1, если выделена одна запись в таблице, отправляем id выделеной записи
	 */
	 void valueSelected(int);


	
	 
	
};