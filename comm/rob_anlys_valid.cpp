/**
@file rob_anlys_valid.cpp

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		GunLi
@version	1.0
@date		2010/10/22

history:
<PRE>
History ID	:	1
Author    	:	GunLi
DateTime	:	2010/10/22
Description :	Create
</PRE>
*/

#include "rob_anlys_valid.h"
#include "iplib.h"
#include "comm_datetime.h"
RobRuleValid::RobRuleValid()
{
	MAKEVALIDLOG="./logs/"+GetCurDateDay()+"RobRuleValid.log";
	ValidLOG = new CLog((const char*)MAKEVALIDLOG.c_str());
	ValidLOG->LogOn();
	iRet = 0;
}

RobRuleValid::~RobRuleValid()
{

	if(NULL != ValidLOG)
	{
		delete ValidLOG;
	}

}



/**
Description:	����ҵ��Ĳ����Ƿ���˸߼�ֵ��Ʒ
@param	[in]	info
@return			in_size������ǰ����Ʒ������ out_size�����˺����Ʒ������ fliter_item_flag �Ƿ���и߼�ֵ��Ʒ���˵ı�־λ

fliter_item_flag ��
0  ���ù��˸߼�ֵ��Ʒ
1  ��Ҫ���˸߼�ֵ��Ʒ

return:
0	   ����
other  ������,���ִ�����Ϣ

@exception		none
*/
int RobRuleValid::DoGetItemsToPresent(spoil_info all_item_info[],spoil_info get_item_info[],const unsigned int in_size,unsigned int& out_size,int fliter_item_flag)
{
	ValidLOG->WriteLog("In DoGetItemsToPresent() Proccess!\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In DoGetItemsToPresent() Proccess!\n");

	ValidLOG->WriteLog("DoGetItemsToPresent()INParams=in_size:%d|out_size:%d|fliter_item_flag:%d\n",in_size,out_size,fliter_item_flag);
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "DoGetItemsToPresent()INParams=in_size:%d|out_size:%d|fliter_item_flag:%d\n",in_size,out_size,fliter_item_flag);

	//������
	if(fliter_item_flag == 0)
	{
		for(unsigned int i = 0; i <in_size; i++)
		{
			get_item_info[i]=all_item_info[i];
		}
		out_size = in_size;
	}
	ValidLOG->WriteLog("DoGetItemsToPresent() OUTParams=in_size:%d|out_size:%d|fliter_item_flag:%d\n",in_size,out_size,fliter_item_flag);
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "DoGetItemsToPresent() OUTParams=in_size:%d|out_size:%d|fliter_item_flag:%d\n",in_size,out_size,fliter_item_flag);

	return 0;
}


/**
Description:	�趨������Ʒ�Ƿ�ת�˹�����
@param			[in]	info
@				(a):    ��Ʒ�Ƿ����
@return			int     �ӿ���ȷ���
@exception		none
*/
int RobRuleValid::DoSetItemsToPresent(spoil_info all_item_info[],const unsigned int in_size)
{
	ValidLOG->WriteLog("In DoSetItemsToPresent() Proccess!\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In DoSetItemsToPresent() Proccess!\n");

	for(unsigned int i = 0; i <in_size; i++)
	{
		/* 
		@ (a): �����Ʒ�ǵ���״̬��
		@		  Status   1    ת�˹�����
		*/
		if(all_item_info[i].ItemCou != "1")
		{
			all_item_info[i].Status = "1"; //��Ҫ�˹�����
		}
		else
		{
			all_item_info[i].Status = "0";
		}
	}
	return 0;
}





/**********************************************************************************
Description:	��ҵĽ�ɫ������Ϣ����
@param	[in]	info
@return			int
@exception		none
*/
int RobRuleValid::DoICSSetRoleInfoPro(Rob_Ics_Complaint& ics_info)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	int iRoleExistFlag = 0;
	ValidLOG->WriteLog("In Valid Proccess,DoICSSetRoleInfoPro-IcsInfo==|ICSID:%s|UIN:%s|Role_ID:%s\n",ics_info.sICSId.c_str(),ics_info.sUin.c_str(),ics_info.sRoleID.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess,DoICSSetRoleInfoPro-IcsInfo==|ICSID:%s|UIN:%s|Role_ID:%s\n",ics_info.sICSId.c_str(),ics_info.sUin.c_str(),ics_info.sRoleID.c_str());


	//��һ����ɫ������Ϣ���£���ɫ���ֺ͵ȼ�
	//Code Should Code Here
	//Only For dnf2
#ifdef dnf2
	//ics_get_role_list
	ValidLOG->WriteLog("In [dnf2] Proccess,ǰ�� ׼������ɫ�б�\n");
	theParamIn["area"]=ics_info.sWordId;
	theParamIn["uin"]=ics_info.sUin;
	ics_log_header header;
	iRet = valid_parse.GetFunction(header,theParamIn,"ics_get_role_list",theGetResult);
	ValidLOG->WriteLog("In Valid Proccess,DoICSSetRoleInfoPro-ics_get_role_list-iRet=%d\n",iRet);
	/*<field name="name_0"><![CDATA[20703365]]></field>
	<field name="name_1"><![CDATA[�価������]]></field>
	<field name="name_2"><![CDATA[2]]></field>
	<field name="name_3"><![CDATA[69]]></field>
	*/
	for(int i = 0;i<theGetResult.size();i++){


		if(theGetResult[i]["name_0"]==ics_info.sRoleID){
			ics_info.sRoleName=theGetResult[i]["name_1"];
			ics_info.sRoleLevel=theGetResult[i]["name_3"];
			iRoleExistFlag = 1;
			ValidLOG->WriteLog("[INFO] role_name=%s&role_id=%s&role_level=%s&\n",ics_info.sRoleName.c_str(),ics_info.sRoleID.c_str(),ics_info.sRoleLevel.c_str());
		}
	}
	if(iRoleExistFlag==0){
		ValidLOG->WriteLog("û���ҵ������ҽ�ɫ��Ϣ\n");
		return STEP_FLITER_RUN_STAT_ERROR_ROLE_INFO;
	}
#endif
	//�ڶ�����ɫ������Ϣ���£���ɫ�����Ϣ
	theParamIn.clear();
	theGetResult.clear();
#ifdef dnf2
	//modify by asherzhou 2012-05-23
	//��Ϊ��Ŀ��˵��������ж�
	/*		ValidLOG->WriteLog("In [dnf2] Proccess,�ڶ�����ɫ������Ϣ���£���ɫ�����Ϣ\n");
	theParamIn["uin"]=ics_info.sUin;
	iRet = valid_parse.GetFunction(theParamIn,"ics_get_role_ban_info",theGetResult);
	ValidLOG->WriteLog("In Valid Proccess,DoICSSetRoleInfoPro-ics_get_role_ban_info-iRet=%d\n",iRet);
	if(iRet!=0||theGetResult.size()!=1){
	return STEP_FLITER_RUN_STAT_ERROR_ROLE_INFO;
	}else{
	/*
	<field name="result"><![CDATA[2]]></field>
	<field name="uin"><![CDATA[309436543]]></field>
	*/
	/*			ValidLOG->WriteLog("In [dnf2] Proccess,DoICSSetRoleInfoPro-ics_get_role_ban_info-result=%s\n",theGetResult[0]["result"].c_str());
	if(theGetResult[0]["result"]=="1"||theGetResult[0]["result"]=="2"){
	ics_info.siRobedRoleBan =  "0";
	}else if(theGetResult[0]["result"]=="0"){
	ics_info.siRobedRoleBan =  "1";
	}else{
	ics_info.siRobedRoleBan =  "0";
	return STEP_FLITER_RUN_STAT_ERROR_ROLE_INFO;
	}
	}
	*/
	ics_info.siRobedRoleBan =  "0";
#endif
	//��������ɫ������Ϣ���£���ɫVIP��Ϣ
	theParamIn.clear();
	theGetResult.clear();
#ifdef dnf2

	ValidLOG->WriteLog("In [dnf2] Proccess,��������ɫ������Ϣ���£���ɫVIP��Ϣ\n");
	int vip_level=ipInterface.getVipByUin(ics_info.sUin);
	ValidLOG->WriteLog("[INFO]: vip_level=%d\n",vip_level);
	ics_info.sIsVip = IntToStr(vip_level);
#endif
	return 0;
}


/**********************************************************************************
Description:	��֤���ݵĺϷ���
@param	[in]	info
@return			int
@exception		none
*/

int RobRuleValid::DoICSCheckPro(Rob_Ics_Complaint& ics_info)
{
	ValidLOG->WriteLog("[INFO] [function]DoICSCheckPro::argv:ICSID=%s&UIN=%s&\n",ics_info.sICSId.c_str(),ics_info.sUin.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO] [function]DoICSCheckPro::argv:ICSID=%s&UIN=%s&\n",ics_info.sICSId.c_str(),ics_info.sUin.c_str());

	//A ���Ƚ�����ص�������ݸ���
	iRet = DoICSSetRoleInfoPro(ics_info);
	if(iRet!=0){
		ValidLOG->WriteLog("[ERROR] [function]DoICSCheckPro::DoICSSetRoleInfoPro::return(%d)\n",iRet);
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR] [function]DoICSCheckPro::DoICSSetRoleInfoPro::return(%d)\n",iRet);

		return iRet;
	}
	ValidLOG->WriteLog("[PHASE] [function]DoICSCheckPro::DoICSSetRoleInfoPro::return(%d)\n",iRet);
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[PHASE] [function]DoICSCheckPro::DoICSSetRoleInfoPro::return(%d)\n",iRet);

	ValidLOG->WriteLog("[INFO]  [function]DoICSCheckPro::ics_info:ICSID=%s&UIN=%s&sRoleID=%s&RoleName=%s&sIsVip=%s&sRoleLevel=%s&siRobedRoleBan=%s&\n",\
		ics_info.sICSId.c_str(),\
		ics_info.sUin.c_str(),\
		ics_info.sRoleID.c_str(),\
		ics_info.sRoleName.c_str(),\
		ics_info.sIsVip.c_str(),\
		ics_info.sRoleLevel.c_str(),\
		ics_info.siRobedRoleBan.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]  [function]DoICSCheckPro::ics_info:ICSID=%s&UIN=%s&sRoleID=%s&RoleName=%s&sIsVip=%s&sRoleLevel=%s&siRobedRoleBan=%s&\n",\
		ics_info.sICSId.c_str(),\
		ics_info.sUin.c_str(),\
		ics_info.sRoleID.c_str(),\
		ics_info.sRoleName.c_str(),\
		ics_info.sIsVip.c_str(),\
		ics_info.sRoleLevel.c_str(),\
		ics_info.siRobedRoleBan.c_str());

	if (0 != ProCheckRoleLevel(ics_info)){
		// �û��ȼ������Ϸ���Ҫ��
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'ProCheckRoleLevel' is �û��ȼ������Ϸ���Ҫ��\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'ProCheckRoleLevel' is �û��ȼ������Ϸ���Ҫ��\n");

		return STEP_FLITER_RUN_STAT_ERROR_LEVEL;
	}else if(0 != ProCheckBlockTime(ics_info)){
		// ��ɫ�������5��
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'ProCheckBlockTime' is ��ɫ�������5��\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'ProCheckBlockTime' is ��ɫ�������5��\n");

		return STEP_FLITER_RUN_STAT_ERROR_BANNED;
	}else if (0 != ProCheckRobedTime(ics_info)){
		// ����ʱ�䳬������Ҫ��
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'ProCheckRobedTime' is ����ʱ�䳬������Ҫ��\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'ProCheckRobedTime' is ����ʱ�䳬������Ҫ��\n");

		return STEP_FLITER_RUN_STAT_ERROR_APPLY_TIME;
	}else if (0 != ProCheckLastTime(ics_info))
	{
		// ���ֱ���ʱ�䲻�������һ�ε�½ʱ��
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is  ���ֱ���ʱ�䲻�������һ�ε�½ʱ��\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'CheckRobedTime' is  ���ֱ���ʱ�䲻�������һ�ε�½ʱ��\n");

		return STEP_FLITER_RUN_STAT_ERROR_INPUT_TIME;
	}else if ((iRet=ProCheckDobleQuest(ics_info))!=0)
	{
		// 60�����ظ�����
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is   60�����ظ�����\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'CheckRobedTime' is   60�����ظ�����\n");

		return iRet;
	}
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'DoICSCheck' is  valid ,Go to Auto Process\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess, Find Uin 'DoICSCheck' is  valid ,Go to Auto Process\n");

	// ������������
	return STEP_FLITER_RUN_STAT_SUCCESS;
}
/**********************************************************************************
*	����LEVEL��ics��check���˺���
*   params:		ics_info
* 	return:		int
*/
int RobRuleValid::ProCheckRoleLevel(Rob_Ics_Complaint& info)
{
	ValidLOG->WriteLog("[Info]'ProCheckRoleLevel' Proccessing\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckRoleLevel' Proccessing\n");

	return (StrToInt(info.sRoleLevel) < StrToInt(CRobConf::getConf()->GetIcsValid()["role_level"]))?1:0;
}
/***********************************************************************************	
*	����BLOCK��ics��check���˺���
*   params:		ics_info
* 	return:		int
*/
int RobRuleValid::ProCheckBlockTime(Rob_Ics_Complaint& info)
{
	ValidLOG->WriteLog("[Info]'ProCheckBlockTime' Proccessing\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckBlockTime' Proccessing\n");

	return StrToInt(info.siRobedRoleBan)==0?0:1;
}

/**  ����ʱ�䳬������Ҫ��, ��ͨ�û��ݶ�Ϊ15�죬VIPΪ20��***********************************************************************************	
* 		params:		ics_info
* 		return:		int
*/
int RobRuleValid::ProCheckRobedTime(Rob_Ics_Complaint &info)
{
	ValidLOG->WriteLog("[Info]'ProCheckRobedTime' Proccessing\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckRobedTime' Proccessing\n");

	time_t	tRobedTime;
	time_t	tCurTime = time(0);
	char szRobedTime[256];
	sprintf(szRobedTime, "%s", (char*)(info.stBookTime.c_str()));
	GetTmsecFromUnxfmt(tRobedTime, szRobedTime);

	return ((tCurTime-tRobedTime) > 3600*24*StrToInt(CRobConf::getConf()->GetIcsValid()["create_due"]))?1:0;
}
/**  ����ʱ�䳬������¼��ʱ��, �����Ϲ���************************************
* 		params:		ics_info
* 		return:		int
*/
int RobRuleValid::ProCheckLastTime(Rob_Ics_Complaint &info)
{
	ValidLOG->WriteLog("[Info]'ProCheckLastTime' Proccessing\n");
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckLastTime' Proccessing\n");

	char szLastLogin[256];
	sprintf(szLastLogin, "%s", (char*)(info.stLastLogin).c_str());
	char szRobedTime[256];
	sprintf(szRobedTime, "%s", (char*)(info.stRobeLogin).c_str());

	return (GetDiffTime(szRobedTime,szLastLogin)<0)?0:1;
}


/**   ����idip�ӿڲ���vip��Ϣ
* 		params:		ics_info
* 		return:		int
*/
int RobRuleValid::ProCheckVIPInfo(Rob_Ics_Complaint& info)
{
	map<string,string> theParamIn;
	vector< map<string,string> > theResult;

	theParamIn.clear();
	theParamIn["uin"] = info.sUin;
	theParamIn["business"] = "sap";

	ics_log_header header(info.sICSId,info.sUin,info.sServiceID);
	if(0 != valid_parse.GetFunction(header,theParamIn,"get_vip_info",theResult))
	{
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'get_vip_info' happened error,not find info\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID, info.sICSId.c_str(), info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "In Valid Proccess, Find Uin 'get_vip_info' happened error,not find info\n" );
		return -1;
	}
	else
	{
		ValidLOG->WriteLog("In Valid Proccess, Find Uin 'get_vip_info' success!\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID, info.sICSId.c_str(), info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "In Valid Proccess, Find Uin 'get_vip_info' success!\n" );
		return theResult[0]["type"] == "0" ? 0 : 1;
	}

}

/**   receive_doble�����ظ����������� ��ͨ�û��ݶ�Ϊ60�죬VIPΪ90��
* 		params:		ics_info
* 		return:		int
*/
int RobRuleValid::ProCheckDobleQuest(Rob_Ics_Complaint &info)
{
	//�˴���Ҫ��ϸ�Ĺ滮��,��ʱ������
	//select count(*) as applyValueCount from tbICSComplaint where TO_DAYS(now())-TO_DAYS(dtBookTime)<=60
	// and iUin='732945415' and iRoleID='12306';
	//
	vector< map<string,string> > base_result;
	map<string,string> theParamIn;
	theParamIn.clear();
	theParamIn["uin"] = info.sUin;
	theParamIn["service_id"] = info.sServiceID;
	int del_slot = StrToInt(CRobConf::getConf()->GetIcsValid()["receve_double"]);

	//����ʱ����Ч�� 
	if(info.sIsVip!="0"){
		del_slot = StrToInt(CRobConf::getConf()->GetIcsValid()["receve_vip_double"]);
	}

	//add by v_yfzhu 20121109 ----begin 
    if(ProCheckVIPInfo(info) == 1 )
	{
		del_slot=StrToInt(CRobConf::getConf()->GetIcsValid()["receve_vip_double"]);
	}
	//add by v_yfzhu 20121109 ----end 



	//add by v_yilyan 20160224 beg
	theParamIn["apply_day_in"] = IntToStr(del_slot);
	/* MEMO
	switch (StrToInt(info.sIsVip))
	{
	case 0:
		theParamIn["apply_day_in"] = CRobConf::getConf()->GetIcsValid()["receve_double"];
		break;
	case 1:
		theParamIn["apply_day_in"] = CRobConf::getConf()->GetIcsValid()["receve_vip_1"];
		break;
	case 2:
		theParamIn["apply_day_in"] = CRobConf::getConf()->GetIcsValid()["receve_vip_2"];
		break;
	case 3:
		theParamIn["apply_day_in"] = CRobConf::getConf()->GetIcsValid()["receve_vip_3"];
		break;
	default:
		break;
	}
	*/
	//add by v_yilyan 20160224 end


	ValidLOG->WriteLog("[Info]'ProCheckDobleQuest' Proccessing: uin=%s&service_id=%s\n",theParamIn["uin"].c_str(),theParamIn["service_id"].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckDobleQuest' Proccessing: uin=%s&service_id=%s\n",theParamIn["uin"].c_str(),theParamIn["service_id"].c_str());

	ics_log_header header(info.sICSId,info.sUin,info.sServiceID);
	iRet = valid_parse.GetFunction(header,theParamIn,"ics_apply_repeat_uin",base_result); // desc: use value="33811". v_yilyan 20160224
	if(iRet!=0)
	{
		ValidLOG->WriteLog("[Error] [RobRuleValid::ProCheckDobleQuest] Find Uin 'ics_apply_repeat_uin' happened error,not find info\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__, "[Error] [RobRuleValid::ProCheckDobleQuest] Find Uin 'ics_apply_repeat_uin' happened error,not find info\n");

		return STEP_FLITER_RUN_STAT_ERROR_INTERFACE;
	}
	//�߼��ж�
	//1.�����ѯ60���ڵ������=1����ͨ��û���ظ��
	//2.�����ѯ60���ڵ������>1, 
	//  a.����Ǵ˵��ݵ����е����д���һ�����˵�һ�׶ε��ҳɹ����ߴ����е�
	//	  a.1 �������һ���������˵���Ϊ���ظ����ݡ�
	//    a.2 ������е��ݶ��ǵ�һ�׶δ����У���ô,���ǵ�һ������ͨ����
	//	  a.3 ������Ѿ�����ĵ����ˡ�����û��ͨ����һģ�飬�Ҳ����Ƴٵģ����ԡ�ͨ���������Ƴٵģ���ô��Ҳ���Ƴ١�
	time_t _i_nowBookTime=0;
	time_t tCurTime = time(0);
	//string _maxBookTime="0";
	ValidLOG->WriteLog("[Info]'ProCheckDobleQuest' Proccessing base_result.size()=%d\n",base_result.size());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Error] [RobRuleValid::ProCheckDobleQuest] Find Uin 'ics_apply_repeat_uin' happened error,not find info\n");



	/*	a.1	*/
	int check_flag=0;
	int check_running_flag=0;
	int check_false_flag=0;
	int check_result=0;


#ifdef _modified_by_v_yilyan_20160224

	//ɾ��base_result�еĲ���60���ڵĵ���
	int i_site=0;
	for(vector< map<string,string> >::iterator i_vtor=base_result.begin();i_vtor!=base_result.end();){
		string dtBookTime = string((char*)((*i_vtor)["dtBookTime"].c_str()));
		GetTmsecFromUnxfmt(_i_nowBookTime,(char*)(dtBookTime.c_str()));
		ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] ICS booktime[%s],tCurTime[%ld],_i_nowBookTime[%ld]\n",(char*)((*i_vtor)["dtBookTime"].c_str()),tCurTime,_i_nowBookTime);
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] ICS booktime[%s],tCurTime[%ld],_i_nowBookTime[%ld]\n",(char*)((*i_vtor)["dtBookTime"].c_str()),tCurTime,_i_nowBookTime);

		//VIP����ͨ�û�������
		//add by asherzhou 2012-09-10
		//
		//vip:
		//CRobConf::getConf()->Ics_Valid["receve_vip_double"]
		//common:
		//CRobConf::getConf()->Ics_Valid["receve_double"]
		if(tCurTime-_i_nowBookTime>=del_slot*24*60*60){
			ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] Exist The Formor 60 Day's ICS booktime[%s]\n",(char*)((*i_vtor)["dtBookTime"].c_str()));
			WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] Exist The Formor 60 Day's ICS booktime[%s]\n",(char*)((*i_vtor)["dtBookTime"].c_str()));

			base_result.erase(base_result.begin()+i_site);
			i_vtor=base_result.begin()+i_site;
		}else{
			//now the standard
			ValidLOG->WriteLog("[INFO] [function]ProCheckDobleQuest::role_id=%s&now_role_id=%s&\n",(char*)((*i_vtor)["vRobed_RoleId"].c_str()),info.sRoleID.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO] [function]ProCheckDobleQuest::role_id=%s&now_role_id=%s&\n",(char*)((*i_vtor)["vRobed_RoleId"].c_str()),info.sRoleID.c_str());
			if((*i_vtor)["vRobed_RoleId"] == info.sRoleID){	
				ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] RoleId Is Equal\n");
				WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] RoleId Is Equal\n");

				i_site++;
				i_vtor++;
				//dnf2 2012-05-15  2012-05-01�ĵ���
			}else{
				if((*i_vtor)["vRobed_RoleId"]=="0"){
					ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] vRobed_RoleName=%s&info.sRoleName=%s\n",(*i_vtor)["vRobed_RoleName"].c_str(),info.sRoleName.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] vRobed_RoleName=%s&info.sRoleName=%s\n",(*i_vtor)["vRobed_RoleName"].c_str(),info.sRoleName.c_str());

					if((*i_vtor)["vRobed_RoleName"]==info.sRoleName)
					{
						ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Equal\n");
						WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Equal\n");
						i_site++;
						i_vtor++;
						check_flag=1;
					}
					else
					{
						ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Not Equal,i_site=%d\n",i_site);
						WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Not Equal,i_site=%d\n",i_site);
						base_result.erase(base_result.begin()+i_site);
					}
				}
				else
				{
					ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Not Equal,i_site=%d\n",i_site);
					WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] sRoleName Is Not Equal,i_site=%d\n",i_site);
					base_result.erase(base_result.begin()+i_site);
				}
			}
		}
	}
	//dnf2 2012-05-15  2012-05-01�ĵ��� 60���Ҫȥ��
	if(check_flag){
		return STEP_FLITER_RUN_STAT_ERROR_APPLY_AGAIN;
	}

#endif


	if(base_result.size()>1){
		for(int i=0;i<base_result.size();i++){
			ValidLOG->WriteLog("[Info]'ProCheckDobleQuest' Proccessing <%d>&iICSAnaylseResult=<%s>\n",base_result.size(),base_result[i]["iICSAnaylseResult"].c_str());
			WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info]'ProCheckDobleQuest' Proccessing <%d>&iICSAnaylseResult=<%s>\n",base_result.size(),base_result[i]["iICSAnaylseResult"].c_str());

			//a ������ڴ���ɹ���---���˵���Ϊ�쳣���ݡ��ظ����ݡ�
			if(StrToInt(base_result[i]["iICSAnaylseResult"])>=STEP_ITEMFSEND_RUN_RESULT_TURE){
				ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] Get The Fromer icsID Result<%s>, Process Pass Fliter Ok, So Not Pass It\n",base_result[i]["iICSAnaylseResult"].c_str());
				WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] Get The Fromer icsID Result<%s>, Process Pass Fliter Ok, So Not Pass It\n",base_result[i]["iICSAnaylseResult"].c_str());

				return STEP_FLITER_RUN_STAT_ERROR_APPLY_AGAIN;
			}
			//a.1 ��������ڽ׶εĴ���������еĵ���
			if(StrToInt(base_result[i]["iICSAnaylseResult"])>=STEP_FLITER_RUN_RESULT_TURE&&StrToInt(base_result[i]["iICSAnaylseResult"])<STEP_ITEMFSEND_RUN_RESULT_TURE){
				ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] Runing AND False=[%s]\n",base_result[i]["iICSAnaylseResult"].c_str());
				WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] Runing AND False=[%s]\n",base_result[i]["iICSAnaylseResult"].c_str());

				if(StrToInt(base_result[i]["iICSAnaylseResult"])==STEP_ROBSLOT_RUN_RESULT_FALSE||\
					StrToInt(base_result[i]["iICSAnaylseResult"])==STEP_ITEMFLOW_RUN_RESULT_FALSE||\
					StrToInt(base_result[i]["iICSAnaylseResult"])==STEP_ITEMFTRACE_RUN_RESULT_FALSE||\
					StrToInt(base_result[i]["iICSAnaylseResult"])==STEP_ITEMFSEND_RUN_RESULT_FALSE){
						check_false_flag++;
						ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] False=%s&check_false_flag=%d\n",base_result[i]["iICSAnaylseResult"].c_str(),check_false_flag);
						WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] False=%s&check_false_flag=%d\n",base_result[i]["iICSAnaylseResult"].c_str(),check_false_flag);

				}else{
					check_result++;
					ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] Running=%s&check_result=%d\n",base_result[i]["iICSAnaylseResult"].c_str(),check_result);
					WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] Running=%s&check_result=%d\n",base_result[i]["iICSAnaylseResult"].c_str(),check_result);

				}
			}
			//a.2 ���������һ�δ����ĵ���
			if(StrToInt(base_result[i]["iICSAnaylseStat"])==STEP_FLITER_RUNNING_STAT){
				check_running_flag++;
				ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] LOT APPLY=%d",check_running_flag);
				WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] LOT APPLY=%d",check_running_flag);

			}
		}
		//������ڴ����еĵ���
		if(check_result>0){
			return STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT;
		}
		//������еĵ��ݵĶ����ڵ�һ���׶εĵ���
		if(check_running_flag>1){
			ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT,So Pass Last It ,check_flag=<%d>\n",check_running_flag);
			WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT,So Pass Last It ,check_flag=<%d>\n",check_running_flag);

			return STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT;
		}else if(check_running_flag==1){
			ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT,So I am The Last One ,check_flag=<%d>\n",check_running_flag);
			WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT,So I am The Last One ,check_flag=<%d>\n",check_running_flag);

			return 0;
		}
		return 0;//������� ��������
	}else if(base_result.size()==1){
		ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] The Fisrt Processing Only One, So Pass It\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] The Fisrt Processing Only One, So Pass It\n");

		return 0;
	}else{
		ValidLOG->WriteLog("[Info] [RobRuleValid::ProCheckDobleQuest] The <ics_apply_repeat_uin> Processing Interface Error\n");
		WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[Info] [RobRuleValid::ProCheckDobleQuest] The <ics_apply_repeat_uin> Processing Interface Error\n");

		return STEP_FLITER_RUN_STAT_ERROR_INTERFACE;
	}
}


/*=======================V1.00=================================*/


/**
Description:	��֤���ݵĺϷ���
@param	[in]	info
@return			int
@exception		none
*/
int RobRuleValid::DoICSCheck(ics_info& info)
{
	ValidLOG->WriteLog("In Valid Proccess,IcsInfo==|ICSID:%s|UserID:%s|\n",info.ICSID.c_str(),info.UserID.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBANLYSVALID , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "In Valid Proccess,IcsInfo==|ICSID:%s|UserID:%s|\n",info.ICSID.c_str(),info.UserID.c_str());

	/*if (0 != CheckRoleLevel(info))
	{
	// �û��ȼ������Ϸ���Ҫ��
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRoleLevel' is �û��ȼ������Ϸ���Ҫ��\n");
	return STEP_FLITER_RUN_STAT_ERROR_LEVEL;
	}
	else if (0 != CheckRobedTime(info))
	{
	// ����ʱ�䳬������Ҫ��
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is ����ʱ�䳬������Ҫ��\n");
	return STEP_FLITER_RUN_STAT_ERROR_APPLY_TIME;
	}
	else if (0 != CheckLastTime(info))
	{
	// ���ֱ���ʱ�䲻�������һ�ε�½ʱ��
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is  ���ֱ���ʱ�䲻�������һ�ε�½ʱ��\n");
	return STEP_FLITER_RUN_STAT_ERROR_INPUT_TIME;
	}
	else if (0 != CheckDobleQuest(info))
	{
	// 60�����ظ�����
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is   60�����ظ�����\n");
	return STEP_FLITER_RUN_STAT_ERROR_APPLY_AGAIN;
	}
	else if (0 != CheckBlockTime(info))
	{
	// ��ɫ�������5��
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'CheckRobedTime' is  ��ɫ�������5��\n");
	return STEP_FLITER_RUN_STAT_ERROR_BANNED;
	}
	ValidLOG->WriteLog("In Valid Proccess, Find Uin 'DoICSCheck' is  valid ,Go to Auto Process\n");
	// ������������*/
	return STEP_FLITER_RUN_STAT_SUCCESS;
}






