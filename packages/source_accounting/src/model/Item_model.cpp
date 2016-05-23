#include "Item_model.h"

#include <QBrush>
#include <QDebug>

#include "Base_item.h"
#include "Item_factory.h"

ItemModel::~ItemModel() {
	if (m_rootItem != NULL) {
		delete m_rootItem;
		m_rootItem = NULL;
	}
	m_editedItem = NULL;
};

ItemModel::ItemModel() : QAbstractItemModel() {
};

int ItemModel::rowCount(const QModelIndex& parent) const {
	BaseItem* parentItem;

	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<BaseItem*>(parent.internalPointer());

	return parentItem->rowCount();
};

int ItemModel::columnCount(const QModelIndex& parent) const {
	BaseItem* parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<BaseItem*>(parent.internalPointer());
	
	return parentItem->columnCount();
};

bool ItemModel::hasChildren(const QModelIndex& parent) const {
	BaseItem* parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<BaseItem*>(parent.internalPointer());

	return parentItem->hasChildren();
	return false;
};

QModelIndex ItemModel::index(int row, int column, const QModelIndex& parent) const {
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	BaseItem* parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<BaseItem*>(parent.internalPointer());

	BaseItem* childItem = parentItem->child(row);
	if (childItem == NULL)
	{
		qDebug() << "childItem == NULL";
		return QModelIndex();
	}
	else
		return createIndex(row, column, childItem);
};

QModelIndex ItemModel::parent(const QModelIndex& child) const {
	if (!child.isValid())
		return QModelIndex();

	BaseItem* childItem = static_cast<BaseItem*>(child.internalPointer());
	BaseItem* parentItem = childItem->parent();

	if (parentItem == m_rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
};

QVariant ItemModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid())
		return QVariant();

	BaseItem* item = static_cast<BaseItem*>(index.internalPointer());

	if (role == Qt::BackgroundRole && item != m_editedItem)
	{
		
		return QVariant();
	}
	
	return item->data(index.column(), role);
};

bool ItemModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (!index.isValid())
		return false;

	BaseItem* item = static_cast<BaseItem*>(index.internalPointer());

	bool res = item->setData(index.column(), value, role);

	emit indexStatusChanged(index);
	
	return res;
};

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal)
		return m_rootItem->headerData(section, role);
	else
		return QVariant();
};

Qt::ItemFlags ItemModel::flags(const QModelIndex& index) const {
	if (!index.isValid())
		return Qt::NoItemFlags;

	BaseItem* item = static_cast<BaseItem*>(index.internalPointer());

	if(item == m_editedItem)
	{
		if (m_editedItem->isCheckable() && index.column() == 0)				//
			return  Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;			//
		
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
	}
	else
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
};

bool ItemModel::insertRows(int row, int count, const QModelIndex& parent) {
	if (m_editedItem != NULL)
		return false;

	BaseItem* parentItem;

	if (!parent.isValid())
		parentItem = m_rootItem;
	else
		parentItem = static_cast<BaseItem*>(parent.internalPointer());

	// Фабрика
	BaseItem* child = ItemFactory::createNew(m_type);
	parentItem->appendChild(child);
	m_editedItem = child;
	
	this->beginInsertRows(parent, parentItem->rowCount(), parentItem->rowCount());
	this->endInsertRows();

	return true;
};

bool ItemModel::removeRows(int row, int count, const QModelIndex& index) {
	if (!index.isValid())
		return true;

	BaseItem* item = static_cast<BaseItem*>(index.internalPointer());
	BaseItem* parentItem = item->parent();
	qDebug() << "Parent index";
	auto parentIndex = index.parent();
	

	this->beginRemoveRows(parentIndex, item->row(), item->row());
	//this->beginResetModel();

	parentItem->removeChild(item);

	this->endRemoveRows();
	//this->endResetModel();

	return true;
};

/*bool ItemModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count,
	const QModelIndex& destinationParent, int destinationChild) {
	
	BaseUpDownItem* upDownItem = dynamic_cast<BaseUpDownItem*>(m_rootItem->child(sourceRow));
	if (upDownItem == NULL)
		return false;

	//this->beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, sourceRow + 1);
	this->beginResetModel();

	bool res;
	if (sourceRow > destinationChild)
		res = upDownItem->up();
	else
		res = upDownItem->down();

	//this->endMoveRows();
	this->endResetModel();

	return res;
};*/

void ItemModel::startEditMode(const QModelIndex& index) {
	if (!index.isValid())
	{
		return;
		qDebug() << "index invalid";
	}
	m_editedItem = static_cast<BaseItem*>(index.internalPointer());
	qDebug()<<"startEditMode m_editItem"<<m_editedItem;
};

bool ItemModel::allowSave() {
	if (m_editedItem == NULL)
		return true;
	return m_editedItem->isValid();
};

bool ItemModel::save() {
	if (m_editedItem == NULL)
		return false;

	if (!m_editedItem->save())
		return false;

	m_editedItem = NULL;
	return true;
};

bool ItemModel::cancel() {
	if (m_editedItem == NULL)
		return false;

	bool allow_clear = true;

	if (m_editedItem->isNew()) {
		auto index = createIndex(m_editedItem->row(), 0, m_editedItem);

		this->beginRemoveRows(index.parent(), m_editedItem->row(), m_editedItem->row());
		//this->beginResetModel();

		BaseItem* parentItem = m_editedItem->parent();
		parentItem->removeChild(m_editedItem);

		//this->endResetModel();
		this->endRemoveRows();
	}
	else {
		if (!m_editedItem->cancel())
			allow_clear = false;
	}

	if (allow_clear)
		m_editedItem = NULL;

	return true;
};

void ItemModel::loadData(int type, QVariant id) {
	// Фабрика
	this->beginResetModel();

	if (m_rootItem != NULL) {
		delete m_rootItem;
		m_rootItem = NULL;
	}
	m_editedItem = NULL;

	m_rootItem = ItemFactory::loadFromDb(type, id);
	m_type = type;

	this->endResetModel();
};

//QModelIndex ItemModel::mapFromSource(const QModelIndex & index) const
//{
//	if (index.isValid())
//		return createIndex(index.row(), index.column());
//	else
//		return QModelIndex();
//}
//
//QModelIndex ItemModel::mapToSource(const QModelIndex & index) const
//{
//	if (index.isValid())
//		return createIndex(index.row(), index.column());
//	else
//		return QModelIndex();
//}
