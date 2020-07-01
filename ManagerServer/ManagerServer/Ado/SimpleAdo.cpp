
#include "SimpleAdo.h"
#include "AdoRecordSet.h"
#include "AdoCommand.h"


void DumpError(ErrorsPtr Errors)
{
	register long nCount = Errors->GetCount();
	register long nIndex = 0;

	if (Errors == NULL) return;

	while (nIndex < nCount)
	{
		_bstr_t Description;
		_bstr_t Source;
		_bstr_t SqlState;
		long ErrNum;

		ErrorPtr Err = Errors->Item[nIndex++];
		Description = Err->GetDescription();
		Source = Err->GetSource();
		SqlState = Err->GetSQLState();
		ErrNum = Err->GetNumber();
		if (!ErrNum)
			continue;

		SYSTEMTIME cur_time;
		::GetLocalTime(&cur_time);     // ���� �ð��� �д´�.
		// �̰͵� ���͵� �� ������ �����ϰ� ����ڿ��� ��¥�� �ð��� ���ڿ� �������� ����ϰ� �ʹٸ�
		// �Ʒ��� ���� �Լ��� ����ϱ� �ٶ��ϴ�.
		string date, time;
		date.resize(20);
		time.resize(20);
		// ���� �������� ��¥�� "��/��/��"�� �������� date_buff�� �����
		//      12/28/09       (����) ������ �� �� �ڸ��� ǥ�õ�.
		_strdate_s((char*)date.c_str(), date.size());
		//date.shrink_to_fit();
		// ���� �������� �ð��� "��:��:��"�� �������� time_buff�� �����
		//       12:58:59
		_strtime_s((char*)time.c_str(), time.size());
		date.resize(8);
		date.append(" ");
		date.append(time);

		string strMsg = "";
		strMsg.resize(300);
		sprintf_s((char*)strMsg.c_str(), strMsg.size(),
			"Date       : %s\n"
			"Source     : %s\n"
			"Error Code : %u\n"
			"SQL Sate   : %s\n"
			"Description: %s\n",
			date.c_str(),
			(LPCSTR)Source,
			ErrNum,
			(LPCSTR)SqlState,
			(LPCSTR)Description
		);

		printf("%s", strMsg.c_str());

	}

	Errors->Clear();
}

void DumpError(_com_error e)
{
	string date, time;
	date.resize(20);
	time.resize(20);
	// ���� �������� ��¥�� "��/��/��"�� �������� date_buff�� �����
	//      12/28/09       (����) ������ �� �� �ڸ��� ǥ�õ�.
	_strdate_s((char*)date.c_str(), date.size());
	//date.shrink_to_fit();
	// ���� �������� �ð��� "��:��:��"�� �������� time_buff�� �����
	//       12:58:59
	_strtime_s((char*)time.c_str(), time.size());
	date.resize(8);
	date.append(" ");
	date.append(time);

	string strMsg;
	strMsg.resize(500);
	sprintf_s((char*)strMsg.c_str(), strMsg.size(),
		"Date       : %s\n"
		"Description: %s\n",
		date.c_str(),
		(LPCTSTR)e.Description()
	);
	printf("%s", strMsg.c_str());
}


SimpleAdo::SimpleAdo(string strUserId, string strPassWord, string strDbIp, string strDbName, string strPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout)
{
	::CoInitialize(NULL);			// ?? 
	m_pConnection = nullptr;

	this->Connect(strUserId, strPassWord, strDbIp, strDbName, strPort, dbProvider, dbConnectionTimeout, dbCommandTimeout);
}

SimpleAdo::SimpleAdo(string connectionString, string dbConnectionTimeout, string dbCommandTimeout)
{
	::CoInitialize(NULL);			// ?? 
	m_pConnection = nullptr;

	this->Connect(connectionString, dbConnectionTimeout, dbCommandTimeout);
}

SimpleAdo::~SimpleAdo()
{
	::CoUninitialize();
}

void SimpleAdo::Connect(string strUserId, string strPassWord, string strDbIp, string strDbName, string strPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout)
{
	this->Create(strDbIp, strDbName, strPort, dbProvider, dbConnectionTimeout, dbCommandTimeout);
	bool result = this->Open((LPCSTR)m_pConnection->GetConnectionString(), strUserId, strPassWord);
	if (result == false)
	{
		// throw Error;
	}
}

void SimpleAdo::Connect(string connectionString, string dbConnectionTimeout, string dbCommandTimeout)
{
	this->Create(connectionString, dbConnectionTimeout, dbCommandTimeout);
	bool result = this->Open((string)m_pConnection->GetConnectionString());
	if (result == false)
	{
		// throw Error;
	}
}

void SimpleAdo::Create(string strDbIp, string strDbName, string strPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout)
{
	string strConn;
	strConn.resize(200);
	sprintf_s((char*)strConn.c_str(), strConn.size(),
		"Data Source=%s;Initial Catalog=%s;Network Address=%s,%s;Network Library=dbmssocn",
		strDbIp.c_str(), strDbName.c_str(), strDbIp.c_str(), strPort.c_str());

	m_pConnection.CreateInstance(__uuidof(Connection));
	m_pConnection->PutProvider(dbProvider.c_str());
	m_pConnection->PutConnectionTimeout((long)dbConnectionTimeout.c_str());
	m_pConnection->PutCommandTimeout((long)dbCommandTimeout.c_str());

	_bstr_t pbstr(strConn.c_str());
	m_pConnection->PutConnectionString(pbstr);
}

void SimpleAdo::Create(string connectionString, string dbConnectionTimeout, string dbCommandTimeout)
{
	if (connectionString.empty()) {
		// Error�� ���� ��
		exit(1);
		return;
	}

	m_pConnection.CreateInstance(__uuidof(Connection)); //m_pConn.CreateInstance("ADODB.Connection");

	// default
	m_pConnection->PutConnectionTimeout((long)dbConnectionTimeout.c_str());
	m_pConnection->PutCommandTimeout((long)dbCommandTimeout.c_str());

	_bstr_t pbstr(connectionString.c_str());	
	m_pConnection->PutConnectionString(pbstr);
}

bool SimpleAdo::Open(string connectionString)
{
	bool result = false;
	if (connectionString.empty())
		return result;

	try {		
		_bstr_t bstr_empty = "";
		HRESULT hr = m_pConnection->Open(connectionString.c_str(), bstr_empty, bstr_empty, adConnectUnspecified);
		if (hr == S_OK)
			result = true;
		
		// �Ʒ� �κ��� �߰� �˻��� �ǹ� ������... 
		DumpError(m_pConnection->GetErrors());
		if (m_pConnection->GetState() == adStateClosed)
			result = false;
	}
	catch (_com_error &e)
	{
		ErrorQuery(e);
		DumpError(e);
		result = false;
	}

	return result;
}

bool SimpleAdo::Open(string connectionString, string user, string password)
{
	bool result = false;
	if (connectionString.empty()) 
		return result;

	try {
		HRESULT hr = m_pConnection->Open(connectionString.c_str(), user.c_str(), password.c_str(), adConnectUnspecified);
		if (hr == S_OK)
			result = true;

		DumpError(m_pConnection->GetErrors());
		if (m_pConnection->GetState() == adStateClosed)
			result = false;
	}
	catch (_com_error &e)
	{
		DumpError(e);
		result = false;
	}

	return result;
}

void SimpleAdo::Close()
{
	if (m_pRecordSet != nullptr && m_pRecordSet->IsOpen())
	{
		m_pRecordSet->Close();
		delete m_pRecordSet;
	}

	if (m_pCommand != nullptr)
	{
		m_pCommand->Close();
		delete m_pCommand;
	}

	if (this->IsOpen())
	{
		m_pConnection->Close();
	}
}

long SimpleAdo::ExecuteCmd(string query, CommandTypeEnum commandType)
{
	long rowCount = 0;		// ����� variant���� �ο�� ����
	try
	{		
		m_pCommand = new AdoCommand(this, m_pConnection->GetCommandTimeout());
		rowCount = m_pCommand->Execute(query, commandType);
	}
	catch (_com_error& e)
	{
		ErrorQuery(e);
	}

	return rowCount;
}

void SimpleAdo::ExecuteProc(string procName, DynamicParameters& parameters)
{
	try
	{
		m_pCommand = new AdoCommand(this, m_pConnection->GetCommandTimeout());
		// ���� �̱���
		m_pCommand->ExecuteProc();
	}
	catch (_com_error& e)
	{
		ErrorQuery(e);
	}
}

long SimpleAdo::Execute(string query)
{
	// �̷� ����ڵ� �ʹ� ������ ����...
	if (m_pConnection == nullptr) {
		// throw DB ������ �ϼ��� 
	}
	
	_variant_t updatedCount;	// Update�� ����� �ο��
	try
	{
		// Ʈ������� �������� ��ƾ� �Ѵ� 
		//m_pConn->BeginTrans();
		

		m_pConnection->Execute(_bstr_t(query.c_str()), &updatedCount, adExecuteNoRecords);
		//m_pConn->CommitTrans();
		DumpError(m_pConnection->GetErrors());
	}
	catch (_com_error& e)
	{
		//m_pConn->RollbackTrans();
		ErrorQuery(e);
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery ����> �޽���[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery ����> ����[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery ����> �ҽ�[%s] !! ***\n", (LPCSTR)e.Source());
	}

	long effected = atol((char*)((_bstr_t)updatedCount));	// variant���� long���� ����
	if (effected == 0) 
	{
		// ����Ȱ� ����
	}

	return effected;
}

void SimpleAdo::Query(string query, CursorTypeEnum cursorType, CursorLocationEnum cursorLocation, LockTypeEnum lockType, CommandTypeEnum commandType)
{
	try
	{
		m_pRecordSet = new AdoRecordSet(this);
		bool result = m_pRecordSet->Query(query, cursorType, cursorLocation, lockType, commandType);
		if (result == false) 
		{
			// throew Error;
		}
	}
	catch (_com_error& e)
	{
		printf("*** DOG CHECK [ADO DB] => <OpenQuery ����> �޽���[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <OpenQuery ����> ����[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <OpenQuery ����> �ҽ�[%s] !! ***\n", (LPCSTR)e.Source());
	}
}

bool SimpleAdo::IsOpen()
{	
	try {
		return ((m_pConnection->GetState() == adStateOpen) ? true : false);
	}
	catch (_com_error &e)
	{
		DumpError(e);
		return false;
	}
}

bool SimpleAdo::Move(long nIndex)
{
	return Record().Move(nIndex);
}
bool SimpleAdo::MoveLast(void)
{
	return Record().MoveLast();
}
bool SimpleAdo::MovePrevious(void)
{
	return Record().MovePrevious();
}
bool SimpleAdo::MoveNext(void)
{
	return Record().MoveNext();
}
bool SimpleAdo::MoveFirst(void)
{
	return Record().MoveFirst();
}

long SimpleAdo::GetRecordCount(void)
{
	if (m_pRecordSet == nullptr) return 0;

	return m_pRecordSet->GetRecordCount();
}

long SimpleAdo::GetCurrentRecordCount(void)
{
	if (m_pRecordSet == nullptr) return 0;

	return m_pRecordSet->GetCurrentRecordCount();
}

long SimpleAdo::GetFieldCount(void)
{
	if (m_pRecordSet == nullptr) return 0;

	return m_pRecordSet->GetFieldCount();
}

void SimpleAdo::DisConnect()
{
	try
	{
		this->Close();
		m_pConnection.Release();
		m_pConnection = nullptr;
	}
	catch (_com_error& e)
	{
		DumpError(e);
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect ����> �޽���[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect ����> ����[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect ����> �ҽ�[%s] !! ***\n", (LPCSTR)e.Source());
	}	
	delete this;
}




void SimpleAdo::ErrorQuery(_com_error& e)
{
	_bstr_t bstrDescription(e.Description());
	printf("error - %s\n", (LPCSTR)bstrDescription);
	printf("*** DOG CHECK [ADO DB] => <DBConnect ����> �޽���[%Ls] !! ***\n", e.ErrorMessage());
	printf("*** DOG CHECK [ADO DB] => <DBConnect ����> ����[%s] !! ***\n", (LPCSTR)e.Description());
	printf("*** DOG CHECK [ADO DB] => <DBConnect ����> �ҽ�[%s] !! ***\n", (LPCSTR)e.Source());
	//exit(1); // ?? �̰� ��
}


