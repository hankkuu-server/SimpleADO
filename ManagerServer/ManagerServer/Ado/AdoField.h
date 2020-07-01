#pragma once
#include "SimpleAdo.h"

class AdoField
{
public:
	AdoField(FieldPtr field);
	virtual ~AdoField();

	void Close();
	FieldPtr GetFieldPtr() { return m_pField; }
	string GetFieldName();

	operator int();
	operator string();
	operator long();
	operator double();
	operator float();
	operator short();
	operator char();
	operator VARIANT();

	void operator=(long iValue);
	void operator=(int iValue);
	void operator=(double dblValue);
	void operator=(float fValue);
	void operator=(short sValue);
	void operator=(char cValue);
	void operator=(LPCTSTR szValue);
	void operator=(bool bValue);
	_variant_t operator=(_variant_t vtValue);

private:
	FieldPtr m_pField;
};

