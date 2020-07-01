#pragma once
#include <string>
#include <map>
using namespace std;

class DynamicParameters
{
public:
	DynamicParameters();
	virtual ~DynamicParameters();

	// 아래 방법은 권장사항은 아닌것 같다 
	//void SetWhere(Where where);
	//void SetValuse(Values values);
	//void SetSet(Set set);

	// 차라리 순번을 지키는 것으로 쿼리를 만들자 SQL parse 역할을 하는 녀석이 있고 순서대로 파라미터를 셋팅하자 
	void Add(string key, double value);
	void Add(string key, int value);
	void Add(string key, string value);

private:
	map<string, string> paramters;
};

//// Template 적용이 필요하다 
//class Parameter
//{
//public:
//	Parameter()
//};