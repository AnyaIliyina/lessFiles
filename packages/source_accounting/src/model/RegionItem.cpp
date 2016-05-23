#include "RegionItem.h"



#include <QBrush>
#include <QDebug>
#include <QPixmap>
#include <QSqlError>
#include <QSqlDatabase> 
#include <QSqlQuery>


RegionItem::~RegionItem() {};
RegionItem::RegionItem() : BaseItem() { };

int RegionItem::columnCount() const {
	return 3;
};


void RegionItem::removeChild(BaseItem* child)
{
	if (!m_children.contains(child)) 
		return;
	RegionItem *r_item = dynamic_cast<RegionItem*>(child);
	if (r_item == NULL)   
		return; 
	if (!child->isNew()) {
		Region::deleteRegion(r_item->m_id);
		/*auto db = QSqlDatabase::database("PsqlConnection", true);  
		QSqlQuery query(db); 
		query.prepare(" DELETE FROM itgi.itgi_groups "    " WHERE id = :id ");  
		query.bindValue(":id", itgiGroup->m_id);  
		if (!query.exec())    
			throw ItgiException(QString("Не удалось удалить группу ИТГИ.\n%1").arg(db.lastError().text())); */
	} 
	m_children.removeOne(child);
};


QVariant RegionItem::data(int column, int role) const
{ 
	if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
	{
		if (column == 0)
			return m_name;
		if (column == 1)
			return m_id;
		if (column == 2)
			return m_comment;
	}

	if (role == Qt::UserRole) 
			return m_id;
	
	return QVariant();	
}; 


bool RegionItem::setData(int column, const QVariant& value, int role)
{
	if (value.isNull() || value.toString().isEmpty()) 
		return false;  
	if (role == Qt::EditRole)
	{
	if (column == 0) 
		m_name = value.toString();  
	if (column == 1)   
		m_id = value.toInt();
	if (column == 2)
		m_comment = value.toString();
	} 
	return true; 
}

bool RegionItem::isCheckable() const
{
	return false;
}


QVariant RegionItem::headerData(int section, int role) const {
	if (role == Qt::DisplayRole) {			
		if (section == 0)
			return "Регионы";
		/*if (section == 1)
			return "ID";
		if (section == 2)
			return "Комментарий";*/
	}
	return QVariant();
}; 

bool RegionItem::isValid() const 
{
	if (m_name.isNull() || m_name.isEmpty()) 
		return false;  
	/*if (m_comment.isNull() || m_comment.isEmpty())
		return false;*/
	return true;
};

bool RegionItem::isNew() const 
{
	return (m_id == 0);
}; 

bool RegionItem::save() {
	if (isValid()) {
		auto db = Database::database();
		QSqlQuery query(db);
		if (m_id == 0) {
			// Вставка нового региона 
			Region *r = new Region();
			query.prepare("INSERT INTO regions( "
				" comment, name, parent_id "
				" ) VALUES( "
				" :comment, :name, :parent_id "
				" ) "
				);
			RegionItem * parentGroup = static_cast<RegionItem*>(m_parent);
			query.bindValue(":comment", m_comment);
			query.bindValue(":name", m_name);
			query.bindValue(":parent_id", parentGroup->m_id == 0 ? 0 : parentGroup->m_id);
			if (!query.exec())
				qDebug() << "Не удалось добавить новую группу ИТГИ." << query.lastError().text();
			if (query.next()) {
				qDebug() << "получилось";
				m_id = query.lastInsertId().toInt();
			}
		}
		else {
			// Изменение 
			int parent_id = m_parent->data(1, Qt::EditRole).toInt();
			Region *r = new Region(parent_id, m_name, m_comment);
			r->setId(m_id);
			r->update();
			delete r;
		}
		return true;
	}
	return false;
}; 

bool RegionItem::cancel() {
	if (isNew())
		return true;
	Region *r = new Region(m_id);
	m_name = r->name();
	m_comment = r->comment();
	delete r;
	return true;
}; 

QList<BaseItem*> RegionItem::loadItemsFromDb(QVariant id )
{
	qDebug() << "loadItemsFromDb RegionItems";
	QList<BaseItem*> list; 
	QSqlDatabase db = Database::database();
	QSqlQuery query(db); 
	if (!query.exec(
		"WITH RECURSIVE tree(id, name, comment, parent_id, depth) AS \
		(\
			SELECT id, name, comment, 0 AS parent_id, 1 AS depth \
			 FROM regions \
				WHERE parent_id IS 0 \
			UNION ALL\
			SELECT r.id, r.name, r.comment, r.parent_id, tr.depth+1 AS depth\
			 FROM regions r, tree tr\
				WHERE r.parent_id = tr.id\
		)\
		SELECT * from tree ORDER BY tree.parent_id ASC, tree.name ASC  limit 1000"))
		qDebug() << "RegionItem::loadItemsFromDb() error:" << query.lastError().text();
				
	QMap<int, RegionItem*> map;
	while (query.next()) 
	{   
		RegionItem* r_item = new RegionItem();
		r_item->m_id = query.value(0).toInt();			// id   
		r_item->m_name = query.value(1).toString(); 	// name 
		r_item->m_comment = query.value(2).toString(); 	// comment   
		int parent_id = query.value(3).toInt(); 		// parent_id  
		if (parent_id == 0)
			list << r_item; 
		else {  
			auto parent = map[parent_id];  
			parent->appendChild(r_item);  
		}   map.insert(r_item->m_id, r_item); 
	} 
	return list;
};