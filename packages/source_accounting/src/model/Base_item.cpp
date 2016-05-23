#include "base_item.h"
#include <QDebug>
BaseItem::~BaseItem() {
	qDeleteAll(m_children);
};

BaseItem::BaseItem() {

};

//------------ Методы по умолчанию --------------

int BaseItem::rowCount() const {
	return m_children.count();
};

BaseItem* BaseItem::parent() const {
	return m_parent;
};

BaseItem* BaseItem::child(int row) const {
	return m_children.value(row);
};

void BaseItem::appendChild(BaseItem* child) {
	m_children << child;
	child->m_parent = this;
};

int BaseItem::row() const {
	if (m_parent == NULL)
		return 0;
	else
	{
		return m_parent->m_children.indexOf(const_cast<BaseItem*>(this));
	}
};

bool BaseItem::hasChildren() const {
	return rowCount() != 0;
}

bool BaseItem::isCheckable() const
{
	return false;
};