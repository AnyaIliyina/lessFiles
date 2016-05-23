#pragma once 
#include <QMainWindow>
#include <QWidget>
#include <QString>
#include "ui_MainWindow.h"
#include "ViewSites.h"
#include "ViewDepartments.h"
#include "RegionItemChecked.h"
#include "TreeRegions.h"
#include "Item_model.h"


/*!
	\file
	\brief Описывает конфигурацию основного окна приложения
	\author Ильина А., Козырева О.
	\date март 2016г.
*/
class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	/*! 
	Конструктор 
	\param QMainWindow *parent - родительский виджет
	*/
	explicit MainWindow(QMainWindow *parent = 0);
	
	/*! Деструктор*/
	~MainWindow();

private:
	/*!
	Выводит основное окно и начинает работу модуля поиска
	*/
	void showMW();
	
	Ui::MainWindow *ui;
	ViewDepartments *m_vd;
	ViewSites *m_vs;
	TreeRegions *m_tr;
	QTreeView *treeSites;
	QTreeView *treeDepartments;
	QTreeView *treeSearch;
	QTableView *tableSites;
	QTableView *tableDepartments;
	QHBoxLayout *vslayout;
	QHBoxLayout *vdlayout;
	QWidget *search;
	QWidget *sites;
	QWidget *departaments;
	QWidget *content;
	ItemModel *m_res_model;
	
	ItemModel *m_dep_model;
	ItemModel *m_regionsChecked=nullptr;
	QMap<int, RegionItemChecked*> map;	// id регионов и указатели на регионы

	/*! Настраивает вкладку Поиск источников (QWidget *search)*/
	void setSearchResources();

	/*! Настраивает вкладку Интернет-ресурсы (QWidget *sites)*/
	void setResourcesView();

	/*! Настраивает вкладку Ведомства (QWidget *departaments)*/
	void setDepartamentView();
	
	/*! Создает модель для таблицы tableSites при поиске источников
	\param id - id сайта*/
	void setupModelSite(int id);

	/*! Создает модель для таблицы tableDepartments при поиске источников
	\param id - id ведомства*/
	void setupModelDepartment(int id);

private slots:
	
	/*!
	Закрывает основное окно
	*/
	void slotCloseMW();

	/*! Создает модель для деревьев treeSites и treeDepartments
	(модель из элементов RegionItemChecked) */
	void slotSetupRegionsModel();
	
	/*!
	"Собирает" основное окно из виджетов
	\param int type - тип пользователя
	*/
	void slotConfigure();

	/*! Начинает поиск источников
	\param const QItemSelection & - новое выделение в дереве treeSearch 
	\param const QItemSelection & - старое выделение*/
	void slotForSearch(const QItemSelection &, const QItemSelection &);
	
	/*!
	Начинает сессию для пользователя user_id
	\param  user_id - идентификатор пользователя */
	void slotStartSession(int user_id);

	/*! Ставит галочки в чек-боксах *treeSites, 
	когда в *tableSites выделен один сайт
	\param int id - id выделенного сайта */
	void slotSelectRegionSite(int);

	/*! Ставит галочки в чек-боксах *treeDepartments,
	когда в *tableDepartments выделено одно ведомство
	\param int id - id выделенного ведомства */
	void slotSelectRegionDepartment(int);

	/*! Открывает url сайта в браузере
	\param const QModelIndex &index*/
	void slotOpenUrl(const QModelIndex &index);
	
	/*! Соединяет treeSites со слотом slotMakeCheckEditbleSite(const QItemSelection &, const QItemSelection &)
	 - изменения выделения в дереве теперь будут отслеживаться*/
	void slotGetCheckSite();

	/*! Начинает редактирование выделенного элемента  treeSites
	\param const QItemSelection & - новое выделение в дереве treeSites 
	\param const QItemSelection & - старое выделение*/
	void slotMakeCheckEditbleSite(const QItemSelection &, const QItemSelection &);

	/*! Заканчивает редактирование элементов  treeSites, сохраняет изменения
	(создает или удаляет записи в таблице site_regions)
	\param int id - id сайта
	\param bool saveChanges: true - сохранить изменения, false - отмена*/
	void slotEditCheckSite(int id, bool saveChanges);

	/*! Соединяет treeDepartments со слотом slotMakeCheckEditbleDepartment(const QItemSelection &, const QItemSelection &)
	- изменения выделения в дереве теперь будут отслеживаться*/
	void slotGetCheckDepartment();

	/*! Начинает редактирование выделенного элемента treeDepartments
	\param const QItemSelection & - новое выделение в дереве treeSites
	\param const QItemSelection & - старое выделение*/
	void slotMakeCheckEditbleDepartment(const QItemSelection &, const QItemSelection &);
	

	/*! Заканчивает редактирование элементов  treeDepartments, сохраняет изменения
	(создает или удаляет записи в таблице department_regions)
	\param int id - id ведомства
	\param bool saveChanges: true - сохранить изменения, false - отмена*/
	void slotEditCheckDepartment(int id, bool saveChanges);

	/*! При смене вкладок обеспечивает соответствие дерева регионов выбранной строке в tableView
	(для вкладок Интернет-ресурсы  и Ведомства)
	\param int tabIndex - номер выбранной вкладки */
	void slotSyncTabs(int tabIndex);

	/*! Включает treeSites и удаляет галочки в чек-боксах*/
	void slotUncheckTreeSites();

	/*! Включает treeDepartments и удаляет галочки в чек-боксах*/
	void slotUncheckTreeDepartments();
};


