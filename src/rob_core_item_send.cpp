/**
@file rob_core_item_send.cpp

@brief 
	---------------------------------------------------------
	@ ������Ʒ����ϵͳ---
	@ 1���������Ҫ׷�٣���Ҫ����Ʒ��ˮϵͳ���µ�׷�ٱ���
	@ 2�������Ҫ׷�٣�a: ׷Ѱ���п��ܵ���Ʒת����־
					b: ���ҵ�һ�ֺ����һ�ֵ���־ȷ���������ߡ��͡�ӵ���ߡ�
	@ 3����¼��صĵ�һ�ֺ����һ����־����Ʒ���ٱ��С�
		 ע�⣺�����Ŀ�Ľ�ɫ���ʺţ�����Ҫ��д�����������д����������
		 ��������eg. P2P���׵Ŀ���ȷ���Է��Ľ�ɫ���ʺ� �����ٺ���ʧ����¼
		 ��NPC����0�����Զ������
	@ 4��������ص���Ʒ������־�ı�״̬���Լ�ICS���ݱ�״̬
	@ 5��������ص���Ʒ���ٱ�Ĵ���״̬
	---------------------------------------------------------
history:
<PRE>
History ID	:	1.01
Author    	:	asherzhou
DateTime	:	2011/05/01
Description :	Create
</PRE>

<<<<<<<<<<<<<<<<<<<<<<dnf>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Cdkey�һ����߽ӿ�

cmd=1003&area=1&send_charac_name=system&receive_charac_no=16584622&item_id=2360&endurance=7&item_type=2&item_num=3&upgrade=2&seal_flag=1

result=0&area=1&receive_charac_no=16584622

˵����result��0��ʾ�����ɹ���< 0��ʾ����ʧ�� ���͵��ߵ����ͣ�
��item_type�Ķ������£� typedef enum ITEM_TYPE { T_COIN = 0, //��Ǯ 
											  T_HONOUR = 1, //�ƺ� 
											  T_EQUIP = 2, //װ�� 
											  T_EXPENDABLE = 3, //����Ʒ 
											  T_AVATAR = 4, //װ�� 
											  //T_PET = 5 
											  �ͳ�����ʹ��ר�ŵĽӿ�1020 
											  };
ע�⣺ ǧ��Ҫ���������ͣ�������������������˵ģ�����ɱȽϴ����ʧ 
seal_flag			�Ƿ�ɽ��ס�Ĭ��Ϊ0���ɽ��� 
send_charac_name	���url encode���� 
endurance			��ͨ��Ʒ (�;ö�)/�����(0: ���, 1: �, 2: �շ�)/װ��(װ���ѡ������) 
upgrade				��Ʒǿ��ֵ������
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
	//������־
	CLog* ItemSendLog;
	string sItemSend = "./logs/"+GetCurDateDay()+"RobItemSend.log";
	ItemSendLog = new CLog((char*)sItemSend.c_str());
	ItemSendLog->LogOn();
	ItemSendLog->WriteLog("===============Start GetRobItemSend============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMSEND , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemSend============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	
	//��Ʒͳ��
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
	@1.��ȡ���ڵȴ���Ʒ���ʹ���Ľ׶εĵ��ݡ�
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
	@ 2.�������ʹ������̡�
	@	a. �޸�ics�ܵ��ݵĴ���״̬
	@	b. ��ȡ��������Ҫ�������Ʒ�б�
	@	c. ���ݹ�����
	@	d. ����޸�ItemTrace��ICS���ݱ�,tbICSItemStats�Ĵ�����
	*/
	//2.1�޸�ics�ܵ��ݵĴ���״̬
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
		//2.2 ��ȡ��������Ҫ�������Ʒ�б�
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

			//2.2.1 ���ݹ�����������Ʒ��VIP���ж�����Ʒ��ˮ��������Ӧ����Ʒ�����޸ģ�
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

			//2.2.2 ItemTrace����޸�
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

			//2.2.3 tbICSItemStats����޸�
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
