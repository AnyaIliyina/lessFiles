#include "Multilist.h"
#include <QSizePolicy>
#include <QStylePainter>

MultiListWidget::MultiListWidget(const QModelIndex &index, QWidget*parent) : QComboBox(parent)
	
{
	
	
	connect(model(), SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(slotModelRowsInserted(QModelIndex, int, int)));
	
	QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

	connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
}

MultiListWidget::~MultiListWidget()
{
}

QStringList MultiListWidget::checkedItems() const
{
	return mCheckedItems;
}

QList<int> MultiListWidget::checkedIDs() const
{
	return mCheckedIDs;
}

void MultiListWidget::setCheckedItems(const QStringList &items)
{
	QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

	disconnect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

	for (int i = 0; i < items.count(); ++i)
	{
		int index = findText(items.at(i));

		if (index != -1)
		{
			standartModel->item(index)->setData(Qt::Checked, Qt::CheckStateRole);
		}
	}

	connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

	collectCheckedItems();
}

void MultiListWidget::paintEvent(QPaintEvent *event)
{
	(void)event;

	QStylePainter painter(this);

	painter.setPen(palette().color(QPalette::Text));

	QStyleOptionComboBox option;

	initStyleOption(&option);

	painter.drawComplexControl(QStyle::CC_ComboBox, option);

	QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
		mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

	painter.drawText(textRect, Qt::AlignVCenter, mDisplayText);
}

void MultiListWidget::resizeEvent(QResizeEvent *event)
{
	(void)event;

	updateDisplayText();
}

void MultiListWidget::collectCheckedItems()
{
	QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

	mCheckedItems.clear();
	mCheckedIDs.clear();

	for (int i = 0; i < count(); ++i)
	{
		QStandardItem *currentItem = standartModel->item(i);

		Qt::CheckState checkState = static_cast<Qt::CheckState>(currentItem->data(Qt::CheckStateRole).toInt());

		if (checkState == Qt::Checked)
		{
			mCheckedItems.push_back((currentItem->text()).toUtf8());
			mCheckedIDs.push_back(currentItem->row()+1);
		}
	}

	updateDisplayText();
	emit signalCheckedIDs(mCheckedIDs);
	repaint();
}

void MultiListWidget::updateDisplayText()
{
	QRect textRect = rect().adjusted(mDisplayRectDelta.left(), mDisplayRectDelta.top(),
		mDisplayRectDelta.right(), mDisplayRectDelta.bottom());

	QFontMetrics fontMetrics(font());

	mDisplayText = mCheckedItems.join(", ");
	mDisplayText = mDisplayText.toUtf8();
	if (fontMetrics.size(Qt::TextSingleLine, mDisplayText).width() > textRect.width())
	{
		while (mDisplayText != "" && fontMetrics.size(Qt::TextSingleLine, mDisplayText + "...").width() > textRect.width())
		{
			mDisplayText.remove(mDisplayText.length() - 1, 1);
		}

		mDisplayText += "...";
	}

}

void MultiListWidget::slotModelRowsInserted(const QModelIndex &parent, int start, int end)
{
	(void)parent;

	QStandardItemModel *standartModel = qobject_cast<QStandardItemModel*>(model());

	disconnect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));

	for (int i = start; i <= end; ++i)
	{
		standartModel->item(i)->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		standartModel->item(i)->setData(Qt::Unchecked, Qt::CheckStateRole);
	}

	connect(standartModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slotModelItemChanged(QStandardItem*)));
}

void MultiListWidget::slotModelItemChanged(QStandardItem *item)
{
	(void)item;

	collectCheckedItems();
}