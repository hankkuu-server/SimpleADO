#include "AdoField.h"

AdoField::AdoField(FieldPtr field)
{
	this->m_pField = field;
}

AdoField::~AdoField()
{	
	m_pField.Detach();
}

void AdoField::Close()
{
	if (m_pField != nullptr)
	{
		m_pField.Release();
	}
}

string AdoField::GetFieldName()
{
	try 
	{
		string fieldName = (LPCSTR)m_pField->GetName();
		return fieldName;
	}
	catch(_com_error &e)
	{		
		//DumpError(e);
	}
	return string();
}

void AdoField::operator=(long iValue)
{
	_variant_t vtValue(iValue);
	this->operator =(vtValue);
}

void AdoField::operator=(int iValue)
{
	_variant_t vtValue((long)iValue);
	this->operator =(vtValue);
}

void AdoField::operator=(double dblValue)
{
	_variant_t vtValue(dblValue);
	this->operator =(vtValue);
}

void AdoField::operator=(float fValue)
{
	_variant_t vtValue(fValue);
	this->operator =(vtValue);
}

void AdoField::operator=(short sValue)
{
	_variant_t vtValue(sValue);
	this->operator =(vtValue);
}

void AdoField::operator=(char cValue)
{
	_variant_t vtValue((BYTE)cValue);
	this->operator =(vtValue);
}

void AdoField::operator=(LPCTSTR szValue)
{
	_variant_t vtValue(_bstr_t((char*)szValue));
	this->operator =(vtValue);
}

void AdoField::operator=(bool bValue)
{
	_variant_t vtValue(bValue);
	this->operator =(vtValue);
}

_variant_t AdoField::operator=(_variant_t vtValue)
{
	try {
		if (m_pField != NULL)
			m_pField->Value = vtValue;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}

	return m_pField->Value;
}


AdoField::operator int()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_INT);			// signed machine int
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.lVal;
}

AdoField::operator string()
{
	_variant_t vt;
	string strBuf = "";
	try 
	{
		vt = m_pField->Value;
		vt.ChangeType(VT_BSTR);
		if (vt.vt == VT_NULL || vt.vt == VT_EMPTY) 
		{
			//strBuf = "";
		}			
		else
		{
			_bstr_t bstr = vt.bstrVal;
			strBuf = (LPCSTR)bstr;
		}
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return strBuf;
}

AdoField::operator long()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_I4);			// 4 byte signed int
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.lVal;
}

AdoField::operator double()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_R8);		// 8 byte real
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.dblVal;
}

AdoField::operator float()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_R4);		// 4 byte real
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.fltVal;
}

AdoField::operator short()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_I2);		// 2 byte signed int
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.intVal;
}

AdoField::operator char()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
		vt.ChangeType(VT_UI1);	// unsinged char
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt.cVal;
}

AdoField::operator VARIANT()
{
	_variant_t vt;
	vt.Clear();
	try {
		if (m_pField != NULL)
			vt = m_pField->Value;
	}
	catch (_com_error &e)
	{
		//DumpError(e);
	}
	return vt;
}