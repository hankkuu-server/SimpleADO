
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
		::GetLocalTime(&cur_time);     // 현재 시간을 읽는다.
		// 이것도 저것도 다 귀찮고 간단하게 사용자에게 날짜와 시간을 문자열 형식으로 출력하고 싶다면
		// 아래와 같은 함수를 사용하길 바랍니다.
		string date, time;
		date.resize(20);
		time.resize(20);
		// 현재 지역기준 날짜가 "월/일/년"의 형식으로 date_buff에 저장됨
		//      12/28/09       (주의) 연도는 끝 두 자리만 표시됨.
		_strdate_s((char*)date.c_str(), date.size());
		//date.shrink_to_fit();
		// 현재 지역기준 시간이 "시:분:초"의 형식으로 time_buff에 저장됨
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
	// 현재 지역기준 날짜가 "월/일/년"의 형식으로 date_buff에 저장됨
	//      12/28/09       (주의) 연도는 끝 두 자리만 표시됨.
	_strdate_s((char*)date.c_str(), date.size());
	//date.shrink_to_fit();
	// 현재 지역기준 시간이 "시:분:초"의 형식으로 time_buff에 저장됨
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
		// Error를 뱉어야 함
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
		
		// 아래 부분은 추가 검사의 의미 있을지... 
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
	long rowCount = 0;		// 적용된 variant형을 로우수 저장
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
		// 아직 미구현
		m_pCommand->ExecuteProc();
	}
	catch (_com_error& e)
	{
		ErrorQuery(e);
	}
}

long SimpleAdo::Execute(string query)
{
	// 이런 방어코드 너무 싫은거 같다...
	if (m_pConnection == nullptr) {
		// throw DB 연결을 하세요 
	}
	
	_variant_t updatedCount;	// Update가 적용된 로우수
	try
	{
		// 트랜잭션은 상위에서 잡아야 한다 
		//m_pConn->BeginTrans();
		

		m_pConnection->Execute(_bstr_t(query.c_str()), &updatedCount, adExecuteNoRecords);
		//m_pConn->CommitTrans();
		DumpError(m_pConnection->GetErrors());
	}
	catch (_com_error& e)
	{
		//m_pConn->RollbackTrans();
		ErrorQuery(e);
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 메시지[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 소스[%s] !! ***\n", (LPCSTR)e.Source());
	}

	long effected = atol((char*)((_bstr_t)updatedCount));	// variant형을 long형에 저장
	if (effected == 0) 
	{
		// 변경된게 없다
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
		printf("*** DOG CHECK [ADO DB] => <OpenQuery 오류> 메시지[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <OpenQuery 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <OpenQuery 오류> 소스[%s] !! ***\n", (LPCSTR)e.Source());
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
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 메시지[%Ls] !! ***\n", e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 소스[%s] !! ***\n", (LPCSTR)e.Source());
	}	
	delete this;
}




void SimpleAdo::ErrorQuery(_com_error& e)
{
	_bstr_t bstrDescription(e.Description());
	printf("error - %s\n", (LPCSTR)bstrDescription);
	printf("*** DOG CHECK [ADO DB] => <DBConnect 오류> 메시지[%Ls] !! ***\n", e.ErrorMessage());
	printf("*** DOG CHECK [ADO DB] => <DBConnect 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
	printf("*** DOG CHECK [ADO DB] => <DBConnect 오류> 소스[%s] !! ***\n", (LPCSTR)e.Source());
	//exit(1); // ?? 이건 왜
}


