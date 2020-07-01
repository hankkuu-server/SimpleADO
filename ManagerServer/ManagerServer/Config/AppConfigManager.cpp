#include "AppConfigManager.h"

AppConfigManager* AppConfigManager::m_pInstance = nullptr;

AppConfigManager* AppConfigManager::Instance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new AppConfigManager();
	}
	return m_pInstance;
}

void AppConfigManager::ExitInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

AppConfigManager::AppConfigManager()
{
	LoadFile();
}

AppConfigManager::~AppConfigManager()
{
	delete m_director;
}

void AppConfigManager::LoadFile()
{
	m_director = new Director(new MSSqlConnection());

	DBConnection* db;	
	// 설정파일의 로딩부분은 아직 구현하지 않는다 임시로 형태만 만든다 
	//Open("educ_user", "educ123", "192.168.10.3", "EDUCDB", "1433");
	string userName = "sa";
	string userPassword = "3300";
	string dbIpAddress = "192.168.10.3";
	string dbName = "NMS";
	string dbPort = "1433";
	string dbProvider = "SQLOLEDB";
	string dbConnectionTimeout = "10";
	string dbCommandTimeout = "30";
		
	db = m_director->Build(userName, userPassword, dbIpAddress, dbName, dbPort, dbProvider, dbConnectionTimeout, dbCommandTimeout);
	m_connections.insert(make_pair("CollectionDB", db));
}

DBConnection* AppConfigManager::GetDBConnection(string key)
{
	map<string, DBConnection*>::const_iterator findIter = m_connections.find(key);
	DBConnection* connection = findIter->second;

	return connection;
}

string AppConfigManager::GetDBConnectionString(string key)
{
	map<string, DBConnection*>::const_iterator findIter = m_connections.find(key);
	if (findIter != m_connections.end())
	{
		DBConnection* connection = findIter->second;

		string strConn;
		strConn.resize(200);
		sprintf_s((char*)strConn.c_str(), strConn.size(),
			"Provider=%s;User ID=%s;Password=%s;Data Source=%s;Initial Catalog=%s;Network Address=%s,%s;Network Library=dbmssocn",
			connection->GetDbProvider().c_str(),
			connection->GetUserName().c_str(), connection->GetUserPassword().c_str(), connection->GetDbIpAddress().c_str(), connection->GetDbName().c_str(), connection->GetDbIpAddress().c_str(), connection->GetDbPort().c_str());

		return strConn;
	}
	
	return string();
}


