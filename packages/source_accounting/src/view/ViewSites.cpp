#include "ViewSites.h"
#include "Database.h"
#include "Scale.h"
#include "State.h"
#include "GeodataType.h"
#include "Types.h"
#include "Item_model.h"
#include "Combo_delegate.h"
#include "Site.h"
#include "Language.h"
#include "Session.h"
#include <QApplication>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>


ViewSites::ViewSites(QWidget * parent): ui(new Ui::ViewSites) // ??
{
	ui->setupUi(this);
	setupModel();
	
	QObject::connect(ui->action_New, SIGNAL(triggered()), this, SLOT(slotAdd()));
	QObject::connect(ui->action_Delete, SIGNAL(triggered()), this, SLOT(slotDelete()));
	QObject::connect(ui->action_Edit, SIGNAL(triggered()), this, SLOT(slotEdit()));
	QObject::connect(ui->action_Yes, SIGNAL(triggered()), this, SLOT(slotSave()));
	QObject::connect(ui->action_No, SIGNAL(triggered()), this, SLOT(slotCancel()));
	QObject::connect(ui->action_OpenUrl, SIGNAL(triggered()), this, SLOT(slotOpenUrl()));
}

ViewSites::~ViewSites()
{
	delete ui;
	delete m_model;
}

void ViewSites::setupModel()
{
	delete m_model;
	QSqlDatabase db = Database::database();
	m_model = new ItemModel();
	m_model->loadData(ItemTypes::ResourceType);
	ui->tableView->setModel(m_model);
	connect(m_model, SIGNAL(indexStatusChanged(const QModelIndex&)), this, SLOT(slotIndexStatusChanged(const QModelIndex&)));
	connect(ui->tableView->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this,
		SLOT(slotSelectionChanged(const QItemSelection&, const QItemSelection&)));
	
	createTable();
}

void ViewSites::backToTab()
{
	slotSelectionChanged(QItemSelection(), QItemSelection());
}


void ViewSites::createTable()
{
	comboDelegateLanguage = new ComboDelegate(Language::getList(), this);
	ui->tableView->setItemDelegateForColumn(3, comboDelegateLanguage);


	comboDelegateGPI = new ComboDelegate(GeodataType::getList(), this);
	ui->tableView->setItemDelegateForColumn(4, comboDelegateGPI);


	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setColumnHidden(0, true);
	ui->tableView->resizeColumnsToContents();
	ui->tableView->resizeRowsToContents();

	ui->tableView->setColumnWidth(1, 120);
	ui->tableView->setColumnWidth(2, 300);
	ui->tableView->setColumnWidth(3, 200);
	ui->tableView->setColumnWidth(4, 300);
	ui->tableView->setColumnWidth(5, 300);

	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewSites::slotAdd()
{
	emit signalNewSite();
	QModelIndex index;
	m_model->insertRows(0, 1, index);
	ui->tableView->resizeRowsToContents();
	auto rowCount = m_model->rowCount(index);
	auto child = m_model->index(rowCount - 1, 0, index); 

	m_editMode = true;

	ui->tableView->selectionModel()->setCurrentIndex(child, QItemSelectionModel::SelectCurrent);
	ui->tableView->edit(child);
}

void ViewSites::slotDelete()
{
	
	int  deleteMsgBox = QMessageBox::question(this, "",
		"Удалить выбранную запись?",
		QMessageBox::Yes, QMessageBox::No);
	if (deleteMsgBox == QMessageBox::Yes)
	{
		auto indexes = ui->tableView->selectionModel()->selectedRows();
		for (int i = indexes.count()-1; i>=0; i--)
		{
			auto index = indexes[i];
			m_model->removeRows(0, 1, index);
		}
	}
	ui->tableView->resizeRowsToContents();
}

void ViewSites::slotEdit()
{
	m_editMode = true;
	emit signalEditSite();
	auto index = ui->tableView->selectionModel()->currentIndex();
	ui->tableView->resizeRowsToContents();
	m_model->startEditMode(index);
	ui->tableView->edit(index);

}

void ViewSites::slotSave()
{
	auto m_index = ui->tableView->selectionModel()->currentIndex();
	auto row = m_index.row()+1;
	auto column = m_index.column();
	auto child = m_model->index(row, column, m_index);
	ui->tableView->selectionModel()->setCurrentIndex(child, QItemSelectionModel::Select |
		QItemSelectionModel::Rows);
	if (m_model->save())
	{
		
		m_editMode = false;
		int value = m_model->data(ui->tableView->selectionModel()->selectedRows()[0], Qt::UserRole).toInt();
		emit signalSave(value, true);
		QMessageBox::information(this, "", "Сохранено", QMessageBox::Ok);
		
	}
	else
	{
		QMessageBox::critical(this, "", "Не удалось применить изменения", QMessageBox::Ok);
	}
	auto index = ui->tableView->selectionModel()->currentIndex();
	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewSites::slotCancel()
{
	if (m_model->cancel())
	{
		m_editMode = false;
		emit signalSave(-1, false);
		
	}
	else
		QMessageBox::critical(this, "", "Не удалось отменить изменения", QMessageBox::Ok);
	auto index = ui->tableView->selectionModel()->currentIndex();
	ui->tableView->reset();
	ui->tableView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select |
		QItemSelectionModel::Rows);
	slotSelectionChanged(QItemSelection(), QItemSelection());
	
}

void ViewSites::slotOpenUrl()
{
	auto index = ui->tableView->selectionModel()->currentIndex();
	int row = index.row();
	auto child = m_model->index(row, 2);
	QString url= m_model->data(child).toString();
	QUrl m_url(url);
	bool res=QDesktopServices::openUrl(m_url);
}

void ViewSites::slotIndexStatusChanged(const QModelIndex &)
{
	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewSites::slotSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
	if (ui->tableView->model() == NULL)
		return;

	if (m_editMode)
	{
		ui->action_Edit->setEnabled(false);
		ui->action_Delete->setEnabled(false);
		ui->action_New->setEnabled(false);
		ui->action_OpenUrl->setEnabled(false);
		ui->action_Yes->setEnabled(m_model->allowSave());
		ui->action_No->setEnabled(true);

	}
	else
	{
		ui->action_New->setEnabled(true);
		ui->action_Yes->setEnabled(false);
		ui->action_No->setEnabled(false);

		if (ui->tableView->selectionModel()->selectedRows().count() > 1)
		{
			ui->action_Delete->setEnabled(true);
			ui->action_Edit->setEnabled(false);
			ui->action_OpenUrl->setEnabled(false);
			emit valueSelected(-1);

		}
		if (ui->tableView->selectionModel()->selectedRows().count() == 1)
		{
			ui->action_Delete->setEnabled(true);
			ui->action_Edit->setEnabled(true);
			ui->action_OpenUrl->setEnabled(true);
			int value = m_model->data(ui->tableView->selectionModel()->selectedRows()[0], Qt::UserRole).toInt();
			emit valueSelected(value);
		}
		if (ui->tableView->selectionModel()->selectedRows().count() == 0)
		{
			ui->action_Delete->setEnabled(false);
			ui->action_Edit->setEnabled(false);
			ui->action_OpenUrl->setEnabled(false);
			emit valueSelected(-1);
		}
	}
}

