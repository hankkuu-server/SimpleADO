#include "AdoCommand.h"

AdoCommand::AdoCommand(SimpleAdo* ado, long commandTimeout)
{
	m_pAdo = ado;
	m_pCommand = nullptr;
	this->Create(commandTimeout);
}

AdoCommand::~AdoCommand()
{

}

void AdoCommand::Create(long commandTimeout)
{
	try
	{
		m_pCommand.CreateInstance(__uuidof(Command));
		m_pCommand->PutCommandTimeout(commandTimeout);
		if ((m_pAdo != nullptr) && m_pAdo->Connections() != nullptr)
			m_pCommand->PutRefActiveConnection(m_pAdo->Connections());
	}
	catch (_com_error& e)
	{
		//DumpError(e);
	}
}

void AdoCommand::Close()
{
	m_pCommand.Release();
}

long AdoCommand::Execute(string query, CommandTypeEnum commandType)
{
	long effected = 0;
	if (query.empty())
		return effected;

	if (m_pAdo->IsOpen() == false)
	{
		//m_pAdo->Close();
		return effected;
		// ���� �缳���� �ϴ� �� �´���?? 
		// �ƴϸ� ������ ���� �ϴ°� �´�
		// m_pAdo->Open(szConnection);
		// throws Error;
	}

	try
	{
		_variant_t updatedCount;
		m_pCommand->PutCommandText(query.c_str());

		bool result = m_pCommand->Execute(&updatedCount, NULL, commandType); // Update ����
		if (result == true) {
			effected = atol((char*)((_bstr_t)updatedCount));	// variant���� long���� ����
		} 
	}
	catch (_com_error& e)
	{
		
	}

	return effected;
}

bool AdoCommand::ExecuteProc()
{
	return false;
}
