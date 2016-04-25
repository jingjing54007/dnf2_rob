/**
@file rob_core_item_send.cpp

@brief 
	---------------------------------------------------------
	@ 盗号物品补送系统---
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

<<<<<<<<<<<<<<<<<<<<<<dnf>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Cdkey兑换道具接口

cmd=1003&area=1&send_charac_name=system&receive_charac_no=16584622&item_id=2360&endurance=7&item_type=2&item_num=3&upgrade=2&seal_flag=1

result=0&area=1&receive_charac_no=16584622

说明：result＝0表示操作成功，< 0表示操作失败 发送道具的类型，
即item_type的定义如下： typedef enum ITEM_TYPE { T_COIN = 0, //金钱 
											  T_HONOUR = 1, //称号 
											  T_EQUIP = 2, //装备 
											  T_EXPENDABLE = 3, //消耗品 
											  T_AVATAR = 4, //装扮 
											  //T_PET = 5 
											  送宠物请使用专门的接口1020 
											  };
注意： 千万不要搞错道具类型，否则赠送数量会很吓人的，会造成比较大的损失 
seal_flag			是否可交易。默认为0不可交易 
send_charac_name	结果url encode编码 
endurance			普通物品 (耐久度)/复活币(0: 免费, 1: 活动, 2: 收费)/装扮(装扮的选择属性) 
upgrade				物品强化值，慎用
*/




#include <iostream>
#include "rob_ics_oper.h"
#include "rob_rule_logic.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"
#include "comm_strings.h"
int main(int argc, char** argv)
{
	//调试日志
	CLog* ItemSendLog;
	string sItemSend = "./logs/"+GetCurDateDay()+"RobItemSend.log";
	ItemSendLog = new CLog((char*)sItemSend.c_str());
	ItemSendLog->LogOn();
	ItemSendLog->WriteLog("===============Start GetRobItemSend============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemSend============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	
	//物品统计
	string sUpdateType="Item";
	string sUpdateNum="0";
	int iUpdateItemNum=0;
	int iUpdateItemHighLevelNum=0;
	long long llUpdateMoney = 0;
	vector <string> retrieveStatNum;

	int iRet = 0;
	int sizeOfItemSend = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_five"]);
	if(sizeOfItemSend<=0)
	{
		ItemSendLog->WriteLog("Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Please Check Your Rob.conf ,'size_of_ics' must be larger 0\n");
		return -110;
	}
	/********************************************************************************
	@1.读取处于等待物品补送处理的阶段的单据。
	*/
	vector < Item_Trace_List > robItemSend;
	Rob_Ics_Complaint getICSItemSendList[sizeOfItemSend];
	

	int iSize = icsOper.ReadICSInfoList(STEP_ITEMSEND_RUN_ICS,getICSItemSendList,sizeOfItemSend);
	if(iSize > 0 && iSize <= sizeOfItemSend)
	{
		ItemSendLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfItemSend size:%d and less than sizeOfItemSend Successfully\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::ReadICSInfoList GET sizeOfItemSend size:%d and less than sizeOfItemSend Successfully\n",iSize);

	}
	else if(iSize > sizeOfItemSend)
	{
		ItemSendLog->WriteLog("[STEP:WARNNING]:::ReadICSInfoList GET sizeOfItemSend size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemSend:%d\n",iSize,sizeOfItemSend);
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:WARNNING]:::ReadICSInfoList GET sizeOfItemSend size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfItemSend:%d\n",iSize,sizeOfItemSend);

		return -111;
	}
	else if(iSize == 0)
	{
		ItemSendLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfItemSend size 0 and No Info to Proccess,So Return\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::ReadICSInfoList GET sizeOfItemSend size 0 and No Info to Proccess,So Return\n");

		return 0;
	}
	else
	{
		ItemSendLog->WriteLog("[STEP:FALTA]:::ReadICSInfoList GET sizeOfItemSend size:%d Happend Error\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::ReadICSInfoList GET sizeOfItemSend size:%d Happend Error\n",iSize);

		return -112;
	}
	/********************************************************************************
	@ 2.单独补送处理流程。
	@	a. 修改ics总单据的处理状态
	@	b. 读取单号下需要处理的物品列表
	@	c. 依据规则补送
	@	d. 结果修改ItemTrace和ICS单据表,tbICSItemStats的处理结果
	*/
	//2.1修改ics总单据的处理状态
	//STEP_ITEMSEND_RUN_ICS STEP_ITEMFSEND_RUNNING_RESULT  STEP_ITEMSEND_RUNNING_STAT
	ItemSendLog->WriteLog("[STEP]:::STEP_ITEMSEND_RUN_ICS:::\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::STEP_ITEMSEND_RUN_ICS:::\n");

	for(int k=0;k<iSize;k++){
		iRet = icsOper.UpdateICSProc(getICSItemSendList[k].sID,STEP_ITEMSEND_RUN_ICS,STEP_ITEMFSEND_RUNNING_RESULT,STEP_ITEMSEND_RUNNING_STAT);
		if(iRet==0){
			ItemSendLog->WriteLog("[INFO]:::STEP_ITEMSEND_RUN_ICS***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::STEP_ITEMSEND_RUN_ICS***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

		}else{
			ItemSendLog->WriteLog("[ERROR]:::STEP_ITEMSEND_RUN_ICS***ERROR\n",getICSItemSendList[k].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::STEP_ITEMSEND_RUN_ICS***ERROR\n",getICSItemSendList[k].sICSId.c_str());

		}
	}
	for(int k=0;k<iSize;k++){
		//2.2 读取单号下需要处理的物品列表
		ItemSendLog->WriteLog("[STEP]:::::::::::::::::::::::[%d]::::::::::::::::::::::::::::\n",k);
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::::::::::::::::::::::[%d]::::::::::::::::::::::::::::\n",k);

		ItemSendLog->WriteLog("[STEP]:::GET_THE_NEED_SEND_ITEMS_LIST:::\n");
		WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::GET_THE_NEED_SEND_ITEMS_LIST:::\n");

		iRet = icsOper.getICSItemSendList(robItemSend,getICSItemSendList[k].sICSId,getICSItemSendList[k].sServiceID);
		if(iRet==STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST){
			iRet = icsOper.UpdateICSProc(getICSItemSendList[k].sID,STEP_ITEMSEND_RUN_ICS,STEP_ITEMFSEND_RUN_RESULT_FALSE,STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST);
			if(iRet==0){
				ItemSendLog->WriteLog("[INFO]:::ICS[%s]STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::ICS[%s]STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

			}else{
				ItemSendLog->WriteLog("[ERROR]:::ICS[%s]STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST***ERROR\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str() ,TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::ICS[%s]STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST***ERROR\n",getICSItemSendList[k].sICSId.c_str());

			}
		}else if(iRet==0){
			ItemSendLog->WriteLog("[STEP]:::SEND_ITEMS_PROCCESS:::\n");
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::SEND_ITEMS_PROCCESS:::\n");

			//2.2.1 依据规则来补送物品（VIP的判断在物品流水那来做相应的物品属性修改）
			iRet = icsRobRuleLogic.doRobItemSend(robItemSend);
			if(iRet==0){
				ItemSendLog->WriteLog("[INFO]:::ICS[%s]SEND_ITEMS_RESULT***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::ICS[%s]SEND_ITEMS_RESULT***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

			}else{
				ItemSendLog->WriteLog("[ERROR]:::ICS[%s]SEND_ITEMS_RESULT***ERROR\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::ICS[%s]SEND_ITEMS_RESULT***ERROR\n",getICSItemSendList[k].sICSId.c_str());

			}
			ItemSendLog->WriteLog("[STEP]:::UPDATE_ITEMS_STATS_IN_tbICSItemTrace:::\n");
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::UPDATE_ITEMS_STATS_IN_tbICSItemTrace:::\n");

			//2.2.2 ItemTrace结果修改
			iRet = icsOper.updateICSItemSendList(robItemSend);
			if(iRet==0){
				ItemSendLog->WriteLog("[INFO]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemTrace***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemTrace***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

			}else{
				ItemSendLog->WriteLog("[ERROR]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemTrace***ERROR\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemTrace***ERROR\n",getICSItemSendList[k].sICSId.c_str());

			}

			ItemSendLog->WriteLog("[STEP]:::UPDATE_ITEMS_STATS_IN_tbICSItemStats:::\n");
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::UPDATE_ITEMS_STATS_IN_tbICSItemStats:::\n");

			//2.2.3 tbICSItemStats结果修改
			retrieveStatNum.clear();
			iUpdateItemNum=0;
			iUpdateItemHighLevelNum=0;
			for(int w=0;w<robItemSend.size();w++){
				if(robItemSend[w].iSendResult=="1"){
					iUpdateItemNum += StrToInt(robItemSend[w].iItem_Num);
					if(robItemSend[w].iItem_Level!="0"){
						iUpdateItemHighLevelNum += StrToInt(robItemSend[w].iItem_Num);
					}
				}
			}
#ifdef dnf2
			sUpdateType = "Item-ItemHighLevel";
			retrieveStatNum.push_back(IntToStr(iUpdateItemNum));
			retrieveStatNum.push_back(IntToStr(iUpdateItemHighLevelNum));
			ItemSendLog->WriteLog("[INFO]:::iUpdateItemNum=%d&iUpdateItemHighLevelNum=%d&\n",iUpdateItemNum,iUpdateItemHighLevelNum);
#endif
			
			iRet = icsOper.updateICSItemStatsResult(sUpdateType,retrieveStatNum,getICSItemSendList[k].sICSId);
			if(iRet==0){
				ItemSendLog->WriteLog("[INFO]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemStats***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemStats***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

			}else{
				ItemSendLog->WriteLog("[ERROR]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemStats***ERROR\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::ICS[%s]UPDATE_ITEMS_STATS_IN_tbICSItemStats***ERROR\n",getICSItemSendList[k].sICSId.c_str());

			}
			ItemSendLog->WriteLog("[STEP]:::UPDATE_ICS_STATE_RESULT:::\n");
			WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::UPDATE_ICS_STATE_RESULT:::\n");

			iRet = icsOper.UpdateICSProc(getICSItemSendList[k].sID,STEP_ITEMSEND_RUN_ICS,STEP_ITEMFSEND_RUN_RESULT_TURE,STEP_ITEMSEND_RUN_STAT_SUCCESS);
			if(iRet==0){
				ItemSendLog->WriteLog("[INFO]:::ICS[%s]UPDATE_ICS_STATE_RESULT***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::ICS[%s]UPDATE_ICS_STATE_RESULT***SUCCESS\n",getICSItemSendList[k].sICSId.c_str());

			}else{
				ItemSendLog->WriteLog("[ERROR]:::ICS[%s]UPDATE_ICS_STATE_RESULT***ERROR\n",getICSItemSendList[k].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMSEND ,getICSItemSendList[k].sICSId.c_str(), getICSItemSendList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[ERROR]:::ICS[%s]UPDATE_ICS_STATE_RESULT***ERROR\n",getICSItemSendList[k].sICSId.c_str());

			}
		}
	}
	
	return 0;
}
