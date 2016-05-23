#include "Departments.h"
#include "Database.h"
#include "Geodata_record.h"
#include "Site.h"
#include "Department.h"
#include "DepartmentType.h"
#include "GeodataType.h"
#include <QBrush>
#include <QDebug>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


Departments::~Departments() {};

Departments::Departments() {
};

int Departments::columnCount() const {
	return 9;
};

void Departments::removeChild(BaseItem* child) {
	if (!m_children.contains(child))
		return;

	Departments* department = dynamic_cast<Departments*>(child);
	if (department == NULL)
		return;
			Department::deleteDepartment(department->m_id);
	m_children.removeOne(child);
};

QVariant Departments::data(int column, int role) const{
	if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
		if (column == 0)
			return m_id;
		if (column == 1)
			return m_name;
		if (column == 2)
			return m_country;
		if (column == 3)
			return m_address;
		if (column == 4)
			return m_phone;
		if (column == 5)
			return m_fax;
		if (column == 6)
			return m_email;
		if (column == 7)
			return m_gpi;
		if (column == 8)
			return m_comment;
	
	}

	if (role == Qt::UserRole) {
		
		return m_id;
	}

	return QVariant();
};

bool Departments::setData(int column, const QVariant& value, int role) {
	if (value.isNull() || value.toString().isEmpty())
		return false;

	if (role == Qt::EditRole) {
		if (column == 0)
			m_id=value.toInt();
		if (column == 1)
			m_name=value.toString();
		if (column == 2)
			m_country = value.toString();
		if (column == 3)
			m_address = value.toString();
		if (column == 4)
			m_phone= value.toString();
		if (column == 5)
			m_fax = value.toString();
		if (column == 6)
			m_email = value.toString();
		if (column == 7)
			m_gpi= value.toString();
		if (column == 8)
			m_comment= value.toString();
		
		}
	
	return true;
};

QVariant Departments::headerData(int section, int role) const {
	
	if (role == Qt::DisplayRole) {
		if(section==0)
			return ("ID");
		if (section == 1) 
			return "Наименование ведомства";
		if (section == 2)
			return "Страна";
		if (section == 3)
			return "Адрес";
		if (section == 4)
			return "Телефон";
		if (section == 5)
			return "Факс";
		if (section == 6)
			return "Адрес электронной почты";
		if (section == 7)
			return "Виды ГПИ, предоставляемые организацией";
		if (section == 8)
			return "Комментарии";
		
	}

	return QVariant();
};

bool Departments::isValid() const {
	if (m_country.isNull() || m_country.isEmpty())
		return false;
	if (m_name.isNull() || m_name.isEmpty())
		return false;
	if (m_phone.isNull() || m_phone.isEmpty())
		return false;
	if (m_address.isNull() || m_address.isEmpty())
		return false;
	if (m_fax.isNull() || m_fax.isEmpty())
		return false;
	if (m_gpi.isNull() || m_gpi.isEmpty())
		return false;
	if (m_email.isNull() || m_email.isEmpty())
		return false;
	
	return true;
};

bool Departments::isNew() const {
	return m_id == 0;
};

bool Departments::hasChildren() const {
	return false;
};

bool Departments::save() {

	if (isValid()) {
		QStringList listGPI = m_gpi.split(", ");
		QList<int> listIdGPI = GeodataType::getIDs(listGPI);

		if (m_id == 0) {
			//Создание
			Department* nd = new Department(m_name, m_country, m_address, m_email, m_phone, m_fax, m_comment);
			nd->insertIntoDatabase();
			m_id = nd->id();

			for (int i = 0;i < listIdGPI.count(); ++i)
			{
				DepartmentType(m_id, listIdGPI[i]).insertIntoDatabase();
			}

			delete nd;

		}
		else {
			// Изменение 
			Department* nd = new Department(m_name, m_country, m_address, m_email, m_phone, m_fax, m_comment);
			nd->setId(m_id);
			nd->update();
			m_id = nd->id();
			DepartmentType::deleteByDepartment(m_id);
			for (int i = 0;i < listIdGPI.count(); ++i)
			{
				DepartmentType(m_id, listIdGPI[i]).insertIntoDatabase();
			}
			delete nd;
		}

		return true;
	}
	else
		return false;
};

bool Departments::cancel() {
	if (isNew())
		return true;

	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	query.prepare("SELECT id, name, country, address,  phone, fax, mail, comment FROM departments WHERE id=:id");
	query.bindValue(":id", m_id);
	if (!query.exec())
	{
		qDebug() << query.lastError().text();
	}
	query.next();
	m_name = query.value(1).toString();
	m_country = query.value(2).toString(); 
	m_address = query.value(3).toString();
	m_phone = query.value(4).toString();
	m_fax = query.value(5).toString();
	m_email = query.value(6).toString();
	m_gpi = GeodataType::getListForDepartments(m_id).join(", ");
	m_comment = query.value(7).toString();
	
	return true;
};

QList<BaseItem*> Departments::loadItemsFromDb(QVariant id) {
	QList<BaseItem*> list;
	QSqlDatabase db = Database::database();
	QSqlQuery query(db);
	if (id.isNull())
	{
		if (!query.exec("SELECT id, name, country, address,  phone, fax, mail, comment FROM departments"))
			qDebug() << query.lastError().text();
	}
	else
	{
		query.prepare(
			" SELECT d.id, d.name, d.country, d.address,  d.phone,\
			d.fax, d.mail, d.comment\
			 FROM departments AS d \
			 JOIN department_regions AS dr \
			 ON dr.department_id = d.id \
			WHERE dr.region_id = :region_id "
			);
		query.bindValue(":region_id", id);
		if (!query.exec())
			qDebug() << query.lastError().text();
	}
	while (query.next()) {
		Departments* dep = new Departments();
		dep->m_id = query.value(0).toInt(); 
		dep->m_name = query.value(1).toString();
		dep->m_country = query.value(2).toString(); // 
		dep->m_address = query.value(3).toString();
		dep->m_phone = query.value(4).toString();
		dep->m_fax = query.value(5).toString();
		dep->m_email =query.value(6).toString();
		dep->m_gpi = GeodataType::getListForDepartments(dep->m_id).join(", ");
		dep->m_comment = query.value(7).toString();
		list << dep;

	}

	return list;
};



