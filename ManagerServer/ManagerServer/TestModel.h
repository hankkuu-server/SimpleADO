#pragma once

using namespace std;
#include <string>

class TestModel
{
public:
	string GetName() { return name; }
	string SetName(string name) { this->name = name; }

	string GetIpAddress() { return ipAddress; }
	string SetIpAddress(string ipAddress) { this->ipAddress = ipAddress; }

	string GetId() { return id; }
	string SetId(string id) { this->id = id; }

private:
	string name;
	string ipAddress;
	string id;
};

