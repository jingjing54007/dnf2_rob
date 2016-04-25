/* -*- C++ -*- */
/*******************************************************************************
*	File			:	SapModel_RobPreAnalyse.c
*	Brief			:	分析玩家是否盗号
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
	result_analyse_init=0,			//初始化数据
	result_analyse_sap_done=1,			//业务受理分析 没有盗号现象
	result_analyse_sap_rob=2,			//业务受理分析 有盗号现象
	result_analyse_sap_error=3,		//业务受理分析 没有盗号现象，异常数据
	
	result_analyse_sap_double_error=8, //15天内重复受理分析有盗号的
	result_analyse_error=9, //其他异常数据,且不分析
};
CLog* RobLog;


// 异常类
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

// 跟进QQ，大区,角色ID,来获取角色相关信息
int GetRoleInfo(string& uin, string& area, string& strroleid, string& strname, string& strlevel, string& strvip )
{
	// 查询角色列表 - 获取角色ID
	RobLog->WriteLog("GetRoleInfo begin......\n");
	map<string, string> inParams;
	DataResult theResult;
	int iRoleExistFlag = 0;
	
	inParams.clear();
	inParams["area"] = area;
	inParams["uin"] = uin;
	if(""==strroleid||"0"==strroleid)
	{
		RobLog->WriteLog("获取角色信息查询失败,角色id=【%s】！\n",strroleid.c_str());
		return ERROR_500;
	}

	theResult.resultSet.clear();
	if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
			20320, inParams, theResult))
	{	
		RobLog->WriteLog("获取角色信息查询失败!\n");
		return ERROR_500;
	}
		
	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("获取角色信息查询返回失败!\n");
		return ERROR_500;
	}
	else if ( theResult.resultSet.size() <= 0 )
	{
		RobLog->WriteLog("获取角色信息查询数据失败!\n");
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
		RobLog->WriteLog("没找到玩家角色信息\n");
		return ERROR_500;
	}
 //获取vip信息
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
		RobLog->WriteLog("更新角色信息查询失败!\n");
		return ERROR_500;
	}
	
	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("更新角色信息返回失败!\n");
		return ERROR_500;
	}

	RobLog->WriteLog("SetRoleInfo end......\n");
	return ERROR_200;
}


///登录ip是否异常
int IsLoginExcept(string& area, string& uin, string& rolename, string& strendtime, string& strlasttime)
{
	map<string, string> inParams;
	DataResult theResult,theOperResult;
	//int iRobedCnt=0;

	/// 计算开始时间
	time_t tEndTime;
	GetTmsecFromUnxfmt(tEndTime, strendtime.c_str());
	tEndTime -= (60*60*24*15);
	char strstartTime[20] = {0};
	strftime(strstartTime, sizeof(strstartTime), "%Y-%m-%d %H:%M:%S", localtime(&tEndTime));
	
	/// 业务受理平台查询登入登出日志
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
		RobLog->WriteLog("获取登入登出查询失败:cmd[20303],errInfo[%s]\n", theResult.resultDesc.c_str());
		return ERROR_500;
	}
			
	if ( 0 != theResult.errCode )
	{
		RobLog->WriteLog("获取登入登出返回失败:cmd[20303],errInfo[%s],errcode[%d]\n", theResult.resultDesc.c_str(),theResult.errCode);
		return ERROR_500;
	}
	else if ( theResult.resultSet.size() <= 0 )
	{
		RobLog->WriteLog(  "获取登入登出数据失败:cmd[20303],errInfo[%s]\n", theResult.resultDesc.c_str());
		return ERROR_500;
	}
	
	map<string, int> mIpOnlineTime;
	string slogType = "1";
	mIpOnlineTime.clear();
	for ( int i = 0; i<theResult.resultSet.size(); i++ )
	{
		slogType = theResult.resultSet[i]["iAct"]; //0登入1登出
		string sLogIPName = IPLib::getAreaByIP(theResult.resultSet[i]["vLogIP"]);
		theResult.resultSet[i]["vClientIpName"] = sLogIPName;
		if("1"==slogType)
		{
			mIpOnlineTime[theResult.resultSet[i]["vClientIpName"]] += StrToInt(theResult.resultSet[i]["iPlayTime"]);
			RobLog->WriteLog( "登录地区:[%s]登录时长[%s]\n", sLogIPName.c_str(),theResult.resultSet[i]["iPlayTime"].c_str());
		}
	}
	if (mIpOnlineTime.size() > 6)
	{
		RobLog->WriteLog( "登录IP太多:[%d]\n", mIpOnlineTime.size());
		return ERROR_500;
	}
	else if(mIpOnlineTime.size()==1)
	{
		RobLog->WriteLog( "没有异常登录时间段:[%d]\n", mIpOnlineTime.size());
		return ERROR_500;
	}
	
	int flag = 0;	// 玩家正常登录的地区数

	for ( map<string, int>::iterator it = mIpOnlineTime.begin(); it != mIpOnlineTime.end(); it++ )
	{
		string sOnStr = "玩家登录地区[" + IntToStr(mIpOnlineTime.size()) + "]:";
		sOnStr = sOnStr + it->first + "[" + IntToStr(it->second) + "]";
		RobLog->WriteLog("%s\n",sOnStr.c_str());
		
		if ( it->second >= 60*60*2 )
		{
			flag++;
		}
	}
	
	if (flag == 0)
	{
		RobLog->WriteLog( "无异常登录IP\n");
		return ERROR_500;
	}
	
	//是否有物品流出
	for (int j = 0; j < theResult.resultSet.size(); j++ )
	{
		if("0"==theResult.resultSet[j]["iAct"])
		{
			continue;
		}
		RobLog->WriteLog( "登录地区【%s】时长【%s】\n",theResult.resultSet[j]["vClientIpName"].c_str(),theResult.resultSet[j]["iPlayTime"].c_str());
		if ( mIpOnlineTime[theResult.resultSet[j]["vClientIpName"]] < 60*60*2 )	// 登录时间小于2小时为异常登录
		{
			//theResult.resultSet[j]["iRobIpFlg"] = "1";

			string sLoginTime;			
			time_t tBeginTime;
			GetTmsecFromUnxfmt(tBeginTime,(theResult.resultSet[j]["dtDateTime"]).c_str());
			tBeginTime -= StrToInt(theResult.resultSet[j]["iPlayTime"]);
			GetUnxfmtFromTmsec(sLoginTime, tBeginTime);
			
			///业务受理平台是否有物品流出			
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
				RobLog->WriteLog(  "查询失败:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				continue;
			}
			
			if ( 0 != theOperResult.errCode )
			{
				RobLog->WriteLog("返回失败:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				continue;
			}
			
			if ( theOperResult.resultSet.size() <= 0 )
			{
				RobLog->WriteLog( "角色[%s]无物品流出:cmd[20321],errInfo[%s]\n",theResult.resultSet[j]["vCharacName"].c_str(), theOperResult.resultDesc.c_str());
				continue;
			}
			else
			{
				//发现最近一条异常登录日志有物品流出即返回。记录登出时间
				strlasttime = theResult.resultSet[j]["dtDateTime"];
				RobLog->WriteLog( "物品流出:cmd[20321],errInfo[%s]\n", theOperResult.resultDesc.c_str());
				return 0;
			}
			
		}

	}

	//没有物品流出
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
		//CLogicManager::PrintLog(QUERIERLOGPATH, "发送邮件: 长度{%d}, 内容{%s}", sCommond.size(), sCommond.c_str());
		RobLog->WriteLog("发送邮件: 长度{%d}, 内容{%s}\n", sCommond.size(), sCommond.c_str());
	}
	else
	{
		//CLogicManager::PrintLog(QUERIERLOGPATH, "发送邮件: 长度{%d}, 内容过多不显示", sCommond.size() );
		RobLog->WriteLog("发送邮件: 长度{%d}, 内容过多不显示\n", sCommond.size());
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
		RobLog->WriteLog("数据层[20325]result_analyse_Have_rob查询失败!\n");
		return ERROR_500;
	}

	if(0 != theResult.errCode)
	{ 
		RobLog->WriteLog("数据层[20325]result_analyse_Have_rob返回失败!\n");
		return ERROR_500;
	}
	
	if(theResult.resultSet.size() == 0)
	{
		RobLog->WriteLog("数据层[20325]没有重复来单!\n");
	}
	else
	{
		RobLog->WriteLog("数据层[20325]有重复来单!\n");
		return ERROR_200;
	}
	
	return OK;
	
}


/*
	result_analyse_init=0,			//初始化数据
	result_analyse_sap_done=1,			//业务受理分析 没有盗号现象
	result_analyse_sap_rob=2,			//业务受理分析 有盗号现象
	result_analyse_sap_error=3, 	//业务受理分析 没有盗号现象，异常数据
		
	result_analyse_error=9, //其他异常数据,且不分析

*/


int main(int argc, char** argv)
{
	//调试日志
	string sLog = "./logs/"+GetCurDateDay()+"SapModel_RobPreAnalyse.log";
	RobLog = new CLog((char*)sLog.c_str());
	RobLog->LogOn();
	RobLog->WriteLog("===============Start GetRobItemFlowList============\n");
	///变量声明
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
	string mailTitle = "盗号预流失分析工具测试邮件";
	string mailText = "内容:\n";
	string destFile = "/usr/local/games/robgame/bin/dnf2/logs/"+GetCurDateDay()+"RobPreAnalyse.log";
	
	
	//避开了0206
	CDBManager::GetInstance().sEndPointUrl = CRobConf::getConf()->GetCommonInterface()["dbserver"];
	
	try
	{
		// 获取业务受理分析数据
		inParams.clear();
		inParams["status"] = IntToStr((int)result_analyse_init);
		inParams["size"] = "100";

		theResult.resultSet.clear();
		if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
			20299, inParams, theResult))
		{	
			RobLog->WriteLog("数据层[20299]查询失败!\n");
			throw Except(20299, string("数据层[20299]查询失败!"));
		}

		if(0 != theResult.errCode)
		{ 
			RobLog->WriteLog("数据层[20299]返回失败!\n");
			throw Except(20299, string("数据层[20299]返回失败!"));
		}
		else if ( theResult.resultSet.size() <= 0 )
		{
			RobLog->WriteLog("数据层[20299]没有需要处理的单据size=%d\n",theResult.resultSet.size());
			throw Except(20299, string("数据层[20299]没有需要处理的单据!"));
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
			RobLog->WriteLog("-----------------------------------------第【%d】记录处理中-------------------------------------------\n", i);
			RobLog->WriteLog("数据信息 strICS[%s] id[%s] uin[%s] roleid[%s] rolename[%s] area[%s] servid[%s] level[%s] vip[%s]\n",
				strICS.c_str(),strId.c_str(),uin.c_str(),strroleid.c_str(),strrolename.c_str(), area.c_str(),servid.c_str(),strlevel.c_str(),strvip.c_str());
			// 1判断重复单据
			ret = Is15DuplicateRecord(area, uin, strroleid);
			if(ret == ERROR_200)
			{		
				RobLog->WriteLog("15天内重复分析\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_double_error);//"8"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("1更新状态失败\n");
				}
				RobLog->WriteLog("1更新状态成功\n");
				continue;
			}		
			// 2获取角色信息
			/*if (0 != GetRoleInfo(uin, area, strroleid, strrolename, strlevel, strvip))
			{
				RobLog->WriteLog("获取角色信息失败\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_error);//"3"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("2更新状态失败\n");
				}
				
				RobLog->WriteLog("2更新状态成功\n");
							
				continue;	
			}
			RobLog->WriteLog("数据信息 roleid[%s], rolename[%s]\n", strroleid.c_str(), strrolename.c_str());
			// 更新表角色ID，角色等级，VIP信息
			if (ERROR_200 != SetRoleInfo(strId, strroleid, strlevel, strvip))
			{
				RobLog->WriteLog("更新角色信息失败\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_error);//"3"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("3更新状态[11]失败\n");
				}
				RobLog->WriteLog("3更新状态成功\n");		
				continue;	
			}*/
			
			//3是否异常ip，是否物品流出
			string strLastTime = "";
			ret = IsLoginExcept(area, uin, strrolename, theResult.resultSet[i]["dtBookTime"], strLastTime);
			if(ret == 0)
			{	
				// 有异常盗号，先更新数据库状态为，状态值为2
				RobLog->WriteLog("有异常盗号发生\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_rob);//"2"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("IsLoginExcept 更新状态[2]失败\n");
				}
				RobLog->WriteLog("IsLoginExcept 更新状态[2]成功\n");
				
				//发送tips
				//cmd=26100&business=xx&uin=xx&msg_id=xx&verify_code=xx&msg_type=xx&from_uin=xx&temp_id=xx&data=xx,xx|xx,xx...
				inParams.clear();
				string text = "预流失分析工具";
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
					RobLog->WriteLog("cmd[20296]调用tips接口失败\n");
				}
				else
				{
					RobLog->WriteLog("cmd[20296]调用tips接口成功\n");
					RobLog->WriteLog("errcode[%d]\n",theOperResult.errCode);
					if(theOperResult.resultSet.size()>0)
					{
						RobLog->WriteLog("cmd[20296]发送tips成功\n");
					}
					else
					{
						RobLog->WriteLog("cmd[20296]发送tips失败\n");
					}
				}
				
			}
			else
			{
				//更新单据状态为未发现盗号，状态值为1	
				RobLog->WriteLog("无异常盗号发生\n");
				inParams.clear();
				inParams["status"] = IntToStr((int)result_analyse_sap_done);//"1"; 
				inParams["id"] = theResult.resultSet[i]["iID"];
				DataResult UpdatetheResult;
				if(false == CDBManager::GetInstance().ExeQuerier(ics_log_header("","",CRobConf::getConf()->GetGroupInfo()["group_id"]),
				20300, inParams, UpdatetheResult))
				{
					RobLog->WriteLog("3更新状态[1]失败\n");
				}
				
				RobLog->WriteLog("3更新状态[1]成功\n");
			
			}
		}
	}
	catch (Except ex)
	{
		RobLog->WriteLog("业务受理分析异常:%d,%s\n", ex.GetCode(), ex.GetInfo().c_str());
	}
	catch (...)
	{
		RobLog->WriteLog("业务受理分析异常[未知错误].\n");
	}

	RobLog->WriteLog("盗号分析处理成功\n");

	//ret = SendMail( mailTo, mailTitle, mailText, destFile );
	//if(ret != OK)
	//{
	//	RobLog->WriteLog("发送邮件失败,返回码[%d]\n", ret);
	//}
	delete RobLog;
	RobLog=NULL;
	return 0;
}

