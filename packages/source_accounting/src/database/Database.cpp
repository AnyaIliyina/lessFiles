#include "Database.h"
#include "Department.h"
#include "Format.h"
#include "State.h"
#include "Scale.h"
#include "SiteLang.h"
#include "User.h"
#include "Status.h"
#include "Session.h"
#include "Usertype.h"
#include "Log.h"
#include "GeodataType.h"
#include "Site.h"
#include "Language.h"
#include "Region.h"
#include "SiteRegion.h"
#include "SiteType.h"
#include "DepartmentRegion.h"
#include "DepartmentType.h"
#include <QFileInfo>
#include <QStandardPaths>
/*!
\file
*/

const QString Database::pathToDb = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QString("/database/geoDB"); // путь к базе

const QString Database::connectionName = "geoDb_connection";	// название подключения

int Database::CurrentSessionId = 0;

int Database::SmSessionId = 0;

int Database::SystemSessionId = 1;

																/*!
Создает подключение connectionName к базе.
Если необходимо, заново создает таблицы базы pathToDb 
*/
void Database::restore(int session_id)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
	auto pathToDb = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QString("/geoDB");
	db.setDatabaseName(pathToDb);

	bool allow_create = false;
	QFileInfo dbFile(pathToDb);
	if (!dbFile.exists())
		allow_create = true;
	db.open();
	QSqlQuery query(db);
	if (!query.exec("PRAGMA foreign_keys = ON"))
		qDebug() << query.lastError().text();

	// Если файла pathToDb не существует, создать все таблицы:
	if (allow_create) {
		configure();
		Log::create(session_id, "Database: restore");
	}
}

void Database::setCurrentSessionId(int _session_id)
{
	CurrentSessionId = _session_id;
}

void Database::setSmSessionId(int smSessionId)
{
	SmSessionId = smSessionId;
}

void Database::setSystemSessionId(int systemSessionId)
{
	SystemSessionId = systemSessionId;
}

int Database::currentSessionId()
{
	return CurrentSessionId;
}

int Database::smSessionId()
{
	return SmSessionId;
}

int Database::systemSessionId()
{
	return SystemSessionId;
}


/*!
Возвращает открытое подключение к базе.
\return QSqlDatabase db - подключение к базе
*/
QSqlDatabase Database::database()
{
	if (!QSqlDatabase::contains (connectionName))
		qDebug() << "Database::database() error: connection " << connectionName << " not found";
	
	return QSqlDatabase::database(connectionName, true);
}



/*!
Создает в базе таблицы "Sites", "Formats", 
"State", "Scale", "User", "Status", "Session", "Usertype"
*/
void Database::configure()
{
		Usertype::createTable();
		Usertype::completeTable();
		User::createTable();
		User::completeTable();
		Log::createTable();
		Session::createTable();
		Session::completeTable();
		Language::createTable();
		Language::completeTable();
		Site::createTable();
		Site::completeTable();
		Format::createTable();
		Format::completeTable();
		State::createTable();
		State::completeTable();
		Scale::createTable();
		Scale::completeTable();
		Status::createTable();
		Status::completeTable();
		//Geodata_record::createTable();
		// Geodata_record::completeTable();
		GeodataType::createTable();
		GeodataType::completeTable();
		Department::createTable();
		Department::completeTable();
		Region::createTable();
		Region::completeTable();
		createBridgeTables();
}

void Database::createBridgeTables()
{
	SiteRegion::createTable();
	SiteLang::createTable();
	SiteType::createTable();
	DepartmentRegion::createTable();
	DepartmentType::createTable();
}

