#include "Combo_delegate.h"
#include "Multilist.h"
#include "ViewSites.h"

#include <QComboBox>

ComboDelegate::~ComboDelegate() {
};

ComboDelegate::ComboDelegate(const QList<QString>& list, QObject* parent) {
		m_list = list;
	
};

QWidget* ComboDelegate::createEditor(
	QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	
	MultiListWidget *cBox = new MultiListWidget(index, parent);
	QObject::connect(cBox, SIGNAL(signalCheckedIDs(QList<int>)), this, SLOT(slotSetListIDs(QList<int>)));
	for (int i = 0; i < m_list.count(); i++) {
		cBox->addItem(m_list[i]);
	}
	
	return cBox;
}
void ComboDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	QString value = index.model()->data(index).toString();
	QStringList listCheckedItems = value.split(", ");
	MultiListWidget *listWidget = static_cast<MultiListWidget*>(editor);
	listWidget->setCheckedItems(listCheckedItems);

}
void ComboDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	MultiListWidget *listWidget = static_cast<MultiListWidget*>(editor);
	if (!listWidget->checkedItems().isEmpty())
	{
		model->setData(index, listWidget->checkedItems().join(", "));
	}
	
}
;

void ComboDelegate::slotSetListIDs(QList <int> listIDs)
{
	m_listIDs = listIDs;
}
