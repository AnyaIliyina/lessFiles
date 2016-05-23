#pragma once

#include <QtGui>
#include <QComboBox>
#include <QWidget>
/*!
*  \brief ��������� combobox, ����������� ��� ��������� ���������� ��������� ������
*  \author �������� �.
*  \date  ��� 2016�.
*/

class MultiListWidget: public QComboBox
{
	Q_OBJECT

		Q_PROPERTY(QStringList checkedItems READ checkedItems WRITE setCheckedItems)

public:
	MultiListWidget(const QModelIndex &index, QWidget *parent=NULL);
	virtual ~MultiListWidget();

	QStringList checkedItems() const;
	QList<int> checkedIDs() const;
	void setCheckedItems(const QStringList &items);

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

private:
	QStringList mCheckedItems;
	QList<int> mCheckedIDs;

	void collectCheckedItems();

	QString mDisplayText;
	const QRect mDisplayRectDelta;

	void updateDisplayText();

	private slots:
	void slotModelRowsInserted(const QModelIndex &parent, int start, int end);
	//void slotModelRowsRemoved(const QModelIndex &parent, int start, int end);
	void slotModelItemChanged(QStandardItem *item);
	
signals:
	void signalCheckedIDs(QList<int>);
};

