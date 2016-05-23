/*!

	\file
	\brief Модель управления базовыми элементами
	\author Чернышев Д.Р.
	\date Апрель 2016

*/

#pragma once

#include <QAbstractItemModel>

class BaseItem;

class ItemModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	~ItemModel();
	ItemModel();

	int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	bool hasChildren(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

	QModelIndex parent(const QModelIndex& child) const Q_DECL_OVERRIDE;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const  Q_DECL_OVERRIDE;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

	Qt::ItemFlags flags(const QModelIndex& index) const Q_DECL_OVERRIDE;

	bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) Q_DECL_OVERRIDE;
	bool removeRows(int row, int count, const QModelIndex& index) Q_DECL_OVERRIDE;
	//bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
	//	const QModelIndex& destinationParent, int destinationChild) Q_DECL_OVERRIDE;
	//QModelIndex mapFromSource(const QModelIndex& index) const;
	//QModelIndex mapToSource(const QModelIndex& index) const;
	void startEditMode(const QModelIndex& index);
	bool allowSave();
	bool save();
	bool cancel();

	void loadData(int type, QVariant id = QVariant());

signals:
	void indexStatusChanged(const QModelIndex& index);

private:
	
	BaseItem* m_rootItem = NULL;
	BaseItem* m_editedItem = NULL;
	int m_type = 0;
};