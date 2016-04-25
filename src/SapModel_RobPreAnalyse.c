/* -*- C++ -*- */
/*******************************************************************************
*	File			:	SapModel_RobPreAnalyse.c
*	Brief			:	��������Ƿ����
*	Version		:	1.0.0
*	Author		:	v_wxywu
*	Datetime		:	2014-05-19
*	input		:	
*	output		:
*******************************************************************************/

/*
	g++
	g++ -w SapModel_RobAnalyse.c  -o SapModel_RobAnalyse -I. -I../include -I../include/libxml2  -I../include/libxml2/libxml  -I../include/wsclient  -I../include/wsclient/logmanager  -I../include/wsclient/dbmanager  -I../include/curl -include sap.nsmap -I../include -I../comm -I../include/network -L../lib -lpthread -ldl -lutil -lrt -lxerces-c -ltds -lcurl -lAutoRob -lAltovaXML -lAltova -lsap -lsap_func -ldata_interface  -lWSLogManager -lWSDBManager -L../lib -lossbasenetwork -lAutoRob -ltsf4g  -g -Dylzt
*/
#include <iostream>
#include "comm_datetime.h"
#include "comm_dbmanager.h"
#include "rob_rule_logic.h"
#include "comm_value.h"
#include "iplib.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"

#define QUERIERLOGPATH "./logs/SapModel_RobPreAnalyse.log"

#define OK 0
#define ERROR_500 500
#define ERROR_100 100
#define ERROR_200 200

enum result_analyse
{
	result_analyse_init=0,			//��ʼ������
	result_analyse_sap_done=1,			//ҵ��������� û�е�������
	result_analyse_sap_rob=2,			//ҵ��������� �е�������
	result_analyse_sap_error=3,		//ҵ��������� û�е��������쳣����
	
	result_analyse_sap_double_error=8, //15�����ظ���������е��ŵ�
	result_analyse_error=9, //�����쳣����,�Ҳ�����
};
CLog* RobLog;


// �쳣��
class Except
{
public:
	Except()
	{
		errCode = 0;
		errInfo = "";
	}
	Except(const Except &other)
	{
		errCode = other.errCode;
		errInfo = other.errInfo;
	}
	Except(int code, const std::string &info)
	{
		errCode = code;
		errInfo = info;
	}
	Except(int code, char *info)
	{
		errCode = code;
		errInfo = info;
	}
	~Except(){}
	
	Except &operator=(const Except &other)
	{
		if ( this != &other )
		{
			errCode = other.errCode;
			errInfo = other.errInfo;
		}
		return *this;
	}
	std::string What()
	{
		return errInfo;
	}
	int GetCode()
	{
		return errCode;
	}
	string GetInfo()
	{
		return errInfo;
	}
public:
	int errCode;
	string errInfo;
};

// ����QQ������,��ɫID,����ȡ��ɫ�����Ϣ
int GetRoleInfo(string& uin, string& area, string& strroleid, string& strname, string& strlevel, string& strvip )
{
	// ��ѯ��ɫ�б� - ��ȡ��ɫID
	RobLog->WriteLog("GetRoleInfo begin......\n");
	map<string, string> inParams;
	DataResult theResult;
	int iRoleExistFlag = 0;
	
	inParams.clear();
	inParams["area"] = area;
	inParams["uin"] = uin;
	if(""==strroleid||"0"==strroleid)
	{
		RobLog->WriteLog("��ȡ��ɫ��Ϣ��ѯʧ��,��ɫid=��%s����\n",strroleid.c_str());
		return ERROR_500;
	}

	theResult.resultSet.clear();
	if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
			20320, inParams, theResult))
	{	
		RobLog->WriteLog("��ȡ��ɫ��Ϣ��ѯʧ��!\n");
		return ERROR_500;
	}
		
	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("��ȡ��ɫ��Ϣ��ѯ����ʧ��!\n");
		return ERROR_500;
	}
	else if ( theResult.resultSet.size() <= 0 )
	{
		RobLog->WriteLog("��ȡ��ɫ��Ϣ��ѯ����ʧ��!\n");
		return ERROR_500;
	}

	int len = theResult.resultSet.size();
	for(int i = 0;i<len;i++)
	{
		if(theResult.resultSet[i]["name_0"]==strroleid)
		{
			strname = theResult.resultSet[i]["name_1"];
			strlevel = theResult.resultSet[i]["name_3"];
			iRoleExistFlag = 1;
			RobLog->WriteLog("[INFO] role_name=%s&role_id=%s\n",strname.c_str(),strroleid.c_str());
		}
	}
	if(iRoleExistFlag==0){
		RobLog->WriteLog("û�ҵ���ҽ�ɫ��Ϣ\n");
		return ERROR_500;
	}
 //��ȡvip��Ϣ
  IPLib ipInterface;
 	int vip_level = ipInterface.getVipByUin(uin);
 	RobLog->WriteLog("[INFO]: vip_level=%d\n",vip_level);
  strvip = IntToStr(vip_level);
	RobLog->WriteLog("GetRoleInfo end......\n");
	return 0;
}

int SetRoleInfo(string id, string& roleid,string& rolelevel, string& vip)
{	
	RobLog->WriteLog("SetRoleInfo begin......\n");
	map<string, string> inParams;
	DataResult theResult;

	inParams["id"] = id; 
	inParams["roleid"] =roleid; 
	inParams["rolelevel"] =rolelevel; 
	inParams["vip"] =vip; 
	
	if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
		20326, inParams, theResult))
	{	
		RobLog->WriteLog("���½�ɫ��Ϣ��ѯʧ��!\n");
		return ERROR_500;
	}
	
	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("���½�ɫ��Ϣ����ʧ��!\n");
		return ERROR_500;
	}

	RobLog->WriteLog("SetRoleInfo end......\n");
	return ERROR_200;
}


///��¼ip�Ƿ��쳣
int IsLoginExcept(string& area, string& uin, string& rolename, string& strendtime, string& strlasttime)
{
	map<string, string> inParams;
	DataResult theResult,theOperResult;
	//int iRobedCnt=0;

	/// ���㿪ʼʱ��
	time_t tEndTime;
	GetTmsecFromUnxfmt(tEndTime, strendtime.c_str());
	tEndTime -= (60*60*24*15);
	char strstartTime[20] = {0};
	strftime(strstartTime, sizeof(strstartTime), "%Y-%m-%d %H:%M:%S", localtime(&tEndTime));
	
	/// ҵ������ƽ̨��ѯ����ǳ���־
	inParams.clear();
	inParams["area"] = area; 
	inParams["uin"] = uin;
	inParams["gid"] = string(GbkToUtf8((char*)rolename.c_str()));
	inParams["start_time"] = strstartTime;
	inParams["end_time"] = strendtime;

	RobLog->WriteLog("cmd[20303],area[%s],uin[%s],gid[%s],strstartTime[%s],strendtime[%s]\n", \
					area.c_str(), uin.c_str(), rolename.c_str(), strstartTime, strendtime.c_str());
		RobLog->WriteLog("cmd[20303],area[%s],uin[%s],gid[%s],strstartTime[%s],strendtime[%s]\n", \
					inParams["area"].c_str(), inParams["uin"].c_str(), inParams["gid"].c_str(), strstartTime, strendtime.c_str());
	theResult.resultSet.clear();
	if ( false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),20303, inParams, theResult) )
	{
		RobLog->WriteLog("��ȡ����ǳ���ѯʧ��:cmd[20303],errInfo[%s]\n", theResult.resultDesc.c_str());
		return ERROR_500;
	}
			
	if ( 0 != theResult.errCode )
	{
		RobLog->WriteLog("��ȡ����ǳ�����ʧ��:cmd[20303],errInfo[%s],errcode[%d]\n", theResult.resultDesc.c_str(),theResult.errCode);
		return ERROR_500;
	}
	else if ( theResult.resultSet.size() <= 0 )
	{
		RobLog->WriteLog(  "��ȡ����ǳ�����ʧ��:cmd[20303],errInfo[%s]\n", theResult.resultDesc.c_str());
		return ERROR_500;
	}
	
	map<string, int> mIpOnlineTime;
	string slogType = "1";
	mIpOnlineTime.clear();
	for ( int i = 0; i<theResult.resultSet.size(); i++ )
	{
		slogType = theResult.resultSet[i]["iAct"]; //0����1�ǳ�
		string sLogIPName = IPLib::getAreaByIP(theResult.resultSet[i]["vLogIP"]);
		theResult.resultSet[i]["vClientIpName"] = sLogIPName;
		if("1"==slogType)
		{
			mIpOnlineTime[theResult.resultSet[i]["vClientIpName"]] += StrToInt(theResult.resultSet[i]["iPlayTime"]);
			RobLog->WriteLog( "��¼����:[%s]��¼ʱ��[%s]\n", sLogIPName.c_str(),theResult.resultSet[i]["iPlayTime"].c_str());
		}
	}
	if (mIpOnlineTime.size() > 6)
	{
		RobLog->WriteLog( "��¼IP̫��:[%d]\n", mIpOnlineTime.size());
		return ERROR_500;
	}
	else if(mIpOnlineTime.size()==1)
	{
		RobLog->WriteLog( "û���쳣��¼ʱ���:[%d]\n", mIpOnlineTime.size());
		return ERROR_500;
	}
	
	int flag = 0;	// ���������¼�ĵ�����

	for ( map<string, int>::iterator it = mIpOnlineTime.begin(); it != mIpOnlineTime.end(); it++ )
	{
		string sOnStr = "��ҵ�¼����[" + IntToStr(mIpOnlineTime.size()) + "]:";
		sOnStr = sOnStr + it->first + "[" + IntToStr(it->second) + "]";
		RobLog->WriteLog("%s\n",sOnStr.c_str());
		
		if ( it->second >= 60*60*2 )
		{
			flag++;
		}
	}
	
	if (flag == 0)
	{
		RobLog->WriteLog( "���쳣��¼IP\n");
		return ERROR_500;
	}
	
	//�Ƿ�����Ʒ����
	for (int j = 0; j < theResult.resultSet.size(); j++ )
	{
		if("0"==theResult.resultSet[j]["iAct"])
		{
			continue;
		}
		RobLog->WriteLog( "��¼������%s��ʱ����%s��\n",theResult.resultSet[j]["vClientIpName"].c_str(),theResult.resultSet[j]["iPlayTime"].c_str());
		if ( mIpOnlineTime[theResult.resultSet[j]["vClientIpName"]] < 60*60*2 )	// ��¼ʱ��С��2СʱΪ�쳣��¼
		{
			//theResult.resultSet[j]["iRobIpFlg"] = "1";

			string sLoginTime;			
			time_t tBeginTime;
			GetTmsecFromUnxfmt(tBeginTime,(theResult.resultSet[j]["dtDateTime"]).c_str());
			tBeginTime -= StrToInt(theResult.resultSet[j]["iPlayTime"]);
			GetUnxfmtFromTmsec(sLoginTime, tBeginTime);
			
			///ҵ������ƽ̨�Ƿ�����Ʒ����			
			inParams.clear();
			inParams["area"] = area; 
			inParams["uin"] = uin;
			inParams["gid"] = string(GbkToUtf8((char*)theResult.resultSet[j]["vCharacName"].c_str()));
			inParams["start_time"] = sLoginTime;
			inParams["end_time"] = theResult.resultSet[j]["dtDateTime"];
			
			RobLog->WriteLog("cmd[20321],area[%s],uin[%s],gid[%s],start_time[%s],end_time[%s]\n", \
					area.c_str(), uin.c_str(), theResult.resultSet[j]["vCharacName"].c_str(),sLoginTime.c_str(), theResult.resultSet[j]["dtDateTime"].c_str());
			theOperResult.resultSet.clear();
			if ( false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),20321, inParams, theOperResult) )
			{
				RobLog->WriteLog(  "��ѯʧ��:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				continue;
			}
			
			if ( 0 != theOperResult.errCode )
			{
				RobLog->WriteLog("����ʧ��:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				continue;
			}
			
			if ( theOperResult.resultSet.size() <= 0 )
			{
				RobLog->WriteLog( "��ɫ[%s]����Ʒ����:cmd[20321],errInfo[%s]\n",theResult.resultSet[j]["vCharacName"].c_str(), theOperResult.resultDesc.c_str());
				continue;
			}
			else
			{
				//�������һ���쳣��¼��־����Ʒ���������ء���¼�ǳ�ʱ��
				strlasttime = theResult.resultSet[j]["dtDateTime"];
				RobLog->WriteLog( "��Ʒ����:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				return 0;
			}
			
		}

	}

	//û����Ʒ����
	return ERROR_500;
			
}



GLog OptObj;


int SendMail( const string &mailTo, const string &mailTitle, const string &mailText, const string &fileName = "" )
{
	if ( 0 == mailTo.size() )
	{
		return -1;
	}
	string sCommond("");
	sCommond = sCommond + "nohup /usr/local/games/sapws/script/mail/sendmail.pl -r \"" + mailTo + "\" -s \"" + mailTitle + "\" -n \"" + mailText + "\" ";
	if ( 0 != fileName.size() )
	{
		sCommond = sCommond + " -i \"" + fileName + "\" ";
	}
	sCommond += "&";
	if ( sCommond.size() < 3*1024 )
	{
		//CLogicManager::PrintLog(QUERIERLOGPATH, "�����ʼ�: ����{%d}, ����{%s}", sCommond.size(), sCommond.c_str());
		RobLog->WriteLog("�����ʼ�: ����{%d}, ����{%s}\n", sCommond.size(), sCommond.c_str());
	}
	else
	{
		//CLogicManager::PrintLog(QUERIERLOGPATH, "�����ʼ�: ����{%d}, ���ݹ��಻��ʾ", sCommond.size() );
		RobLog->WriteLog("�����ʼ�: ����{%d}, ���ݹ��಻��ʾ\n", sCommond.size());
	}
	system(sCommond.c_str());
	return 0;
	
}


int Is15DuplicateRecord(string& area, string& uin, string& roleid)
{
	map<string, string> inParams;
	DataResult theResult;
	
	inParams.clear();
	inParams["roleid"] = roleid; 
	inParams["area"] = area;
	inParams["uin"] = uin;
	
	theResult.resultSet.clear();
	if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
		20325, inParams, theResult))
	{
		RobLog->WriteLog("���ݲ�[20325]result_analyse_Have_rob��ѯʧ��!\n");
		return ERROR_500;
	}

	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("���ݲ�[20325]result_analyse_Have_rob����ʧ��!\n");
		return ERROR_500;
	}
	
	if(theResult.resultSet.size() == 0)
	{
		RobLog->WriteLog("���ݲ�[20325]û���ظ�����!\n");
	}
	else
	{
		RobLog->WriteLog("���ݲ�[20325]���ظ�����!\n");
		return ERROR_200;
	}
	
	return OK;
	
}


/*
	result_analyse_init=0,			//��ʼ������
	result_analyse_sap_done=1,			//ҵ��������� û�е�������
	result_analyse_sap_rob=2,			//ҵ��������� �е�������
	result_analyse_sap_error=3, 	//ҵ��������� û�е��������쳣����
		
	result_analyse_error=9, //�����쳣����,�Ҳ�����

*/


int main(int argc, char** argv)
{
	//������־
	string sLog = "./logs/"+GetCurDateDay()+"SapModel_RobPreAnalyse.log";
	RobLog = new CLog((char*)sLog.c_str());
	RobLog->LogOn();
	RobLog->WriteLog("===============Start GetRobItemFlowList============\n");
	///��������
	int j = 0;
	int ret = 0, len = 0;
	map<string, string> inParams;
	DataResult theResult, theOperResult;
	string servid;
	string uin;
	string roleid;	
	string strvip;
	string area;
	string strroleid = "";
	string strrolename = "";
	string strlevel;
	string strICS;
	string strId;

	string mailTo = "v_wxywu;";
	string mailTitle = "����Ԥ��ʧ�������߲����ʼ�";
	string mailText = "����:\n";
	string destFile = "/usr/local/games/robgame/bin/dnf2/logs/"+GetCurDateDay()+"RobPreAnalyse.log";
	
	
	//�ܿ���0206
	CDBManager::GetInstance().sEndPointUrl = CRobConf::getConf()->GetCommonInterface()["dbserver"];
	
	try
	{
		// ��ȡҵ�������������
		inParams.clear();
		inParams["status"] = IntToStr((int)result_analyse_init);
		inParams["size"] = "100";

		theResult.resultSet.clear();
		if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
			20299, inParams, theResult))
		{	
			RobLog->WriteLog("���ݲ�[20299]��ѯʧ��!\n");
			throw Except(20299, string("���ݲ�[20299]��ѯʧ��!"));
		}

		if(0 != theResult.errCode)
		{ 
			RobLog->WriteLog("���ݲ�[20299]����ʧ��!\n");
			throw Except(20299, string("���ݲ�[20299]����ʧ��!"));
		}
		else if ( theResult.resultSet.size() <= 0 )
		{
			RobLog->WriteLog("���ݲ�[20299]û����Ҫ����ĵ���size=%d\n",theResult.resultSet.size());
			throw Except(20299, string("���ݲ�[20299]û����Ҫ����ĵ���!"));
		}
		
		len = theResult.resultSet.size();
		
		for ( int i = 0; i < len; i++ )
		{
			strICS = theResult.resultSet[i]["iIcsID"];
			strId = theResult.resultSet[i]["iID"];
			strroleid = theResult.resultSet[i]["vRobed_RoleId"];
			strrolename = theResult.resultSet[i]["vRobed_RoleName"];
			uin = theResult.resultSet[i]["iRobed_Uin"];
			area = theResult.resultSet[i]["iWordId"];
			servid = theResult.resultSet[i]["iServiceID"];  //148
			
			strlevel = theResult.resultSet[i]["iRobed_RoleLevel"];
			strvip = theResult.resultSet[i]["iVip"];
			RobLog->WriteLog("-----------------------------------------�ڡ�%d����¼������-------------------------------------------\n", i);
			RobLog->WriteLog("������Ϣ strICS[%s] id[%s] uin[%s] roleid[%s] rolename[%s] area[%s] servid[%s] level[%s] vip[%s]\n",
				strICS.c_str(),strId.c_str(),uin.c_str(),strroleid.c_str(),strrolename.c_str(), area.c_str(),servid.c_str(),strlevel.c_str(),strvip.c_str());
			// 1�ж��ظ�����
			ret = Is15DuplicateRecord(area, uin, strroleid);
			if(ret == ERROR_200)
			{		
				RobLog->WriteLog("15�����ظ�����\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_double_error);//"8"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("1����״̬ʧ��\n");
				}
				RobLog->WriteLog("1����״̬�ɹ�\n");
				continue;
			}		
			// 2��ȡ��ɫ��Ϣ
			/*if (0 != GetRoleInfo(uin, area, strroleid, strrolename, strlevel, strvip))
			{
				RobLog->WriteLog("��ȡ��ɫ��Ϣʧ��\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_error);//"3"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("2����״̬ʧ��\n");
				}
				
				RobLog->WriteLog("2����״̬�ɹ�\n");
							
				continue;	
			}
			RobLog->WriteLog("������Ϣ roleid[%s], rolename[%s]\n", strroleid.c_str(), strrolename.c_str());
			// ���±��ɫID����ɫ�ȼ���VIP��Ϣ
			if (ERROR_200 != SetRoleInfo(strId, strroleid, strlevel, strvip))
			{
				RobLog->WriteLog("���½�ɫ��Ϣʧ��\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_error);//"3"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("3����״̬[11]ʧ��\n");
				}
				RobLog->WriteLog("3����״̬�ɹ�\n");		
				continue;	
			}*/
			
			//3�Ƿ��쳣ip���Ƿ���Ʒ����
			string strLastTime = "";
			ret = IsLoginExcept(area, uin, strrolename, theResult.resultSet[i]["dtBookTime"], strLastTime);
			if(ret == 0)
			{	
				// ���쳣���ţ��ȸ������ݿ�״̬Ϊ��״ֵ̬Ϊ2
				RobLog->WriteLog("���쳣���ŷ���\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_rob);//"2"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("IsLoginExcept ����״̬[2]ʧ��\n");
				}
				RobLog->WriteLog("IsLoginExcept ����״̬[2]�ɹ�\n");
				
				//����tips
				//cmd=26100&business=xx&uin=xx&msg_id=xx&verify_code=xx&msg_type=xx&from_uin=xx&temp_id=xx&data=xx,xx|xx,xx...
				inParams.clear();
				string text = "Ԥ��ʧ��������";
				inParams["business"] = string(GbkToUtf8((char*)text.c_str()));
				inParams["uin"] = "632879551";//uin;
				inParams["verify_code"] = "CFAA";
				inParams["msg_type"] = "0x4c";
				inParams["msg_id"] = "134456";
				inParams["NickName"] = string(GbkToUtf8((char*)strrolename.c_str()));
				inParams["Date"] = string(GbkToUtf8((char*)strLastTime.c_str()));
				DataResult theOperResult;
				RobLog->WriteLog("cmd[20296],business[%s],uin[%s],verify_code[%s],msg_type[%s],msg_id[%s],NickName[%s],Date[%s]\n", \
					inParams["business"].c_str(),uin.c_str(),inParams["verify_code"].c_str(),inParams["msg_type"].c_str(),inParams["msg_id"].c_str(),inParams["NickName"].c_str(),inParams["Date"].c_str());
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20296, inParams, theOperResult))
				{
					RobLog->WriteLog("cmd[20296]����tips�ӿ�ʧ��\n");
				}
				else
				{
					RobLog->WriteLog("cmd[20296]����tips�ӿڳɹ�\n");
					RobLog->WriteLog("errcode[%d]\n",theOperResult.errCode);
					if(theOperResult.resultSet.size()>0)
					{
						RobLog->WriteLog("cmd[20296]����tips�ɹ�\n");
					}
					else
					{
						RobLog->WriteLog("cmd[20296]����tipsʧ��\n");
					}
				}
				
			}
			else
			{
				//���µ���״̬Ϊδ���ֵ��ţ�״ֵ̬Ϊ1	
				RobLog->WriteLog("���쳣���ŷ���\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_done);//"1"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("3����״̬[1]ʧ��\n");
				}
				
				RobLog->WriteLog("3����״̬[1]�ɹ�\n");
			
			}
		}
	}
	catch (Except ex)
	{
		RobLog->WriteLog("ҵ����������쳣:%d,%s\n", ex.GetCode(), ex.GetInfo().c_str());
	}
	catch (...)
	{
		RobLog->WriteLog("ҵ����������쳣[δ֪����].\n");
	}

	RobLog->WriteLog("���ŷ�������ɹ�\n");

	//ret = SendMail( mailTo, mailTitle, mailText, destFile );
	//if(ret != OK)
	//{
	//	RobLog->WriteLog("�����ʼ�ʧ��,������[%d]\n", ret);
	//}
	delete RobLog;
	RobLog=NULL;
	return 0;
}

