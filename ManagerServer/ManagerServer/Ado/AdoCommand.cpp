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
		// 연결 재설정을 하는 게 맞는지?? 
		// 아니면 에러로 보고 하는게 맞다
		// m_pAdo->Open(szConnection);
		// throws Error;
	}

	try
	{
		_variant_t updatedCount;
		m_pCommand->PutCommandText(query.c_str());

		bool result = m_pCommand->Execute(&updatedCount, NULL, commandType); // Update 실행
		if (result == true) {
			effected = atol((char*)((_bstr_t)updatedCount));	// variant형을 long형에 저장
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
