#pragma once

#include <string>
#include <map>
using namespace std;

class DBConnection
{
public:
	string GetUserName() { return m_userName; }
	string GetUserPassword() { return m_userPassword; }
	string GetDbIpAddress() { return m_dbIpAddress; }
	string GetDbName() { return m_dbName; }
	string GetDbPort() { return m_dbPort; }
	string GetDbProvider() { return m_dbProvider; }
	string GetDbConnectionTimeout() { return m_dbConnectionTimeout; }
	string GetDbCommandTimeout() { return m_dbCommandTimeout; }

	DBConnection(string userName, string userPassword, string dbIpAddress, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout)
	{
		m_userName = userName;
		m_userPassword = userPassword;
		m_dbIpAddress = dbIpAddress;
		m_dbName = dbName;
		m_dbPort = dbPort;
		m_dbProvider = dbProvider;
		m_dbConnectionTimeout = dbConnectionTimeout;
		m_dbCommandTimeout = dbCommandTimeout;
	}

private:
	string m_userName;
	string m_userPassword;
	string m_dbIpAddress;
	string m_dbName;
	string m_dbPort;
	string m_dbProvider;
	string m_dbConnectionTimeout;
	string m_dbCommandTimeout;
};

class ConnectionBuilder
{
public:
	virtual DBConnection* GetConnection() = 0;
	virtual void Build(string userName, string userPassword, string dbIpAddress, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout) = 0;
	virtual ~ConnectionBuilder() {}
};

class MSSqlConnection : public ConnectionBuilder
{
public:
	~MSSqlConnection()
	{		
		delete m_pDbConnection; 
		m_pDbConnection = nullptr;		
	}
private:
	void Build(string userName, string userPassword, string dbIpAddress, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout) override
	{
		m_pDbConnection = new DBConnection(userName, userPassword, dbIpAddress, dbName, dbPort, dbProvider, dbConnectionTimeout, dbCommandTimeout);
	}
	DBConnection* GetConnection() override { return m_pDbConnection; }

private:
	DBConnection* m_pDbConnection;

};


class Director
{
public:
	Director(ConnectionBuilder* builder) : m_pBuilder(builder) {}
	~Director() 
	{ 
		if (m_pBuilder) 
			delete m_pBuilder; 
	}

	DBConnection* Build(string userName, string userPassword, string dbIpAddress, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout)
	{
		m_pBuilder->Build(userName, userPassword, dbIpAddress, dbName, dbPort, dbProvider, dbConnectionTimeout, dbCommandTimeout);
		return m_pBuilder->GetConnection();
	}
private:
	ConnectionBuilder* m_pBuilder;
};

class AppConfigManager
{
public:	
	~AppConfigManager();

	void LoadFile();
	DBConnection* GetDBConnection(string key);
	string GetDBConnectionString(string key);

	static	AppConfigManager* Instance();
	static	void ExitInstance();
private:
	AppConfigManager();

private:
	static AppConfigManager* m_pInstance;

	Director* m_director;

	// 생성과 동시에 설정파일 정보를 모두 가져와야 한다 
	map<string, DBConnection*> m_connections;
};



