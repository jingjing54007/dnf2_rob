/**
@file rob_core_anaylse.cpp

@brief 
	---------------------------------------------------------
	@ 盗号物品追踪系统---可选的流程选项
	@ 1、如果不需要追踪，需要把物品流水系统更新到追踪表中
	@ 2、如果需要追踪，a: 追寻所有可能的物品转移日志
					b: 查找第一手和最后一手的日志确定“盗号者”和“拥有者”
	@ 3、记录相关的第一手和最后一手日志到物品跟踪表中。
		 注意：这里的目的角色和帐号，必须要填写，至于如何填写，依据流向动
		 作来做，eg. P2P交易的可以确定对方的角色和帐号 ，销毁和消失，记录
		 成NPC，或0，或自定义编码
	@ 4、更新相关的物品流向日志的表状态和以及ICS单据表状态
	@ 5、更新相关的物品跟踪表的处理状态
	---------------------------------------------------------
history:
<PRE>
History ID	:	1.01
Author    	:	asherzhou
DateTime	:	2011/05/01
Description :	Create
</PRE>
*/
#include <iostream>
#include "rob_ics_oper.h"
#include "rob_rule_logic.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"
#include "comm_strings.h"
#include "comm_datetime.h"
int main(int argc, char** argv)
{
	//调试日志
	CLog* ItemTraceLog;
	string sItemTrace = "./logs/"+GetCurDateDay()+"RobItemTrace.log";
	ItemTraceLog = new CLog((char*)sItemTrace.c_str());
	ItemTraceLog->LogOn();
	ItemTraceLog->WriteLog("===============Start GetRobItemTraceList============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemTraceList============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	
	//物品统计
	string sUpdateType="Item";
	string sUpdateNum="0";
	int iUpdateItemNum=0;
	int iUpdateItemHighLevelNum=0;
	long long llUpdateMoney = 0;
	vector <string> addStatNum;
	//是否需要追踪的标志 0 无需追踪 1 需要追踪
	int iTraceProcFlag = 1;

#ifdef dnf2
		iTraceProcFlag = 0;
#endif

	int iRet = 0;
	int sizeOfItemTrace = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_four"]);
	if(sizeOfItemTrace<=0)
	{
		ItemTraceLog->WriteLog("Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");

		return -110;
	}
	/********************************************************************************
	@1.读取处于等待物品跟踪处理的阶段的单据。
	*/
	vector < Item_Flow_List > robItemFlowInfo;
	vector < Item_Trace_List > robItemTrace;
	Rob_Ics_Complaint getICSItemTraceList[sizeOfItemTrace];
	

	int iSize = icsOper.ReadICSInfoList(STEP_ITEMTRACE_RUN_ICS,getICSItemTraceList,sizeOfItemTrace);
	if(iSize > 0 && iSize <= sizeOfItemTrace)
	{
		ItemTraceLog->WriteLog("ReadICSInfoList GET sizeOfItemTrace size:%d and less than sizeOfItemTrace Successfully\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemTrace size:%d and less than sizeOfItemTrace Successfully\n",iSize);

	}
	else if(iSize > sizeOfItemTrace)
	{
		ItemTraceLog->WriteLog("Warning:::ReadICSInfoList GET sizeOfItemTrace size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemTrace:%d\n",iSize,sizeOfItemTrace);
		WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Warning:::ReadICSInfoList GET sizeOfItemTrace size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemTrace:%d\n",iSize,sizeOfItemTrace);

		return -111;
	}
	else if(iSize == 0)
	{
		ItemTraceLog->WriteLog("ReadICSInfoList GET sizeOfItemTrace size 0 and No Info to Proccess,So Return\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemTrace size 0 and No Info to Proccess,So Return\n");

		return 0;
	}
	else
	{
		ItemTraceLog->WriteLog("ReadICSInfoList GET sizeOfItemTrace size:%d Happend Error\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemTrace size:%d Happend Error\n",iSize);

		return -112;
	}
	/********************************************************************************
	@2.更改单据处理状态。为物品跟踪处理中……
	@  STEP_ITEMTRACE_RUN_ICS  STEP_ITEMFTRACE_RUNNING_RESULT  STEP_ITEMTRACE_RUNNING_STAT
	*/
	ItemTraceLog->WriteLog("----2.单据更新状态STEP_ITEMTRACE_RUN_ICS处理中\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----2.单据更新状态STEP_ITEMTRACE_RUN_ICS处理中\n");

	for(int i=0;i<iSize;i++){
		iRet = icsOper.UpdateICSProc(getICSItemTraceList[i].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUNNING_RESULT,STEP_ITEMTRACE_RUNNING_STAT);
		if(iRet==0){
			ItemTraceLog->WriteLog("2.1单据【%s】更新状态STEP_ITEMTRACE_RUN_ICS成功\n",getICSItemTraceList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[i].sICSId.c_str(), getICSItemTraceList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1单据【%s】更新状态STEP_ITEMTRACE_RUN_ICS成功\n",getICSItemTraceList[i].sICSId.c_str());

		}else{
			ItemTraceLog->WriteLog("2.1单据【%s】更新状态STEP_ITEMTRACE_RUN_ICS失败\n",getICSItemTraceList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[i].sICSId.c_str(), getICSItemTraceList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1单据【%s】更新状态STEP_ITEMTRACE_RUN_ICS失败\n",getICSItemTraceList[i].sICSId.c_str());

		}
	}
	ItemTraceLog->WriteLog("----3.单据丢失物品流水读取\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.单据丢失物品流水读取\n");

	for(int k=0;k<iSize;k++){
			ItemTraceLog->WriteLog("3.1.0单据【%s】读取流水开始\n",getICSItemTraceList[k].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.0单据【%s】读取流水开始\n",getICSItemTraceList[k].sICSId.c_str());

			iRet=icsOper.ReadICSItemListProc(robItemFlowInfo,getICSItemTraceList[k].sICSId);
			ItemTraceLog->WriteLog("3.1.0单据【%s】读取流水大小[%d]\n",getICSItemTraceList[k].sICSId.c_str(),robItemFlowInfo.size());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.0单据【%s】读取流水大小[%d]\n",getICSItemTraceList[k].sICSId.c_str(),robItemFlowInfo.size());

			if(iRet==STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS){
				//更新单据ICS状态
				iRet = icsOper.UpdateICSProc(getICSItemTraceList[k].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUN_RESULT_FALSE,STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.1.1单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS成功\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS成功\n",getICSItemTraceList[k].sICSId.c_str());

				}else{
					ItemTraceLog->WriteLog("3.1.1单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS失败\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS失败\n",getICSItemTraceList[k].sICSId.c_str());

				}
				continue;
			}else if(iRet==0){
				//追踪物品
				//如果需要追踪物品，就要追踪，并更新ItemTrace内存数组
				if(iTraceProcFlag){
					//这里省略
					//1.读取ItemFlow
					//2.按照物品ID的唯一条件来做robItemTrace的表的赋值

				}else{
					//3.1.2.1更新单据ItemFlow到ItemTrace表中
					iRet=icsRobRuleLogic.getRobItemTraceNoTracking(robItemTrace,robItemFlowInfo);
					if(iRet==0){
						ItemTraceLog->WriteLog("3.1.2.1单据【%s】无须追踪，获取玩家的物品流水追踪成功\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.1单据【%s】无须追踪，获取玩家的物品流水追踪成功\n",getICSItemTraceList[k].sICSId.c_str());

					}else{
						ItemTraceLog->WriteLog("3.1.2.1单据【%s】无须追踪，获取玩家的物品流水追踪失败\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.1单据【%s】无须追踪，获取玩家的物品流水追踪失败\n",getICSItemTraceList[k].sICSId.c_str());

					}
					//3.1.2.1把ItemFlow的单据系统状态更新
					iRet=icsOper.UpdateICSItemFlow(getICSItemTraceList[k].sICSId,getICSItemTraceList[k].sServiceID,STEP_ItemFlow_RUN_OVER_TRACING,STEP_ItemFlow_NONEED_TRACE_RESULT);
					if(iRet==0){
						ItemTraceLog->WriteLog("3.1.2.2单据【%s】无须追踪，更新玩家的物品流水状态STEP_ItemFlow_RUN_OVER_TRACING成功\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.2单据【%s】无须追踪，更新玩家的物品流水状态STEP_ItemFlow_RUN_OVER_TRACING成功\n",getICSItemTraceList[k].sICSId.c_str());

					}else{
						ItemTraceLog->WriteLog("3.1.2.2单据【%s】无须追踪，更新玩家的物品流水状态STEP_ItemFlow_RUN_OVER_TRACING失败\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.2单据【%s】无须追踪，更新玩家的物品流水状态STEP_ItemFlow_RUN_OVER_TRACING失败\n",getICSItemTraceList[k].sICSId.c_str());

					}
		
				}
				//3.1.3把ItemTrace的单据系统插入到ItemTrace表中，包括处理状态
				iRet = icsOper.InsertICSItemTrace(robItemTrace);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.1.3单据【%s】追踪完成，获取玩家的物品追踪数据成功\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3单据【%s】追踪完成，获取玩家的物品追踪数据成功\n",getICSItemTraceList[k].sICSId.c_str());
				}else{
					ItemTraceLog->WriteLog("3.1.3单据【%s】追踪完成，获取玩家的物品追踪数据失败\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3单据【%s】追踪完成，获取玩家的物品追踪数据失败\n",getICSItemTraceList[k].sICSId.c_str());

				}
				//3.2更新单据ICS状态
				iRet = icsOper.UpdateICSProc(getICSItemTraceList[k].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUN_RESULT_TURE,STEP_ITEMTRACE_RUN_STAT_SUCCESS);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.2单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_SUCCESS成功\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.2单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_SUCCESS成功\n",getICSItemTraceList[k].sICSId.c_str());

				}else{
					ItemTraceLog->WriteLog("3.2单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_SUCCESS失败\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.2单据【%s】更新状态STEP_ITEMTRACE_RUN_STAT_SUCCESS失败\n",getICSItemTraceList[k].sICSId.c_str());
				}

			}
	}
	return 0;
}
