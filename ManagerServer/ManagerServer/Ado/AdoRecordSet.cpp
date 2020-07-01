#include "AdoRecordSet.h"
#include "AdoField.h"

AdoRecordSet::AdoRecordSet(SimpleAdo* ado)
{
	m_pAdo = ado;
	m_pRecordset = nullptr;
	m_start = false;
	m_recordCount = 0;
	m_fieldCount = 0;
	m_currentCount = 0;

	this->Create();
}

AdoRecordSet::~AdoRecordSet()
{
	if (m_pRecordset != nullptr)
	{
		m_pRecordset.Release();
		m_pRecordset = nullptr;
	}
}

void AdoRecordSet::Create()
{
	try {
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		if ((m_pAdo != nullptr) && (m_pAdo->Connections() != nullptr))
			m_pRecordset->PutActiveConnection(_variant_t((IDispatch*)m_pAdo->Connections()));
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
}

bool AdoRecordSet::IsOpen()
{
	try {
		return ((m_pRecordset->GetState() == adStateOpen) ? TRUE : FALSE);
	}
	catch (_com_error &e)
	{
		//DumpError(e);
		return false;
	}
}


void AdoRecordSet::Close()
{
	try {
		vector<AdoField*>::iterator iter;
		for (iter = m_adoField.begin(); iter != m_adoField.end(); iter++)
		{
			(*iter)->Close();
			delete* iter;
		}
		m_adoField.clear();

		if (m_pRecordset->GetState() != adStateClosed)
			m_pRecordset->Close();
	}
	catch (_com_error& e)
	{
		//DumpError(e);
	}

}

bool AdoRecordSet::Query(string query, CursorTypeEnum cursorType, CursorLocationEnum cursorLocation, LockTypeEnum lockType, CommandTypeEnum commandType)
{	
	bool result = false;
	if (query.empty()) 
		return result;

	if (m_pAdo->IsOpen() == false)
	{
		//m_pAdo->Close();
		return result;
		// 연결 재설정을 하는 게 맞는지?? 
		// 아니면 에러로 보고 하는게 맞다
		// m_pAdo->Open(szConnection);
		// throws Error;
	}

	try {
		HRESULT hr = m_pRecordset->Open(_variant_t(_bstr_t(query.c_str())), _variant_t((IDispatch*)m_pAdo->Connections()), adOpenStatic, adLockUnspecified, adCmdText);
		if (hr == S_OK)
			result = true;
				
		m_recordCount = (long)m_pRecordset->GetRecordCount();
		m_fieldCount = m_pRecordset->GetFields()->GetCount();
	}
	catch (_com_error &e)
	{
		result = false;
		//DumpError(e);
	}
	return result;
}

AdoField& AdoRecordSet::operator[] (string columnName)
{	
	AdoField* field = nullptr;
	try {
		field = new AdoField(m_pRecordset->GetFields()->Item[_bstr_t(columnName.c_str())]);
		m_adoField.push_back(field);
		return *field;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
		return *field;
	}
}

AdoField& AdoRecordSet::operator[] (int nColumnIndex)
{
	AdoField* field = nullptr;
	try {
		field = new AdoField(m_pRecordset->GetFields()->Item[_variant_t((long)nColumnIndex)]);
		m_adoField.push_back(field);
		return *field;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
		return *field;
	}
}

bool AdoRecordSet::MoveFirst()
{
	bool result = false;
	try {
		HRESULT hResult = m_pRecordset->MoveFirst();
		if(hResult == S_OK)
			result = true;
		
		// 18
		m_currentCount = 0;
		m_start = true;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return result;
}

bool AdoRecordSet::MoveNext()
{
	// c8
	if (m_currentCount == m_recordCount)
		return false;

	bool result = false;
	m_currentCount++;
	try {
		if (m_start == false) {
			m_start = true;
			return true;
		}		
		
		HRESULT hResult = m_pRecordset->MoveNext();
		if (hResult == S_OK)
			result = true;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 메시지[%s] !! ***\n", (LPSTR)(LPCTSTR)e.ErrorMessage());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		printf("*** DOG CHECK [ADO DB] => <DBDisConnect 오류> 소스[%s] !! ***\n", (LPCSTR)e.Source());
	}
	return result;
}

bool AdoRecordSet::MovePrevious()
{
	if (m_currentCount == 0)
	{
		m_start = true;
		return false;
	}		

	bool result = false;
	try {
		HRESULT hResult = m_pRecordset->MovePrevious();
		if (hResult == S_OK)
			result = true;

		m_currentCount--;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return result;
}

bool AdoRecordSet::MoveLast()
{
	bool result = false;
	try {
		HRESULT hResult = m_pRecordset->MoveLast();
		if (hResult == S_OK)
			result = true;

		m_currentCount = m_recordCount;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return result;
}

bool AdoRecordSet::Move(long index)
{
	//c87
	if (index < m_recordCount)
		return false;

	bool result = false;
	try {
		HRESULT hResult = m_pRecordset->Move(index);
		if (hResult == S_OK)
			result = true;

		if (index == 0)
			m_start = true;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return result;
}

bool AdoRecordSet::IsBOF()
{
	bool result = false;
	try {
		if (m_pRecordset->BOF) 
			result = true;
	}
	catch (_com_error &e)
	{
		//DumpError(e);		
	}
	return result;
}

bool AdoRecordSet::IsEOF()
{
	bool result = false;
	try {
		if (m_pRecordset->EndOfFile)
			result = true;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return result;
}

long AdoRecordSet::GetRecordCount()
{
	return m_recordCount;
}

long AdoRecordSet::GetCurrentRecordCount()
{
	return m_currentCount;
}

long AdoRecordSet::GetFieldCount()
{
	return m_fieldCount;
}


bool AdoRecordSet::Reflash()
{
	bool result = false;
	try {
		if (m_pRecordset != NULL)
		{
			m_pRecordset->Requery(adCmdUnspecified);
			if (!(IsEOF() && IsBOF()))
			{
				m_pRecordset->MoveFirst();
				result = true;
			}				
		}
	}
	catch (_com_error& e)
	{
		//DumpError(e);
	}
	return result;
}

