#pragma once
#include "SimpleAdo.h"

class AdoCommand
{
public:
	AdoCommand(SimpleAdo* ado, long commandTimeout = 30);
	~AdoCommand();
	void Create(long commandTimeout);
	void Close();
	long Execute(string query, CommandTypeEnum commandType = adCmdText);
	bool ExecuteProc();

private:
	_CommandPtr m_pCommand;
	SimpleAdo* m_pAdo;
};

