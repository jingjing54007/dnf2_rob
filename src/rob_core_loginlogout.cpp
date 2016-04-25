/**
@file rob_core_anaylse.cpp

@brief 

	---------------------------------------------------------
	盗号自助分析功能(IP异常分析、赃物分析和流水分析)
	
	分析的单据必须是单据合法的；是盗号处理过程中，删除赠送物品的前提
	盗号自助功能输入参数： ICS单号
	处理过程如下，1) 获取要分析的单据；
	1）等级是否符合 2)时间控制判断
	---------------------------------------------------------

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		darrenqiu
@version	1.0
@date		2010/10/29

history:
<PRE>
History ID	:	1
Author    	:	asherzhou
DateTime	:	2011/05/01
Description :	Create
</PRE>
*/
#include <iostream>
#include "rob_anlys_ip.h"
#include "rob_ics_oper.h"
#include "rob_anlys_valid.h"
#include "rob_rule_logic.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"
#include "comm_strings.h"
int main(int argc, char** argv)
{
	

	//调试日志
	CLog* ICSLogOutLOG;
	
	//基础模块类
	RobICSOper icsOper;
	RobAnlysIP icsRobAnlysIP;
	RobRuleLogic icsRobLogic;
	int iRet = 0;

	//调试日志的初始化
	std::string sICSLogOutLOG="./logs/"+GetCurDateDay()+"RobLogOutFliter.log";
	ICSLogOutLOG = new CLog((const char*)sICSLogOutLOG.c_str());
	ICSLogOutLOG->LogOn();
	
	ICSLogOutLOG->WriteLog("Start RobLogOutFliter------------------------------------\n");
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Start RobLogOutFliter------------------------------------\n");

#ifdef dnf2
	ICSLogOutLOG->WriteLog("======The [dnf2] Prcosess======\n");
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "======The [dnf2] Prcosess======\n");

#endif

	int SIZE_OF_ICS = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_two"]);
	ICSLogOutLOG->WriteLog("Read ICS LogOut Count is %d\n",SIZE_OF_ICS);
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Read ICS LogOut Count is %d\n",SIZE_OF_ICS);
	
	/*****************************************************************************************
	@  A:内存读取，Rob_Ics_Complaint 中需要进一步进行盗号时间段查询的
	@  1.读取SIZE_OF_ICS的单据信息，存在Rob_Ics_Complaint的数组
	*/
	Rob_Ics_Complaint Ics_Info[SIZE_OF_ICS];
	map<string,string> theParamIn;
	vector < map<string,string> > icsResult;

	int size_of_ics_info = icsOper.ReadICSInfoList(STEP_ROBSLOT_RUN_ICS,Ics_Info,SIZE_OF_ICS);
	if(size_of_ics_info >= 0 && size_of_ics_info <= SIZE_OF_ICS){
		ICSLogOutLOG->WriteLog("1> 读取单据结果： ICS size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "1> 读取单据结果： ICS size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);

		vector< Rob_SLOT > robSlotList;
		for(int k=0;k<size_of_ics_info;k++){
			//2.更新单据状态为处理中
			iRet = icsOper.UpdateICSProc(Ics_Info[k].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUNNING_RESULT,STEP_ROBSLOT_RUNNING_STAT);
			ICSLogOutLOG->WriteLog("2> 更新单据成功： UpdateICSProc  LogOut iRet:%d\n",iRet);
			WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2> 更新单据成功： UpdateICSProc  LogOut iRet:%d\n",iRet);
			if(iRet!=0){
				//这里需要邮件报警处理！【后续版本控制---可停可跑】
				ICSLogOutLOG->WriteLog("2.1> 更新单据失败： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1> 更新单据失败： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

			}else{
				ICSLogOutLOG->WriteLog("2.1> 更新单据成功： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1> 更新单据成功： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);

			}
		}
		for(int j=0;j<size_of_ics_info;j++){
			robSlotList.clear();
			//3.进行玩家数据的异常登录时间段
			ICSLogOutLOG->WriteLog("3> 玩家的登录登出日志：sICSId:【%s】\n",Ics_Info[j].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3> 玩家的登录登出日志：sICSId:【%s】\n",Ics_Info[j].sICSId.c_str());

			iRet = icsRobAnlysIP.getRobLogInOutLog(Ics_Info[j],robSlotList);
			if(iRet != 0){
				//4.1更新玩家不符合盗号分析时间段的内容
				iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUN_RESULT_FALSE,(ApplyAnaylseStat)iRet);
				ICSLogOutLOG->WriteLog("4> 更新单据状态为登录登出《无法分析》出异常盗号时段： UpdateICSProc  LogOut iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4> 更新单据状态为登录登出《无法分析》出异常盗号时段： UpdateICSProc  LogOut iRet:%d\n",iRet);

				if(iRet!=0){
					//这里需要邮件报警处理！【后续版本控制---可停可跑】
					ICSLogOutLOG->WriteLog("4.1> 更新单据状态为登录登出《无法分析》出异常盗号时段接口返回《失败》： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.1> 更新单据状态为登录登出《无法分析》出异常盗号时段接口返回《失败》： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

				}else{
					//这里需要邮件报警处理！【后续版本控制---可停可跑】
					ICSLogOutLOG->WriteLog("4.1> 更新单据状态为登录登出《无法分析》出异常盗号时段接口返回《成功》： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.1> 更新单据状态为登录登出《无法分析》出异常盗号时段接口返回《成功》： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
				}
				continue;
			}else{
				//4.2更新玩家符合盗号分析时间段的内容，同时把数据插入tbICSRoleLogout异常登录表中
				iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUN_RESULT_TURE,STEP_ROBSLOT_RUN_STAT_SUCCESS);
				ICSLogOutLOG->WriteLog("4> 更新单据状态为登录登出《已分析》出异常盗号时段： UpdateICSProc  LogOut iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4> 更新单据状态为登录登出《已分析》出异常盗号时段： UpdateICSProc  LogOut iRet:%d\n",iRet);

				if(iRet!=0){
					//这里需要邮件报警处理！【后续版本控制---可停可跑】
					ICSLogOutLOG->WriteLog("4.2> 更新单据状态为登录登出《已分析》出异常盗号时段接口返回《失败》： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
				   WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.2> 更新单据状态为登录登出《已分析》出异常盗号时段接口返回《失败》： UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

				}else{
					//这里需要邮件报警处理！【后续版本控制---可停可跑】
					ICSLogOutLOG->WriteLog("4.2> 更新单据状态为登录登出《已分析》出异常盗号时段接口返回《成功》： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.2> 更新单据状态为登录登出《已分析》出异常盗号时段接口返回《成功》： UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);

					//5.插入玩家的异常登录数据
					ICSLogOutLOG->WriteLog("5> 插入单据登录登出异常盗号时段\n");
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5> 插入单据登录登出异常盗号时段\n");

					for(int k=0 ;k<robSlotList.size();k++){
						if(robSlotList[k].iRobFlag=="1"){
							sleep(1);
							//5.1获取玩家在盗号时间段内的丢失金钱数，跟新在iMoney_Sub中
							iRet = icsRobLogic.getRobSlotMoneySub(robSlotList[k]);
							if(iRet!=0){
								ICSLogOutLOG->WriteLog("5.1>获取【%d】玩家【%s】金钱减少量查询《失败》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.1>获取【%d】玩家【%s】金钱减少量查询《失败》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);

							}else{
								ICSLogOutLOG->WriteLog("5.1>获取【%d】玩家【%s】金钱减少量查询《成功》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.1>获取【%d】玩家【%s】金钱减少量查询《成功》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);

								iRet = icsOper.InsertICSRobSlotData(robSlotList[k]);
								if(iRet!=0){
									ICSLogOutLOG->WriteLog("5.2> 插入单据【%d】单据号【%s】登录登出异常盗号时《操作失败》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
									WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.2> 插入单据【%d】单据号【%s】登录登出异常盗号时《操作失败》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								}else{
									ICSLogOutLOG->WriteLog("5.2> 插入单据【%d】单据号【%s】登录登出异常盗号时《操作成功》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
									WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.2> 插入单据【%d】单据号【%s】登录登出异常盗号时《操作成功》iRet：%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								}
							}
						}else{
							continue;
						}
					}
				}
				
			}
		}
	}else{
		ICSLogOutLOG->WriteLog("--->>>获取处理单据ReadICSInfoList size:【%d】，可能没有单据处理或者及接口错误\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "--->>>获取处理单据ReadICSInfoList size:【%d】，可能没有单据处理或者及接口错误\n",size_of_ics_info);

		return 0;
	}


	return 0;
}
