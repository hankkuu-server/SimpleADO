#pragma once

#include "SimpleAdo.h"
#include "AdoField.h"

class SimpleAdo;

class AdoRecordSet
{
public:
	AdoRecordSet(SimpleAdo* ado);
	virtual ~AdoRecordSet();
	void Create();
	bool IsOpen();
	void Close();
	bool Query(string szQuery, CursorTypeEnum cursorType = adOpenDynamic, CursorLocationEnum cursorLocation = adUseClient, LockTypeEnum lockType = adLockOptimistic, CommandTypeEnum commandType = adCmdText);
	AdoField& operator[] (string szColumnName);
	AdoField& operator[] (int nColumnIndex);
	bool MoveFirst();
	bool MoveNext();
	bool MovePrevious();
	bool MoveLast();
	bool Move(long nIndex);
	long GetRecordCount();
	long GetCurrentRecordCount();
	long GetFieldCount();

private:
	bool IsBOF();
	bool IsEOF();
	bool Reflash();


private:
	SimpleAdo* m_pAdo;
	_RecordsetPtr	m_pRecordset;
	_ConnectionPtr m_pConnection;
	// ORM �������ϴ� ���̺� ������ �ڷᱸ���� ����� �־ �ȴ�.. 
	// ������ ������ ���� �ʿ��� �κ��� �ƴ�
	vector<AdoField*> m_adoField;		
	// map<long, vector<AdoField*>> m_adoTable; �ϳ��� Ű���� �ϳ��� Row�� ������ 
	long m_recordCount;
	long m_currentCount;
	long m_fieldCount;
	bool m_start;

};

