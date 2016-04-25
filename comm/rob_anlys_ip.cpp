/**
@file rob_anlys_ip.cpp

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		GunLi
@version	1.0
@date		2010/10/12

history:
<PRE>
History ID	:	1
Author    	:	asherzhou
DateTime	:	2011/05/07
Description :	Alert
</PRE>
*/

#include <iostream>

#include "rob_anlys_ip.h"
#include "iplib.h"
#include "comm_datetime.h"
#include "comm_strings.h"
#include "comm_value.h"

/**
Description:	构造函数
@param	[in]	none
@return			bool
@exception		none
*/
RobAnlysIP::RobAnlysIP()
{
	sLOGINOUTLOG ="./logs/"+GetCurDateDay()+"RobAnlysIP.log";
	LogInOutLOG = new CLog((const char*)sLOGINOUTLOG.c_str());
	LogInOutLOG->LogOn();
}
/**
Description:	析构函数
@param	[in]	none
@return			bool
@exception		none
*/
RobAnlysIP::~RobAnlysIP()
{
	if(NULL!=LogInOutLOG)
	{
		delete LogInOutLOG;
	}
}
/**
Description:	输入参数合法性验证
@param	[in]	begin
@param	[in]	end
@return			bool
@exception		none
*/
bool RobAnlysIP::checkValid(time_t begin, time_t end)
{
	if (begin>=end)
	{
		LogInOutLOG->WriteLog("Check LogIn and LogOut Time Value is Invalid\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Check LogIn and LogOut Time Value is Invalid\n");

		return false;
	}
	else{
		LogInOutLOG->WriteLog("Check LogIn and LogOut Time Value is Valid\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Check LogIn and LogOut Time Value is Valid\n");

		return true;
	}
}
/*
判断盗号时间规则，两者时间相差不到12小时，则为正常，返回true，否则返回false
*/
bool RobAnlysIP::checkRobedTime(time_t t_lastlogin, time_t t_robed, string logouttime)
{
	time_t t_logout;

	GetTmsecFromUnxfmt(t_logout,(logouttime).c_str());

	if (t_logout<t_robed && t_logout>t_lastlogin)
	{
		cout << logouttime << "need" << endl;
		return true;
	}
	else
	{
		cout << logouttime << "no" << endl;
		return false;
	}

}
/**
Description:	游戏登陆日志分析逻辑
@param	[in]	string
@param	[in]	in_data
@param	[in]	string
@param	[in]	err_area
@return			void
@exception		none
*/
int RobAnlysIP::anaylseData(Rob_Ics_Complaint& Ics_Loginout, std::vector< map<string,string> > &LogInOutData, vector< Rob_SLOT > &PutOutData)
{
	LogInOutLOG->WriteLog("************In getRobLogInOutLog.anaylseData() Proccess! Begin**************\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "************In getRobLogInOutLog.anaylseData() Proccess! Begin**************\n");

	//用于记录在不同的地区登陆的总的时长
	map <string,unsigned int> mAreaTime;			//记录登录地区的总统计时间
	map <string,unsigned int> mAreaCount;			//记录登录地区的次数
	map <string,unsigned int> mAreaDiffentCount;	//记录登录不同地区的次数
	map <string,string> mIPtoArea;					//记录登录IP对应的区域数
	PutOutData.clear();


	//记录上一条记录中的ip对应的地区
	string sLastArea = "0";
	string sLogIp = "0.0.0.0";
	string sLogIPName = "广东省.深圳市";
	string sIpFlag = "vIP";
	string sLoginTime = "0";
	string sLogoutTime = "0";
	string sLoginTimeFlag = "dtLoginTime";
	string sLogoutTimeFlag = "dtLogoutTime";
	string sLogDuration = "0";
	string sLogDurationFlag = "iOnlineTime";
	string slogType = "1";
	int iRobFlag = 0;
	unsigned int from_id = 0;
	unsigned int end_id = LogInOutData.size();



#ifdef dnf2
		sIpFlag = "vLogIP";
		sLoginTimeFlag = "dtDateTime";
		sLogoutTimeFlag = "dtDateTime";
		slogType = LogInOutData[0]["iAct"];
		
		LogInOutLOG->WriteLog("a.分析登录日志范围：slogType=%s\n",slogType.c_str());
		if(slogType=="0"){//登入
			if(LogInOutData.size()%2==0){
				from_id = 1;
				end_id = LogInOutData.size()-1;
			}else{
				from_id = 1;
				end_id = LogInOutData.size();
			}
		}else if(slogType=="1"){//登出
			if(LogInOutData.size()%2==0){
				from_id = 0;
				end_id = LogInOutData.size();
			}else{
				from_id = 0;//modified by v_zhtang on 2013-05-23;与手动一致，修复登出与登入不匹配的bug
				end_id = LogInOutData.size()-2;//
			}
		}else{
			LogInOutLOG->WriteLog("In anaylseData() DNF2 process get iAct type error \n");
		}
		LogInOutLOG->WriteLog("b.分析登录日志范围：slogType=%s&allsize=%d&from_id=%d&end_id=%d\n",slogType.c_str(),LogInOutData.size(),from_id,end_id);
#endif
	LogInOutLOG->WriteLog("@@@Begin anaylseData() Traversal Logic Process@@@\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "@@@Begin anaylseData() Traversal Logic Process@@@\n");

	//首次遍历，IP转化成地域值，同时计算出在同一地区登陆的总时长
	for(unsigned int i = from_id; i < end_id; i++)
	{
		sLogIp = LogInOutData[i][(char*)sIpFlag.c_str()];
		sLoginTime = LogInOutData[i][(char*)sLoginTimeFlag.c_str()];
		sLogoutTime = LogInOutData[i][(char*)sLogoutTimeFlag.c_str()];
#ifdef dnf2
		slogType = LogInOutData[i]["iAct"];
#endif
		//根据IP获取地区信息
/*		if(mIPtoArea[(char*)sLogIp.c_str()]==""){
			if(IPLib::getAreaByIP(sLogIp)!=""){
				mIPtoArea[(char*)sLogIp.c_str()]= IPLib::getAreaByIP(sLogIp);
			}else{
				mIPtoArea[(char*)sLogIp.c_str()]= sLogIp;
				return STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_ANALYSIS;
			}
		}
		sLogIPName = mIPtoArea[(char*)sLogIp.c_str()];
*/		
		//LogInOutLOG->WriteLog("1.<登录和登出的日志ID>i=%d&from_id=%d&end_id=%d&slogType=%s\n",i,from_id,end_id,slogType.c_str());
#ifdef dnf2
		//（增加额外程序）
		//因为dnf2的登录登出日志的分开的，所以需要进行合并
		//合并程序
		//1 0 1 0 1 0结构--!
		if((i-from_id)%2==0){
			continue;
		}else{
			LogInOutLOG->WriteLog("---1.a i=%d\n",i);
			//根据IP获取地区信息
			if(mIPtoArea.find((char*)sLogIp.c_str())== mIPtoArea.end()){
				string area=IPLib::getAreaByIP(sLogIp);
				Trim(area);
				if(area!=""&&area!="未知IP"){
					mIPtoArea[(char*)sLogIp.c_str()]= IPLib::getAreaByIP(sLogIp);
				}else{
					//mIPtoArea[(char*)sLogIp.c_str()]= sLogIp;
					//modify by asherzhou 2012-05-14
					mIPtoArea[(char*)sLogIp.c_str()]="Unkown Area";
					//return STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_ANALYSIS;
				}
			}
			sLogIPName = mIPtoArea[(char*)sLogIp.c_str()];
			sLoginTime = LogInOutData[i][(char*)sLoginTimeFlag.c_str()];
			sLogoutTime = LogInOutData[i-1][(char*)sLoginTimeFlag.c_str()];
			time_t Login_begin;
			time_t Login_end;
			GetTmsecFromUnxfmt(Login_begin,(sLoginTime).c_str());
			GetTmsecFromUnxfmt(Login_end,(sLogoutTime).c_str());
			int diffTime = (int)(Login_end-Login_begin);
			sLogDuration = 	IntToStr(diffTime);
			LogInOutLOG->WriteLog("2.<登入时间、登出时间以及持续时间>sLogDuration=%s&sLoginTime=%s&sLogoutTime=%s\n",sLogDuration.c_str(),sLoginTime.c_str(),sLogoutTime.c_str());
			if(mAreaTime.find((char*)sLogIPName.c_str())== mAreaTime.end()){
				mAreaTime[(char*)sLogIPName.c_str()] = StrToInt(sLogDuration);
			}else{
				mAreaTime[(char*)sLogIPName.c_str()] = mAreaTime[(char*)sLogIPName.c_str()]+diffTime;
			}
			LogInOutLOG->WriteLog("3.<登入IP、登出地域以及地域持续时间总和>sLogIp=%s&sLogIPName=%s&sOnlineSum=%u\n",sLogIp.c_str(),sLogIPName.c_str(),mAreaTime[(char*)sLogIPName.c_str()]);
		}
#endif

		LogInOutSlotList.ICSID = Ics_Loginout.sICSId;
		LogInOutSlotList.ServiceID = Ics_Loginout.sServiceID;
		LogInOutSlotList.UserID = Ics_Loginout.sUin;
		LogInOutSlotList.WorldID = Ics_Loginout.sWordId;
		LogInOutSlotList.RoleID = Ics_Loginout.sRoleID;
		LogInOutSlotList.RoleName = Ics_Loginout.sRoleName;
		LogInOutSlotList.sBlackRobIp = sLogIp;
		LogInOutSlotList.sRobIPName = sLogIPName;
		LogInOutSlotList.LoginTime = sLoginTime;
		LogInOutSlotList.LogoutTime = sLogoutTime;
		LogInOutSlotList.OnLineTime = sLogDuration;
		LogInOutSlotList.iRobFlag = "0";
			
		//LogInOutLOG->WriteLog("4.<ICS账单的基本信息更新>sICSId=%s&sUin=%s&sRoleID=%s&sRoleName=%s\n",\
									LogInOutSlotList.ICSID.c_str(),\
									LogInOutSlotList.UserID.c_str(),\
									LogInOutSlotList.RoleID.c_str(),\
									LogInOutSlotList.RoleName.c_str());
		PutOutData.push_back(LogInOutSlotList);

	}
	LogInOutLOG->WriteLog("5.<登陆登出信息总条数>iCount=%d\n",PutOutData.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.<登陆登出信息总条数>iCount=%d\n",PutOutData.size());

	/*B---标记异常的登录总时长判断mAreaTime
	@	   1.如果总的地域种类数>6返回，无法判断处理；
	@	   2.如果总的地域时长<2小时，为异常登录，大于2小时则为正常,更新异常；
	@	   3.如果玩家没有异常登录时间段，返回异常
	*/
	if(mAreaTime.size()>6){
		return STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_EXCEED;
	}else if(mAreaTime.size()==1){
		return STEP_ROBSLOT_RUN_STAT_ERROR_FIND_ROB_SLOT;
	}
	LogInOutLOG->WriteLog("@@@Begin anaylseData() Rob Slot Find Logic Process@@@\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "@@@Begin anaylseData() Rob Slot Find Logic Process@@@\n");

	
	for(unsigned int i = 0; i< PutOutData.size(); i++ ){
		if (mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()] < 2*60*60 ){
			LogInOutLOG->WriteLog("1.<发现异常登录段>sRobIPName=【%s】&sOnlineSum=【%u】\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);
			PutOutData[i].iRobFlag = "1";
			iRobFlag = 1;
		}else{
			LogInOutLOG->WriteLog("2.<发现正常登录段>sRobIPName=%s&sOnlineSum=%u\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);
				WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.<发现正常登录段>sRobIPName=%s&sOnlineSum=%u\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);

			PutOutData[i].iRobFlag = "0";
		}
	}
	if(iRobFlag!=1){
		return STEP_ROBSLOT_RUN_STAT_ERROR_FIND_ROB_SLOT;
	}
	LogInOutLOG->WriteLog("************getRobLogInOutLog.anaylseData() Proccess Ended**************\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "************getRobLogInOutLog.anaylseData() Proccess Ended**************\n");
	return 0;
}

/**
Description:	分析输入的所有的登陆日志，并将全部返回，用于手动
@param	[in]	t_begin
@param	[in]	t_end
@param	[in]	uin
@param	[in]	roleid
@param	[in]	world
@param	[in]	map<string
@param	[in]	data
@return			void
@exception		none
*/
int RobAnlysIP::getRobLogInOutLog(Rob_Ics_Complaint& Ics_Loginout, std::vector< Rob_SLOT > &rob_data)
{
	time_t	begin;
	time_t	end;
	//起始时间，终止时间
	//发现被盗时间---最后一次登录时间
	string	s_begin	= Ics_Loginout.stRobeLogin;
	string	s_end	= Ics_Loginout.stLastLogin;

	string	s_uin	= Ics_Loginout.sUin;
	string	s_roleid = Ics_Loginout.sRoleID;
	string	s_wid	= Ics_Loginout.sWordId;
	string  s_rolename = Ics_Loginout.sRoleName;
	//接口的入参和出参
	vector< map<string,string> > data;
	map<string,string> LoginLogoutParam;
	//至少的登出日志条数
	int ThresholdCount = 2;
	int iRes = 0;

#ifdef dnf2
		//因为登录登出日志分开导致必须要修改阀值
		ThresholdCount = 6;
#endif

	GetTmsecFromUnxfmt(begin,(s_begin).c_str());
	GetTmsecFromUnxfmt(end,(s_end).c_str());
	
	/*将查询登陆日志的时间前后延长10天*/
	GetUnxfmtFromTmsec(s_begin,(begin-15*24*3600));
	GetUnxfmtFromTmsec(s_end,(end+2*3600));
	LogInOutLOG->WriteLog("Get LogIn and LogOut Time Between Value is %s and %s\n",s_begin.c_str(),s_end.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogIn and LogOut Time Between Value is %s and %s\n",s_begin.c_str(),s_end.c_str());
	
	/*判断盗号开始时间和结束时间的有效时间*/
	if(!checkValid(begin, end))
	{
		return STEP_ROBSLOT_RUN_STAT_ERROR_TIMESLOT;
	}
	
	/*查询角色登录登出日志*/
	LogInOutLOG->WriteLog("Get LogIn and LogOut Role Info=>|QQ:%s|RoleId:%s|Area:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogIn and LogOut Role Info=>|QQ:%s|RoleId:%s|Area:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str());

	LoginLogoutParam["uin"] = s_uin;
	LoginLogoutParam["gid"] = s_roleid;
#ifdef dnf2
	//dnf没有roleid只有角色名
	LoginLogoutParam["gid"] = string(GbkToUtf8((char*)s_rolename.c_str()));
#endif
	LoginLogoutParam["area"] = s_wid;
	LoginLogoutParam["start_time"] = s_begin;
	LoginLogoutParam["end_time"] = s_end;

	ics_log_header header;
	int ires = api_log.GetFunction(header,LoginLogoutParam,"login_logout_log",data);
	if (0 != ires)
	{
		LogInOutLOG->WriteLog("Get LogInAndOut Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,data.size());
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogInAndOut Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,data.size());

		return STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE;
	}
	LogInOutLOG->WriteLog("Get LogInAndOut Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,data.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogInAndOut Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,data.size());
	
	/*玩家的登录登出的日志总条数不能小于ThresholdCount*/
	if(data.size() < ThresholdCount){
		LogInOutLOG->WriteLog("Get DataResult'Size[%d] Too Small AND Less Than [%d]\n",data.size(),ThresholdCount);
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get DataResult'Size[%d] Too Small AND Less Than [%d]\n",data.size(),ThresholdCount);

		return STEP_ROBSLOT_RUN_STAT_ERROR_COUNT_SMALL;
	}
	/*分析异常IP的时间段，分析时间前后延长半天*/
	//return 0;
	iRes = anaylseData(Ics_Loginout, data, rob_data);
	return iRes;
}


/**
Description:	分析物品流水日志
@param	[in]	t_begin
@param	[in]	t_end
@param	[in]	uin
@param	[in]	roleid
@param	[in]	world
@param	[in]	map<string
@param	[in]	data
@return			void
@exception		none
*/
int RobAnlysIP::getItemFlowLog(const std::string &t_begin, const std::string &t_end, const std::string &uin, const std::string &roleid, const std::string &world, std::vector<map<string,string> > &rob_item_data)
{
	string	s_begin	= t_begin;
	string	s_end	= t_end;

	string	s_uin	= uin;
	string	s_roleid = roleid;
	string	s_wid	= world;

	/*查询物品流水日志*/
	map<string,string> ItemFlowResultReq;
	LogInOutLOG->WriteLog("Get getItemFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getItemFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());

	ItemFlowResultReq["uin"] = s_uin;
	ItemFlowResultReq["gid"] = s_roleid;
	ItemFlowResultReq["area"] = s_wid;
	ItemFlowResultReq["start_date"] = s_begin;
	ItemFlowResultReq["end_date"] = s_end;
	ics_log_header header;
	int ires = api_log.GetFunction(header,ItemFlowResultReq,"ItemFlow",rob_item_data);
	if (0 != ires)
	{
		LogInOutLOG->WriteLog("Get getItemFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getItemFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());

		return ires;
	}
	LogInOutLOG->WriteLog("Get getItemFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getItemFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	
	return 0;
}


/**
Description:	分析装备流水日志
@param	[in]	t_begin
@param	[in]	t_end
@param	[in]	uin
@param	[in]	roleid
@param	[in]	world
@param	[in]	map<string
@param	[in]	data
@return			void
@exception		none
*/
int RobAnlysIP::getArmFlowLog(const std::string &t_begin, const std::string &t_end, const std::string &uin, const std::string &roleid, const std::string &world, std::vector<map<string,string> > &rob_item_data)
{
	string	s_begin	= t_begin;
	string	s_end	= t_end;

	string	s_uin	= uin;
	string	s_roleid = roleid;
	string	s_wid	= world;

	/*查询物品流水日志*/
	map<string,string> ItemFlowResultReq;
	LogInOutLOG->WriteLog("Get getArmFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getArmFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());

	ItemFlowResultReq["uin"] = s_uin;
	ItemFlowResultReq["gid"] = s_roleid;
	ItemFlowResultReq["area"] = s_wid;
	ItemFlowResultReq["start_date"] = s_begin;
	ItemFlowResultReq["end_date"] = s_end;
	
	ics_log_header header;
	int ires = api_log.GetFunction(header,ItemFlowResultReq,"ArmFlow",rob_item_data);
	if (0 != ires)
	{
		LogInOutLOG->WriteLog("Get getArmFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getArmFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
		return ires;
	}
	LogInOutLOG->WriteLog("Get getArmFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getArmFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	
	return 0;
}

/**
Description:	分析宠物流水日志
@param	[in]	t_begin
@param	[in]	t_end
@param	[in]	uin
@param	[in]	roleid
@param	[in]	world
@param	[in]	map<string
@param	[in]	data
@return			void
@exception		none
*/
int RobAnlysIP::getPetFlowLog(const std::string &t_begin, const std::string &t_end, const std::string &uin, const std::string &roleid, const std::string &world, std::vector<map<string,string> > &rob_item_data)
{
	string	s_begin	= t_begin;
	string	s_end	= t_end;

	string	s_uin	= uin;
	string	s_roleid = roleid;
	string	s_wid	= world;

	/*查询物品流水日志*/
	map<string,string> ItemFlowResultReq;
	LogInOutLOG->WriteLog("Get getPetFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getPetFlowLog Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str(),s_begin.c_str(),s_end.c_str());

	ItemFlowResultReq["uin"] = s_uin;
	ItemFlowResultReq["gid"] = s_roleid;
	ItemFlowResultReq["area"] = s_wid;
	ItemFlowResultReq["start_date"] = s_begin;
	ItemFlowResultReq["end_date"] = s_end;
	ics_log_header header;
	int ires = api_log.GetFunction(header,ItemFlowResultReq,"ArmFlow",rob_item_data);
	if (0 != ires)
	{
		LogInOutLOG->WriteLog("Get getPetFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getPetFlowLog Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());

		return ires;
	}
	LogInOutLOG->WriteLog("Get getPetFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getPetFlowLog Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,rob_item_data.size());
	
	return 0;
}

// 离线分析,异常登录分析
int RobAnlysIP::getRobLogInOutLogOffline(Rob_Ics_Complaint &ics_info, vector< map<string, string> > &vOfflineSrc, vector<Rob_SLOT> &rob_data)
{
	ics_log_header logHeader(ics_info.sICSId, ics_info.sUin, ics_info.sServiceID);
	
	map<string, string> inParams;
	vector< map<string, string> > logRet;
	Rob_SLOT tmpLogoutRet;
	int iRet = 0;
	
	for ( int i = 0; i < vOfflineSrc.size(); i++ )
	{
		inParams.clear();
		inParams["area"] = ics_info.sWordId;
		inParams["uin"] = ics_info.sUin;
		inParams["ndx"] = IntToStr(StrToLLong(ics_info.sUin)%100);
		inParams["role_name"] = ics_info.sRoleName;
		inParams["login_ip"] = vOfflineSrc[i]["login_ip"];
		inParams["login_time"] = vOfflineSrc[i]["login_time"];
		
		logRet.clear();
		iRet = api_log.GetFunction(logHeader, inParams, "rob_offline_login_log", logRet);
		LogInOutLOG->WriteLog("查询异常登录日志结果: area[%s],uin[%s],role_name[%s],login_ip[%s],login_time[%s],iRet[%d],size[%d]\n", inParams["area"].c_str(), inParams["uin"].c_str(), inParams["role_name"].c_str(), inParams["login_ip"].c_str(), inParams["login_time"].c_str(), iRet, logRet.size());
		
		if ( 0 != iRet || 1 != logRet.size() )
		{
			return STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE;
		}
		
		tmpLogoutRet.ICSID = ics_info.sICSId;			// 单据号
		tmpLogoutRet.ServiceID = ics_info.sServiceID;	// 业务ID
		tmpLogoutRet.WorldID = ics_info.sWordId;		// 大区
		tmpLogoutRet.UserID = ics_info.sUin;			// QQ
		tmpLogoutRet.RoleID = ics_info.sRoleID;			// 角色ID
		tmpLogoutRet.RoleName = ics_info.sRoleName;		// 角色名
		tmpLogoutRet.sBlackRobIp = vOfflineSrc[i]["login_ip"];	// 异常登录IP
		tmpLogoutRet.sRobIPName = IPLib::getAreaByIP(vOfflineSrc[i]["login_ip"]);	// 异常登录地区
		tmpLogoutRet.LoginTime = vOfflineSrc[i]["login_time"];	// 异常登录时间
		tmpLogoutRet.iMoney_Sub = "0";	// 金钱减少个数
		tmpLogoutRet.iRobFlag = "1";	// 是否是盗号日志
		
		logRet.clear();
		iRet = api_log.GetFunction(logHeader, inParams, "rob_offline_logout_log", logRet);
		LogInOutLOG->WriteLog("查询异常登出日志结果: area[%s],uin[%s],role_name[%s],login_ip[%s],login_time[%s],iRet[%d],size[%d]\n", inParams["area"].c_str(), inParams["uin"].c_str(), inParams["role_name"].c_str(), inParams["login_ip"].c_str(), inParams["login_time"].c_str(), iRet, logRet.size());
		
		if ( 0 != iRet || logRet.size() > 1 )
		{
			return STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE;
		}
		else if ( 1 == logRet.size() )
		{
			tmpLogoutRet.LogoutTime = logRet[0]["dtDateTime"];	// 异常登出时间
			tmpLogoutRet.OnLineTime = logRet[0]["iPlayTime"];	// 在线时长
		}
		else
		{
			time_t stime;
			GetTmsecFromUnxfmt(stime, inParams["login_time"].c_str());
			
			time_t ctime = time(0);
			char LogoutTime[20] = {0};
			strftime(LogoutTime, sizeof(LogoutTime), "%Y-%m-%d %H:%M:%S", localtime(&ctime));
			
			tmpLogoutRet.LogoutTime = LogoutTime;				// 异常登出时间
			tmpLogoutRet.OnLineTime = IntToStr(ctime-stime);	// 在线时长
		}
		rob_data.push_back(tmpLogoutRet);
	}
	return 0;
}
