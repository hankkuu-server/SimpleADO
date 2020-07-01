#pragma once

using namespace std;

#include <string>
#include <vector>
#include <ctime>

#pragma warning(push)                         //ado 버그로 인한 컴파일시 경고 뜨는것 방지.
#pragma warning(disable:4146)
#import "C:\program files\common files\system\ado\msado21.tlb" no_namespace rename("EOF", "EndOfFile") 
#pragma warning(pop) 

//#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")

class AdoRecordSet;
class AdoCommand;
class DynamicParameters;

class SimpleAdo
{
public:
	SimpleAdo(string userName, string password, string dbIp, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout);
	SimpleAdo(string connectionString, string dbConnectionTimeout = "30", string dbCommandTimeout = "30");
	virtual ~SimpleAdo();

	bool IsOpen();
	void Query(string query, CursorTypeEnum cursorType = adOpenDynamic, CursorLocationEnum cursorLocation = adUseClient, LockTypeEnum lockType = adLockOptimistic, CommandTypeEnum commandType = adCmdText);

	long Execute(string query);
	long ExecuteCmd(string query, CommandTypeEnum commandType = adCmdText);
	void ExecuteProc(string procName, DynamicParameters& parameters);


	bool Move(long nIndex);
	bool MoveLast(void);
	bool MovePrevious(void);
	bool MoveNext(void);
	bool MoveFirst(void);

	long GetRecordCount(void);
	long GetCurrentRecordCount(void);
	long GetFieldCount(void);
	_ConnectionPtr Connections() { return m_pConnection; }
	AdoRecordSet& Record() { return *m_pRecordSet; }

	void DisConnect();



	void ErrorQuery(_com_error& e);	
		
	
	//void ExecuteProc();	
	//template<typename T1>
	//bool Query(T1 model, string strSQL);

private:
	void Connect(string userName, string password, string dbIP, string dbName, string dbPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout);
	void Connect(string connectionString, string dbConnectionTimeout, string dbCommandTimeout);
	
	void Create(string strDbIp, string strDbName, string strPort, string dbProvider, string dbConnectionTimeout, string dbCommandTimeout);
	void Create(string connectionString, string dbConnectionTimeout, string dbCommandTimeout);

	bool Open(string connectionString);
	bool Open(string connectionString, string userName, string password);
		
	void Close(void);



private:
	_ConnectionPtr m_pConnection;
	AdoRecordSet* m_pRecordSet;
	AdoCommand* m_pCommand;

};

//// 리플렉션으로 따로 구현해야 할듯 하다 C++ 라이브러리를 추가로 붙여야 하겠다.. 
//template<typename T1>
//BOOL SimpleAdo::Query(T1 model, string strSQL)
//{
//	if (m_pConn->GetState() != adStateOpen)
//		return FALSE;
//
//	long nRowSize = 0;
//	// 알맞은 쿼리를 대입하십시요.
//	//m_MyAdodb.m_pComm->CommandText = "Select * From Test_Tbl where ~~~~";
//	//m_pRs = m_pComm->Execute(NULL, NULL, adOptionUnspecified);
//	try {
//		_bstr_t btQry = (_bstr_t)strSQL.c_str();
//
//		m_pRs = m_pConn->Execute(btQry, NULL, adOptionUnspecified);
//		long nColSize = m_pRs->GetFields()->GetCount();
//		_variant_t vtQryData;
//
//		vector<_variant_t>* vtData;
//		//if (vtData != NULL)
//		//	delete[] vtData;
//
//		vtData = new vector<_variant_t>[nColSize];
//		while (!m_pRs->GetEndOfFile())
//		{
//			for (int i = 0; i < nColSize; ++i)
//			{
//				vtQryData = m_pRs->GetCollect(m_pRs->Fields->GetItem(_variant_t((long)i))->GetName());
//				vtData[i].push_back(vtQryData);
//				vtQryData.Clear();
//			}
//			m_pRs->MoveNext();
//			++nRowSize;
//		}
//
//	}
//	catch (_com_error& e)
//	{
//		//DumpError(e);
//		return FALSE;
//	}
//
//	return TRUE;
//}
