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
#include "rob_ics_oper.h"
#include "rob_rule_logic.h"
#include "comm_datetime.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"

int main(int argc, char** argv)
{
	//调试日志
	CLog* ItemFlowLog;
	string sItemFlowLog = "./logs/"+GetCurDateDay()+"RobItemFlow.log";
	ItemFlowLog = new CLog((char*)sItemFlowLog.c_str());
	ItemFlowLog->LogOn();
	ItemFlowLog->WriteLog("===============Start GetRobItemFlowList============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemFlowList============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	//物品统计
	string sUpdateType="Item";
	string sUpdateNum="0";
	int iUpdateItemNum=0;
	int iUpdateItemHighLevelNum=0;
	int iUpdateRetrieveNum=0;
	long long llUpdateMoney = 0;
	vector <string> addStatNum;
	;

	int iRet = 0;
	int sizeOfItemFlow = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_three"]);
	if(sizeOfItemFlow<=0)
	{
		ItemFlowLog->WriteLog("Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");

		return -110;
	}
	/********************************************************************************
	@1.读取处于等待物品流水处理的阶段的单据。
	*/
	vector < Rob_SLOT > robSlotInfo;
	Rob_Ics_Complaint getICSItemProcList[sizeOfItemFlow];
	vector< Item_Flow_List > robItemFlow;

	int iSize = 0;

	if (sizeOfItemFlow == 1)//单读size设为1时，则读取指定单据
	{
		string ics_id = CRobConf::getConf()->GetIcsOper()["ics_id_send_only_one"];//单据id
		ics_id.erase(remove_if(ics_id.begin(),ics_id.end(), bind2nd(equal_to<char>(), '\t')), ics_id.end());//移除所有空格、制表符等
		if (ics_id.empty())
		{
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW, "", "",TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "读取指定的单据ID：【ics_id_only_one】设为空，请检查！\n" );
		}
		iSize = icsOper.ReadICSInfoOneItem(STEP_ITEMFLOW_RUN_ICS,getICSItemProcList,ics_id);
	}else
	{
		iSize = icsOper.ReadICSInfoList(STEP_ITEMFLOW_RUN_ICS,getICSItemProcList,sizeOfItemFlow);
	}

	if(iSize > 0 && iSize <= sizeOfItemFlow)
	{
		ItemFlowLog->WriteLog("ReadICSInfoList GET sizeOfItemFlow size:%d and less than sizeOfItemFlow Successfully\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemFlow size:%d and less than sizeOfItemFlow Successfully\n",iSize);

	}
	else if(iSize > sizeOfItemFlow)
	{
		ItemFlowLog->WriteLog("Warning:::ReadICSInfoList GET sizeOfItemFlow size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemFlow:%d\n",iSize,sizeOfItemFlow);
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Warning:::ReadICSInfoList GET sizeOfItemFlow size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemFlow:%d\n",iSize,sizeOfItemFlow);

		return -111;
	}
	else if(iSize == 0)
	{
		ItemFlowLog->WriteLog("ReadICSInfoList GET sizeOfItemFlow size 0 and No Info to Proccess,So Return\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemFlow size 0 and No Info to Proccess,So Return\n");

		return 0;
	}
	else
	{
		ItemFlowLog->WriteLog("ReadICSInfoList GET sizeOfItemFlow size:%d Happend Error\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ReadICSInfoList GET sizeOfItemFlow size:%d Happend Error\n",iSize);

		return -112;
	}
	/********************************************************************************
	@2.更改单据处理状态。为处理中……
	@  STEP_ITEMFLOW_RUN_ICS  STEP_ITEMFLOW_RUNNING_RESULT  STEP_ITEMFLOW_RUNNING_STAT
	*/
	ItemFlowLog->WriteLog("----2.单据更新状态STEP_ITEMFLOW_RUN_ICS处理中\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----2.单据更新状态STEP_ITEMFLOW_RUN_ICS处理中\n");

	for(int i=0;i<iSize;i++){
		iRet = icsOper.UpdateICSProc(getICSItemProcList[i].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUNNING_RESULT,STEP_ITEMFLOW_RUNNING_STAT);
		if(iRet==0){
			ItemFlowLog->WriteLog("2.1单据【%s】更新状态STEP_ITEMFLOW_RUN_ICS成功\n",getICSItemProcList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[i].sICSId.c_str(), getICSItemProcList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1单据【%s】更新状态STEP_ITEMFLOW_RUN_ICS成功\n",getICSItemProcList[i].sICSId.c_str());

		}else{
			ItemFlowLog->WriteLog("2.1单据【%s】更新状态STEP_ITEMFLOW_RUN_ICS失败\n",getICSItemProcList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[i].sICSId.c_str(), getICSItemProcList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1单据【%s】更新状态STEP_ITEMFLOW_RUN_ICS失败\n",getICSItemProcList[i].sICSId.c_str());

		}
	}
	/********************************************************************************
	@3.读取对应单据的异常登录信息……
	*/
	for(int j=0;j<iSize;j++)
	{
		ItemFlowLog->WriteLog("----3.第【%d】个单号【%s】进入读取异常时间段的处理程0000000\n",j,getICSItemProcList[j].sICSId.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.第【%d】个单号【%s】进入读取异常时间段的处理程0000000\n",j,getICSItemProcList[j].sICSId.c_str());

		iRet=icsOper.ReadICSRobSlot(robSlotInfo,getICSItemProcList[j].sServiceID,getICSItemProcList[j].sICSId);
		if(iRet==-10000){
			//出现这种情况几率较小，可以不考虑
			continue;
		}else{
			/********************************************************************************
			@3.1 利用对应的时间段来进行玩家丢失物品的查询
			*/
			ItemFlowLog->WriteLog("----3.1.0第【%d】个单号【%s】进入读取异常时间段的处理程大小【%d】\n",j,getICSItemProcList[j].sICSId.c_str(),robSlotInfo.size());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.1.0第【%d】个单号【%s】进入读取异常时间段的处理程大小【%d】\n",j,getICSItemProcList[j].sICSId.c_str(),robSlotInfo.size());

			iRet = icsRobRuleLogic.getRoleItemFlowList(robItemFlow,robSlotInfo,getICSItemProcList[j].sIsVip);
			if(iRet==STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW){
				//3.1.1 更新单据为异常处理完成
				iRet = icsOper.UpdateICSProc(getICSItemProcList[j].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUN_RESULT_FALSE,STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.1单据【%s】更新状态STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW成功\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1单据【%s】更新状态STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW成功\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.1单据【%s】更新状态STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW失败\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1单据【%s】更新状态STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW失败\n",getICSItemProcList[j].sICSId.c_str());

				}
			}else if(iRet==0){
				ItemFlowLog->WriteLog("----3.1.2第【%d】个单号【%s】进入读取物品流水大小【%d】\n",j,getICSItemProcList[j].sICSId.c_str(),robItemFlow.size());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.1.2第【%d】个单号【%s】进入读取物品流水大小【%d】\n",j,getICSItemProcList[j].sICSId.c_str(),robItemFlow.size());

				//3.1.2 更新单据进入tbICSItemFlow表中
				iRet = icsOper.InsertICSRobItemFlowData(robItemFlow,getICSItemProcList[j].sServiceID);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.2单据【%s】插入tbICSItemFlow《成功》\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2单据【%s】插入tbICSItemFlow《成功》\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.2单据【%s】插入tbICSItemFlow《失败》\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2单据【%s】插入tbICSItemFlow《失败》\n",getICSItemProcList[j].sICSId.c_str());

				}
				//3.1.3 更新单据表tbICSComplaint状态
				iRet = icsOper.UpdateICSProc(getICSItemProcList[j].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUN_RESULT_TURE,STEP_ITEMFLOW_RUN_STAT_SUCCESS);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.3-1单据【%s】更新单据表tbICSComplaint状态STEP_ITEMFLOW_RUN_STAT_SUCCESS成功\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3-1单据【%s】更新单据表tbICSComplaint状态STEP_ITEMFLOW_RUN_STAT_SUCCESS成功\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.3-1单据【%s】更新单据表tbICSComplaint状态STEP_ITEMFLOW_RUN_STAT_SUCCESS失败\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3-1单据【%s】更新单据表tbICSComplaint状态STEP_ITEMFLOW_RUN_STAT_SUCCESS失败\n",getICSItemProcList[j].sICSId.c_str());

				}
			}
			//3.1.4 更新单据表tbICSRoleLogout状态
			iRet = icsOper.UpdateICSRobSlotProc(getICSItemProcList[j].sServiceID,getICSItemProcList[j].sICSId,STEP_SlotStat_RUN_OVER_STAT);
			if(iRet==0){
				ItemFlowLog->WriteLog("3.1.4单据【%s】更新单据表tbICSRoleLogout状态STEP_ITEMFLOW_RUN_STAT_SUCCESS成功\n",getICSItemProcList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.4单据【%s】更新单据表tbICSRoleLogout状态STEP_ITEMFLOW_RUN_STAT_SUCCESS成功\n",getICSItemProcList[j].sICSId.c_str());

			}else{
				ItemFlowLog->WriteLog("3.1.4单据【%s】更新单据表tbICSRoleLogout状态STEP_ITEMFLOW_RUN_STAT_SUCCESS失败\n",getICSItemProcList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.4单据【%s】更新单据表tbICSRoleLogout状态STEP_ITEMFLOW_RUN_STAT_SUCCESS失败\n",getICSItemProcList[j].sICSId.c_str());

			}
			//3.1.5 更新物品统计表tbICSItemStats状态
			iUpdateItemNum=0;
			iUpdateRetrieveNum=0;
			iUpdateItemHighLevelNum=0;
			llUpdateMoney = 0;
			addStatNum.clear();
			//丢失道具总数
			iUpdateItemNum=robItemFlow.size();
			ItemFlowLog->WriteLog("[INFO]: iUpdateItemNum=%d\n",iUpdateItemNum);
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]: iUpdateItemNum=%d\n",iUpdateItemNum);

			for(int t=0;t<robItemFlow.size();t++){
				if(robItemFlow[t].iDefineProc=="1"||robItemFlow[t].iDefineProc=="2"){
					//需要找回的物品
					iUpdateRetrieveNum += 1;
					if(robItemFlow[t].iDefineProc=="2"){
						//高等级物品，人工处理的物品
						iUpdateItemHighLevelNum += 1;
					}
				}
			}
			
			for(int t=0;t<robSlotInfo.size();t++){
				//金钱总数
				llUpdateMoney += StrToLLong(robSlotInfo[t].iMoney_Sub);
			}
			ItemFlowLog->WriteLog("[INFO]: iUpdateItemNum=%d&iUpdateRetrieveNum=%d&iUpdateItemHighLevelNum=%d&llUpdateMoney=%lld\n",iUpdateItemNum,iUpdateRetrieveNum,iUpdateItemHighLevelNum,llUpdateMoney);
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]: iUpdateItemNum=%d&iUpdateRetrieveNum=%d&iUpdateItemHighLevelNum=%d&llUpdateMoney=%lld\n",iUpdateItemNum,iUpdateRetrieveNum,iUpdateItemHighLevelNum,llUpdateMoney);


#ifdef dnf2
			sUpdateType="Item-HighLevel-Money";
			addStatNum.push_back(IntToStr(iUpdateItemNum));
			addStatNum.push_back(IntToStr(iUpdateRetrieveNum));
			addStatNum.push_back(IntToStr(iUpdateItemHighLevelNum));
			addStatNum.push_back(LLongToStr(llUpdateMoney));
#endif
			ItemFlowLog->WriteLog("3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】类型【%s】\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str(),sUpdateType.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】类型【%s】\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str(),sUpdateType.c_str());

			iRet = icsOper.UpdateICSItemStatsProc(sUpdateType,addStatNum,getICSItemProcList[j].sICSId);
			if(iRet==0){
				ItemFlowLog->WriteLog("3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】成功\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】成功\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());

			}else{
				ItemFlowLog->WriteLog("3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】失败\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5单据【%s】更新单据表tbICSItemStats物品数量【%s】失败\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());

			}
			
		}	
	}
	return 0;
}
