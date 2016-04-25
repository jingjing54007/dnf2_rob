/**
@file rob_core_anaylse.cpp

@brief 

	
	---------------------------------------------------------
	盗号自助分析功能
	
	分析的单据必须是单据合法的；
	处理过程如下，
	1) 获取要分析的单据；
	2) 单据过滤系统; 
	3) 单据状态更新;
	---------------------------------------------------------

@details
Copyright (c) 2012 Tencent. All rights reserved.

@author		asherzhou
@version	1.0
@date		2011/02/13
</PRE>
*/
#include <iostream>
#include "rob_anlys_valid.h"
#include "rob_rule_logic.h"
#include "rob_ics_oper.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "rob_anlys_ip.h"
#include "comm_datetime.h"
#include "comm_strings.h"
#include "conf.h"


int main(int argc, char** argv)
{
	//调试日志
	CLog* ICSFliterLOG;
	
	//基础模块类
	RobICSOper icsOper;
	RobRuleValid icsCheckValid;
	RobRuleLogic icsRobRuleLogic;
	int iRet = 0;

	//调试日志的初始化
	GetCurDateDay();
	std::string sICSFliterLOG="./logs/"+GetCurDateDay()+"RobICSFliter.log";
	ICSFliterLOG = new CLog((const char*)sICSFliterLOG.c_str());
	ICSFliterLOG->LogOn();
	
	
	ICSFliterLOG->WriteLog("------------------Start RobICSFliter-----------------\n");
	WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "------------------Start RobICSFliter-----------------\n");

#ifdef dnf2
	ICSFliterLOG->WriteLog("------------------In The [dnf2] Prcosess-----------------\n");
	WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"------------------In The [dnf2] Prcosess-----------------\n");

#endif
	int SIZE_OF_ICS = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_one"]);
	ICSFliterLOG->WriteLog("Read ICS Count is %d\n",SIZE_OF_ICS);
	WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"Read ICS Count is %d\n",SIZE_OF_ICS);

	
	/*****************************************************************************************
	@   A: 内存储存，Rob_Ics_Complaint
	@	1.读取SIZE_OF_ICS的单据信息，存在Ics_Info的数组
	*/
	Rob_Ics_Complaint Ics_Info[SIZE_OF_ICS];

	map<string,string> theParamIn;
	vector < map<string,string> > icsResult;
	
	
	// int size_of_ics_info = icsOper.ReadICSInfoList(STEP_FLITER_RUN_ICS,Ics_Info,SIZE_OF_ICS);
	// modified by v_yilyan 20160225 beg
	int size_of_ics_info = 0;
	if (SIZE_OF_ICS == 1){ // 2.1读取SIZE_OF_ICS的单据信息，SIZE等于1，读取指定单据
		string ics_id = CRobConf::getConf()->GetIcsOper()["ics_id_send_only_one"];//单据id
		ics_id.erase(remove_if(ics_id.begin(),ics_id.end(), bind2nd(equal_to<char>(), '\t')), ics_id.end());//移除所有空格、制表符等
		if (ics_id.empty()){
			ICSFliterLOG->WriteLog("读取指定的单据ID：【ics_id_send_only_one】设为空，请检查！\n");// ics_id_send_only_one
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "读取指定的单据ID：【ics_id_send_only_one】设为空，请检查！\n");
		}
		size_of_ics_info = icsOper.ReadICSInfoOneItem(STEP_FLITER_RUN_ICS, Ics_Info, ics_id);
	}else{ // 2.2读取SIZE_OF_ICS的单据信息，SIZE大于1，存在Ics_Info的数组
		size_of_ics_info = icsOper.ReadICSInfoList(STEP_FLITER_RUN_ICS, Ics_Info, SIZE_OF_ICS);
	}
	// modified by v_yilyan 20160225 end

	if(size_of_ics_info >= 0 && size_of_ics_info <= SIZE_OF_ICS){
		ICSFliterLOG->WriteLog("ReadICSInfo GET ics_info size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"ReadICSInfo GET ics_info size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);

		//2.更新单据状态为处理中
		for(int j=0;j<size_of_ics_info;j++){
			iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUNNING_RESULT,STEP_FLITER_RUNNING_STAT);
			ICSFliterLOG->WriteLog("UpdateICSProc iRet:%d\n",iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[j].sID.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"UpdateICSProc iRet:%d\n",iRet);

			if(iRet!=0){
				//这里需要邮件报警处理！后续版本控制
				ICSFliterLOG->WriteLog("UpdateICSProc Happend Error-iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBICSFILTER ,Ics_Info[j].sID.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"UpdateICSProc Happend Error-iRet:%d\n",iRet);
			}
		}
	}else{
		ICSFliterLOG->WriteLog("ReadICSInfo GET ics_info size:%d Happend Error\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER ,"", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"ReadICSInfo GET ics_info size:%d Happend Error\n",size_of_ics_info);

		return -1;
	}
	/*****************************************************************************************
	@   B: 利用读取的信息进行相关过滤系统给的判断，Rob_Ics_Complaint
	@	1.循环判断和读取
	*/
	for(int i=0;i<size_of_ics_info;i++){
		iRet = icsCheckValid.DoICSCheckPro(Ics_Info[i]);
		ICSFliterLOG->WriteLog("icsCheckValid.DoICSCheck iRetResult:%d\n",iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"icsCheckValid.DoICSCheck iRetResult:%d\n",iRet);

		if((ApplyAnaylseResult)iRet==STEP_FLITER_RUN_STAT_SUCCESS){
			ICSFliterLOG->WriteLog("单据【%s】验证正确!!!\n",Ics_Info[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】验证正确!!!\n",Ics_Info[i].sICSId.c_str());

			icsOper.UpdateICSProc(Ics_Info[i].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUN_RESULT_TURE,(ApplyAnaylseStat)iRet);
		}else{
			ICSFliterLOG->WriteLog("单据【%s】验证失败!!!\n",Ics_Info[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】验证失败!!!\n",Ics_Info[i].sICSId.c_str());

			icsOper.UpdateICSProc(Ics_Info[i].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUN_RESULT_FALSE,(ApplyAnaylseStat)iRet);
		}
		// 2.统计库的插入
		iRet = icsOper.AddICSItemStatsProc(Ics_Info[i].sICSId,Ics_Info[i].sServiceID);
		if(iRet!=0){
			ICSFliterLOG->WriteLog("单据【%s】统计库的插入失败!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】统计库的插入失败!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		}
		ICSFliterLOG->WriteLog("单据【%s】统计库的插入成功!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】统计库的插入成功!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		// 3.角色基本信息更新
		iRet = icsOper.UpdateICSRoleInfo(Ics_Info[i]);
		if(iRet!=0){
			ICSFliterLOG->WriteLog("单据【%s】角色基本信息更新失败!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】角色基本信息更新失败!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		}
		ICSFliterLOG->WriteLog("单据【%s】角色基本信息更新成功!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"单据【%s】角色基本信息更新成功!!!返回码：%d\n",Ics_Info[i].sICSId.c_str(),iRet);

	}
	delete ICSFliterLOG;
	return 0;

}
