#include "ViewDepartments.h"
#include "Database.h"
#include "Scale.h"
#include "State.h"
#include "Geodata_record.h"
#include "GeodataType.h"
#include "Item_model.h"
#include "Combo_delegate.h"
#include "Site.h"
#include "Format.h"
#include "Session.h"
#include "Types.h"
#include <QApplication>
#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>


ViewDepartments::ViewDepartments(QWidget * parent): ui(new Ui::ViewDepartments) // ??
{
	ui->setupUi(this);
	setupModel();

	QObject::connect(ui->action_New, SIGNAL(triggered()), this, SLOT(slotAdd()));
	QObject::connect(ui->action_Delete, SIGNAL(triggered()), this, SLOT(slotDelete()));
	QObject::connect(ui->action_Edit, SIGNAL(triggered()), this, SLOT(slotEdit()));
	QObject::connect(ui->action_Yes, SIGNAL(triggered()), this, SLOT(slotSave()));
	QObject::connect(ui->action_No, SIGNAL(triggered()), this, SLOT(slotCancel()));
	
}

ViewDepartments::~ViewDepartments()
{
	delete ui;
	delete m_model;
}

void ViewDepartments::setupModel()
{
	delete m_model;
	QSqlDatabase db = Database::database();
	m_model = new ItemModel();
	m_model->loadData(ItemTypes::DepartmentType);
	ui->tableView->setModel(m_model);
	connect(m_model, SIGNAL(indexStatusChanged(const QModelIndex&)), this, SLOT(slotIndexStatusChanged(const QModelIndex&)));
	connect(ui->tableView->selectionModel(),
		SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this,
		SLOT(slotSelectionChanged(const QItemSelection&, const QItemSelection&)));
	createTable();
}

void ViewDepartments::backToTab()
{
	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewDepartments::createTable()
{
	auto comboDelegate = new ComboDelegate(GeodataType::getList(), this);
	ui->tableView->setItemDelegateForColumn(7, comboDelegate);

	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setColumnHidden(0, true);
	ui->tableView->resizeColumnsToContents();
	ui->tableView->resizeRowsToContents();

	ui->tableView->setColumnWidth(1, 300);
	ui->tableView->setColumnWidth(2, 150);
	ui->tableView->setColumnWidth(3, 350);
	ui->tableView->setColumnWidth(4, 100);
	ui->tableView->setColumnWidth(5, 100);
	ui->tableView->setColumnWidth(6, 200);
	ui->tableView->setColumnWidth(7, 350);
	ui->tableView->setColumnWidth(8, 350);

	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewDepartments::slotIndexStatusChanged(const QModelIndex &)
{
	slotSelectionChanged(QItemSelection(), QItemSelection());
}

void ViewDepartments::slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	if (ui->tableView->model() == NULL)
		return;

	if (m_editMode)
	{
		ui->action_Edit->setEnabled(false);
		ui->action_Delete->setEnabled(false);
		ui->action_New->setEnabled(false);
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
			emit valueSelected(-1);
		}
		if (ui->tableView->selectionModel()->selectedRows().count() == 1)
		{
			ui->action_Delete->setEnabled(true);
			ui->action_Edit->setEnabled(true);
			int value = m_model->data(ui->tableView->selectionModel()->selectedRows()[0], Qt::UserRole).toInt();
			emit valueSelected(value);
		}
		if (ui->tableView->selectionModel()->selectedRows().count() == 0)
		{
			ui->action_Delete->setEnabled(false);
			ui->action_Edit->setEnabled(false);
			emit valueSelected(-1);
		}
	}
	
}

void ViewDepartments::slotAdd()
{
	emit signalNewDepartment();
	QModelIndex index;
	m_model->insertRows(0, 1, index);
	ui->tableView->resizeRowsToContents();
	auto rowCount = m_model->rowCount(index);
	auto child = m_model->index(rowCount - 1, 0, index); 

	m_editMode = true;

	ui->tableView->selectionModel()->setCurrentIndex(child, QItemSelectionModel::SelectCurrent);
	ui->tableView->edit(child);
}

void ViewDepartments::slotDelete()
{
	
	int  deleteMsgBox = QMessageBox::question(this, "",
		"Удалить выбранную запись?",
		QMessageBox::Yes, QMessageBox::No);
	if (deleteMsgBox == QMessageBox::Yes)
	{
		auto indexes = ui->tableView->selectionModel()->selectedRows();
		for (int i = indexes.count() - 1; i >= 0; i--)
		{
			auto index = indexes[i];
			m_model->removeRows(0, 1, index);
		}
	}
	ui->tableView->resizeRowsToContents();
}

void ViewDepartments::slotEdit()
{	
	m_editMode = true;
	emit signalEditDepartment();
	auto index = ui->tableView->selectionModel()->currentIndex();
	ui->tableView->resizeRowsToContents();
	m_model->startEditMode(index);
	ui->tableView->edit(index);
}

void ViewDepartments::slotSave()
{
	auto m_index = ui->tableView->selectionModel()->currentIndex();
	auto row = m_index.row() + 1;
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

void ViewDepartments::slotCancel()
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

