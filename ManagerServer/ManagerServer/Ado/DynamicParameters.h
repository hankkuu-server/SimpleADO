#pragma once
#include <string>
#include <map>
using namespace std;

class DynamicParameters
{
public:
	DynamicParameters();
	virtual ~DynamicParameters();

	// �Ʒ� ����� ��������� �ƴѰ� ���� 
	//void SetWhere(Where where);
	//void SetValuse(Values values);
	//void SetSet(Set set);

	// ���� ������ ��Ű�� ������ ������ ������ SQL parse ������ �ϴ� �༮�� �ְ� ������� �Ķ���͸� �������� 
	void Add(string key, double value);
	void Add(string key, int value);
	void Add(string key, string value);

private:
	map<string, string> paramters;
};

//// Template ������ �ʿ��ϴ� 
//class Parameter
//{
//public:
//	Parameter()
//};