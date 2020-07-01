// ManagerServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "Ado/SimpleAdo.h"
#include "Ado/AdoRecordSet.h"
#include "Ado/DynamicParameters.h"
#include "Config/AppConfigManager.h"
#include "TestModel.h"
#include "Util/StringUtil.h"
#include "Util/DateUtil.h"

using namespace std;

int main()
{
	SimpleAdo* selectCommand = new SimpleAdo(AppConfigManager::Instance()->GetDBConnectionString("CollectionDB")); // 아래 연결은 생성자에서 하게 한다   
	
	//제네릭이 구현되어야 한다 템플릿을 확인해 볼것  myAdodb->Query<ModelList>("Select * From EDUC_IP_LIST");			
	//List<Model> list = new List<Model>();
	selectCommand->Query("SELECT * FROM DEVICE");
	while (selectCommand->MoveNext())
	{
		// Model model = new Model(id, name, email);
		// list.add(model);
		long deviceId = (long)selectCommand->Record()["DeviceId"];
		string ip = (string)selectCommand->Record()["IP"];
		string deviceName = (string)selectCommand->Record()["DeviceName"];
		string community = (string)selectCommand->Record()["Community"];
		string createDate = (string)selectCommand->Record()["CreateDate"];
		string updateDate = (string)selectCommand->Record()["UpdateDate"];
		long vendorType = (long)selectCommand->Record()["VendorType"];
	}
	selectCommand->DisConnect();

	SimpleAdo* insertCommand = new SimpleAdo(AppConfigManager::Instance()->GetDBConnectionString("CollectionDB"));
	string query1 = StringUtil::Format("INSERT INTO DEVICE VALUES('%s', '%s', '%s', '%s', '%s', %d)", "192.168.10.2", "LinuxServer", "nms", DateUtil::Now().c_str(), DateUtil::Now().c_str(), 0 );
	insertCommand->Execute(query1);
	insertCommand->DisConnect();

	SimpleAdo* insertCommand2 = new SimpleAdo(AppConfigManager::Instance()->GetDBConnectionString("CollectionDB"));
	string query2 = StringUtil::Format("INSERT INTO DEVICE VALUES('%s', '%s', '%s', '%s', '%s', %d)", "192.168.10.5", "WindowsPC", "nms", DateUtil::Now().c_str(), DateUtil::Now().c_str(), 0);
	insertCommand2->Execute(query2);
	insertCommand2->DisConnect();

	SimpleAdo* updateCommand = new SimpleAdo(AppConfigManager::Instance()->GetDBConnectionString("CollectionDB"));
	string query3 = StringUtil::Format("UPDATE DEVICE SET IP = '%s' WHERE IP = '%s'", "192.168.10.4", "192.168.10.5");
	updateCommand->ExecuteCmd(query3);
	updateCommand->DisConnect();

	SimpleAdo* deleteCommand = new SimpleAdo(AppConfigManager::Instance()->GetDBConnectionString("CollectionDB"));
	string query4 = StringUtil::Format("DELETE FROM DEVICE WHERE IP = '%s'", "192.168.10.4");
	deleteCommand->ExecuteCmd(query4);
	deleteCommand->DisConnect();


    cout << "Hello World!\n";
	AppConfigManager::ExitInstance();
}

