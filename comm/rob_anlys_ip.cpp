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
Description:	���캯��
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
Description:	��������
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
Description:	��������Ϸ�����֤
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
�жϵ���ʱ���������ʱ������12Сʱ����Ϊ����������true�����򷵻�false
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
Description:	��Ϸ��½��־�����߼�
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

	//���ڼ�¼�ڲ�ͬ�ĵ�����½���ܵ�ʱ��
	map <string,unsigned int> mAreaTime;			//��¼��¼��������ͳ��ʱ��
	map <string,unsigned int> mAreaCount;			//��¼��¼�����Ĵ���
	map <string,unsigned int> mAreaDiffentCount;	//��¼��¼��ͬ�����Ĵ���
	map <string,string> mIPtoArea;					//��¼��¼IP��Ӧ��������
	PutOutData.clear();


	//��¼��һ����¼�е�ip��Ӧ�ĵ���
	string sLastArea = "0";
	string sLogIp = "0.0.0.0";
	string sLogIPName = "�㶫ʡ.������";
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
		
		LogInOutLOG->WriteLog("a.������¼��־��Χ��slogType=%s\n",slogType.c_str());
		if(slogType=="0"){//����
			if(LogInOutData.size()%2==0){
				from_id = 1;
				end_id = LogInOutData.size()-1;
			}else{
				from_id = 1;
				end_id = LogInOutData.size();
			}
		}else if(slogType=="1"){//�ǳ�
			if(LogInOutData.size()%2==0){
				from_id = 0;
				end_id = LogInOutData.size();
			}else{
				from_id = 0;//modified by v_zhtang on 2013-05-23;���ֶ�һ�£��޸��ǳ�����벻ƥ���bug
				end_id = LogInOutData.size()-2;//
			}
		}else{
			LogInOutLOG->WriteLog("In anaylseData() DNF2 process get iAct type error \n");
		}
		LogInOutLOG->WriteLog("b.������¼��־��Χ��slogType=%s&allsize=%d&from_id=%d&end_id=%d\n",slogType.c_str(),LogInOutData.size(),from_id,end_id);
#endif
	LogInOutLOG->WriteLog("@@@Begin anaylseData() Traversal Logic Process@@@\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "@@@Begin anaylseData() Traversal Logic Process@@@\n");

	//�״α�����IPת���ɵ���ֵ��ͬʱ�������ͬһ������½����ʱ��
	for(unsigned int i = from_id; i < end_id; i++)
	{
		sLogIp = LogInOutData[i][(char*)sIpFlag.c_str()];
		sLoginTime = LogInOutData[i][(char*)sLoginTimeFlag.c_str()];
		sLogoutTime = LogInOutData[i][(char*)sLogoutTimeFlag.c_str()];
#ifdef dnf2
		slogType = LogInOutData[i]["iAct"];
#endif
		//����IP��ȡ������Ϣ
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
		//LogInOutLOG->WriteLog("1.<��¼�͵ǳ�����־ID>i=%d&from_id=%d&end_id=%d&slogType=%s\n",i,from_id,end_id,slogType.c_str());
#ifdef dnf2
		//�����Ӷ������
		//��Ϊdnf2�ĵ�¼�ǳ���־�ķֿ��ģ�������Ҫ���кϲ�
		//�ϲ�����
		//1 0 1 0 1 0�ṹ--!
		if((i-from_id)%2==0){
			continue;
		}else{
			LogInOutLOG->WriteLog("---1.a i=%d\n",i);
			//����IP��ȡ������Ϣ
			if(mIPtoArea.find((char*)sLogIp.c_str())== mIPtoArea.end()){
				string area=IPLib::getAreaByIP(sLogIp);
				Trim(area);
				if(area!=""&&area!="δ֪IP"){
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
			LogInOutLOG->WriteLog("2.<����ʱ�䡢�ǳ�ʱ���Լ�����ʱ��>sLogDuration=%s&sLoginTime=%s&sLogoutTime=%s\n",sLogDuration.c_str(),sLoginTime.c_str(),sLogoutTime.c_str());
			if(mAreaTime.find((char*)sLogIPName.c_str())== mAreaTime.end()){
				mAreaTime[(char*)sLogIPName.c_str()] = StrToInt(sLogDuration);
			}else{
				mAreaTime[(char*)sLogIPName.c_str()] = mAreaTime[(char*)sLogIPName.c_str()]+diffTime;
			}
			LogInOutLOG->WriteLog("3.<����IP���ǳ������Լ��������ʱ���ܺ�>sLogIp=%s&sLogIPName=%s&sOnlineSum=%u\n",sLogIp.c_str(),sLogIPName.c_str(),mAreaTime[(char*)sLogIPName.c_str()]);
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
			
		//LogInOutLOG->WriteLog("4.<ICS�˵��Ļ�����Ϣ����>sICSId=%s&sUin=%s&sRoleID=%s&sRoleName=%s\n",\
									LogInOutSlotList.ICSID.c_str(),\
									LogInOutSlotList.UserID.c_str(),\
									LogInOutSlotList.RoleID.c_str(),\
									LogInOutSlotList.RoleName.c_str());
		PutOutData.push_back(LogInOutSlotList);

	}
	LogInOutLOG->WriteLog("5.<��½�ǳ���Ϣ������>iCount=%d\n",PutOutData.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.<��½�ǳ���Ϣ������>iCount=%d\n",PutOutData.size());

	/*B---����쳣�ĵ�¼��ʱ���ж�mAreaTime
	@	   1.����ܵĵ���������>6���أ��޷��жϴ���
	@	   2.����ܵĵ���ʱ��<2Сʱ��Ϊ�쳣��¼������2Сʱ��Ϊ����,�����쳣��
	@	   3.������û���쳣��¼ʱ��Σ������쳣
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
			LogInOutLOG->WriteLog("1.<�����쳣��¼��>sRobIPName=��%s��&sOnlineSum=��%u��\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);
			PutOutData[i].iRobFlag = "1";
			iRobFlag = 1;
		}else{
			LogInOutLOG->WriteLog("2.<����������¼��>sRobIPName=%s&sOnlineSum=%u\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);
				WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.<����������¼��>sRobIPName=%s&sOnlineSum=%u\n",PutOutData[i].sRobIPName.c_str(),mAreaTime[(char*)PutOutData[i].sRobIPName.c_str()]);

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
Description:	������������еĵ�½��־������ȫ�����أ������ֶ�
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
	//��ʼʱ�䣬��ֹʱ��
	//���ֱ���ʱ��---���һ�ε�¼ʱ��
	string	s_begin	= Ics_Loginout.stRobeLogin;
	string	s_end	= Ics_Loginout.stLastLogin;

	string	s_uin	= Ics_Loginout.sUin;
	string	s_roleid = Ics_Loginout.sRoleID;
	string	s_wid	= Ics_Loginout.sWordId;
	string  s_rolename = Ics_Loginout.sRoleName;
	//�ӿڵ���κͳ���
	vector< map<string,string> > data;
	map<string,string> LoginLogoutParam;
	//���ٵĵǳ���־����
	int ThresholdCount = 2;
	int iRes = 0;

#ifdef dnf2
		//��Ϊ��¼�ǳ���־�ֿ����±���Ҫ�޸ķ�ֵ
		ThresholdCount = 6;
#endif

	GetTmsecFromUnxfmt(begin,(s_begin).c_str());
	GetTmsecFromUnxfmt(end,(s_end).c_str());
	
	/*����ѯ��½��־��ʱ��ǰ���ӳ�10��*/
	GetUnxfmtFromTmsec(s_begin,(begin-15*24*3600));
	GetUnxfmtFromTmsec(s_end,(end+2*3600));
	LogInOutLOG->WriteLog("Get LogIn and LogOut Time Between Value is %s and %s\n",s_begin.c_str(),s_end.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogIn and LogOut Time Between Value is %s and %s\n",s_begin.c_str(),s_end.c_str());
	
	/*�жϵ��ſ�ʼʱ��ͽ���ʱ�����Чʱ��*/
	if(!checkValid(begin, end))
	{
		return STEP_ROBSLOT_RUN_STAT_ERROR_TIMESLOT;
	}
	
	/*��ѯ��ɫ��¼�ǳ���־*/
	LogInOutLOG->WriteLog("Get LogIn and LogOut Role Info=>|QQ:%s|RoleId:%s|Area:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get LogIn and LogOut Role Info=>|QQ:%s|RoleId:%s|Area:%s|\n",s_uin.c_str(),s_roleid.c_str(),s_wid.c_str());

	LoginLogoutParam["uin"] = s_uin;
	LoginLogoutParam["gid"] = s_roleid;
#ifdef dnf2
	//dnfû��roleidֻ�н�ɫ��
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
	
	/*��ҵĵ�¼�ǳ�����־����������С��ThresholdCount*/
	if(data.size() < ThresholdCount){
		LogInOutLOG->WriteLog("Get DataResult'Size[%d] Too Small AND Less Than [%d]\n",data.size(),ThresholdCount);
		WriteLogsRUN(TLOG_TYPE_ROBANLYSIP , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get DataResult'Size[%d] Too Small AND Less Than [%d]\n",data.size(),ThresholdCount);

		return STEP_ROBSLOT_RUN_STAT_ERROR_COUNT_SMALL;
	}
	/*�����쳣IP��ʱ��Σ�����ʱ��ǰ���ӳ�����*/
	//return 0;
	iRes = anaylseData(Ics_Loginout, data, rob_data);
	return iRes;
}


/**
Description:	������Ʒ��ˮ��־
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

	/*��ѯ��Ʒ��ˮ��־*/
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
Description:	����װ����ˮ��־
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

	/*��ѯ��Ʒ��ˮ��־*/
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
Description:	����������ˮ��־
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

	/*��ѯ��Ʒ��ˮ��־*/
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

// ���߷���,�쳣��¼����
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
		LogInOutLOG->WriteLog("��ѯ�쳣��¼��־���: area[%s],uin[%s],role_name[%s],login_ip[%s],login_time[%s],iRet[%d],size[%d]\n", inParams["area"].c_str(), inParams["uin"].c_str(), inParams["role_name"].c_str(), inParams["login_ip"].c_str(), inParams["login_time"].c_str(), iRet, logRet.size());
		
		if ( 0 != iRet || 1 != logRet.size() )
		{
			return STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE;
		}
		
		tmpLogoutRet.ICSID = ics_info.sICSId;			// ���ݺ�
		tmpLogoutRet.ServiceID = ics_info.sServiceID;	// ҵ��ID
		tmpLogoutRet.WorldID = ics_info.sWordId;		// ����
		tmpLogoutRet.UserID = ics_info.sUin;			// QQ
		tmpLogoutRet.RoleID = ics_info.sRoleID;			// ��ɫID
		tmpLogoutRet.RoleName = ics_info.sRoleName;		// ��ɫ��
		tmpLogoutRet.sBlackRobIp = vOfflineSrc[i]["login_ip"];	// �쳣��¼IP
		tmpLogoutRet.sRobIPName = IPLib::getAreaByIP(vOfflineSrc[i]["login_ip"]);	// �쳣��¼����
		tmpLogoutRet.LoginTime = vOfflineSrc[i]["login_time"];	// �쳣��¼ʱ��
		tmpLogoutRet.iMoney_Sub = "0";	// ��Ǯ���ٸ���
		tmpLogoutRet.iRobFlag = "1";	// �Ƿ��ǵ�����־
		
		logRet.clear();
		iRet = api_log.GetFunction(logHeader, inParams, "rob_offline_logout_log", logRet);
		LogInOutLOG->WriteLog("��ѯ�쳣�ǳ���־���: area[%s],uin[%s],role_name[%s],login_ip[%s],login_time[%s],iRet[%d],size[%d]\n", inParams["area"].c_str(), inParams["uin"].c_str(), inParams["role_name"].c_str(), inParams["login_ip"].c_str(), inParams["login_time"].c_str(), iRet, logRet.size());
		
		if ( 0 != iRet || logRet.size() > 1 )
		{
			return STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE;
		}
		else if ( 1 == logRet.size() )
		{
			tmpLogoutRet.LogoutTime = logRet[0]["dtDateTime"];	// �쳣�ǳ�ʱ��
			tmpLogoutRet.OnLineTime = logRet[0]["iPlayTime"];	// ����ʱ��
		}
		else
		{
			time_t stime;
			GetTmsecFromUnxfmt(stime, inParams["login_time"].c_str());
			
			time_t ctime = time(0);
			char LogoutTime[20] = {0};
			strftime(LogoutTime, sizeof(LogoutTime), "%Y-%m-%d %H:%M:%S", localtime(&ctime));
			
			tmpLogoutRet.LogoutTime = LogoutTime;				// �쳣�ǳ�ʱ��
			tmpLogoutRet.OnLineTime = IntToStr(ctime-stime);	// ����ʱ��
		}
		rob_data.push_back(tmpLogoutRet);
	}
	return 0;
}
