/**
@file rob_core_anaylse.cpp

@brief 

	---------------------------------------------------------
	����������������(IP�쳣�����������������ˮ����)
	
	�����ĵ��ݱ����ǵ��ݺϷ��ģ��ǵ��Ŵ�������У�ɾ��������Ʒ��ǰ��
	��������������������� ICS����
	����������£�1) ��ȡҪ�����ĵ��ݣ�
	1���ȼ��Ƿ���� 2)ʱ������ж�
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
	//������־
	CLog* ItemFlowLog;
	string sItemFlowLog = "./logs/"+GetCurDateDay()+"RobItemFlow.log";
	ItemFlowLog = new CLog((char*)sItemFlowLog.c_str());
	ItemFlowLog->LogOn();
	ItemFlowLog->WriteLog("===============Start GetRobItemFlowList============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemFlowList============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	//��Ʒͳ��
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
	@1.��ȡ���ڵȴ���Ʒ��ˮ����Ľ׶εĵ��ݡ�
	*/
	vector < Rob_SLOT > robSlotInfo;
	Rob_Ics_Complaint getICSItemProcList[sizeOfItemFlow];
	vector< Item_Flow_List > robItemFlow;

	int iSize = 0;

	if (sizeOfItemFlow == 1)//����size��Ϊ1ʱ�����ȡָ������
	{
		string ics_id = CRobConf::getConf()->GetIcsOper()["ics_id_send_only_one"];//����id
		ics_id.erase(remove_if(ics_id.begin(),ics_id.end(), bind2nd(equal_to<char>(), '\t')), ics_id.end());//�Ƴ����пո��Ʊ����
		if (ics_id.empty())
		{
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW, "", "",TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "��ȡָ���ĵ���ID����ics_id_only_one����Ϊ�գ����飡\n" );
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
	@2.���ĵ��ݴ���״̬��Ϊ�����С���
	@  STEP_ITEMFLOW_RUN_ICS  STEP_ITEMFLOW_RUNNING_RESULT  STEP_ITEMFLOW_RUNNING_STAT
	*/
	ItemFlowLog->WriteLog("----2.���ݸ���״̬STEP_ITEMFLOW_RUN_ICS������\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----2.���ݸ���״̬STEP_ITEMFLOW_RUN_ICS������\n");

	for(int i=0;i<iSize;i++){
		iRet = icsOper.UpdateICSProc(getICSItemProcList[i].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUNNING_RESULT,STEP_ITEMFLOW_RUNNING_STAT);
		if(iRet==0){
			ItemFlowLog->WriteLog("2.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_ICS�ɹ�\n",getICSItemProcList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[i].sICSId.c_str(), getICSItemProcList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_ICS�ɹ�\n",getICSItemProcList[i].sICSId.c_str());

		}else{
			ItemFlowLog->WriteLog("2.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_ICSʧ��\n",getICSItemProcList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[i].sICSId.c_str(), getICSItemProcList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_ICSʧ��\n",getICSItemProcList[i].sICSId.c_str());

		}
	}
	/********************************************************************************
	@3.��ȡ��Ӧ���ݵ��쳣��¼��Ϣ����
	*/
	for(int j=0;j<iSize;j++)
	{
		ItemFlowLog->WriteLog("----3.�ڡ�%d�������š�%s�������ȡ�쳣ʱ��εĴ����0000000\n",j,getICSItemProcList[j].sICSId.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.�ڡ�%d�������š�%s�������ȡ�쳣ʱ��εĴ����0000000\n",j,getICSItemProcList[j].sICSId.c_str());

		iRet=icsOper.ReadICSRobSlot(robSlotInfo,getICSItemProcList[j].sServiceID,getICSItemProcList[j].sICSId);
		if(iRet==-10000){
			//��������������ʽ�С�����Բ�����
			continue;
		}else{
			/********************************************************************************
			@3.1 ���ö�Ӧ��ʱ�����������Ҷ�ʧ��Ʒ�Ĳ�ѯ
			*/
			ItemFlowLog->WriteLog("----3.1.0�ڡ�%d�������š�%s�������ȡ�쳣ʱ��εĴ���̴�С��%d��\n",j,getICSItemProcList[j].sICSId.c_str(),robSlotInfo.size());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.1.0�ڡ�%d�������š�%s�������ȡ�쳣ʱ��εĴ���̴�С��%d��\n",j,getICSItemProcList[j].sICSId.c_str(),robSlotInfo.size());

			iRet = icsRobRuleLogic.getRoleItemFlowList(robItemFlow,robSlotInfo,getICSItemProcList[j].sIsVip);
			if(iRet==STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW){
				//3.1.1 ���µ���Ϊ�쳣�������
				iRet = icsOper.UpdateICSProc(getICSItemProcList[j].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUN_RESULT_FALSE,STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW�ɹ�\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW�ɹ�\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOWʧ��\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1���ݡ�%s������״̬STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOWʧ��\n",getICSItemProcList[j].sICSId.c_str());

				}
			}else if(iRet==0){
				ItemFlowLog->WriteLog("----3.1.2�ڡ�%d�������š�%s�������ȡ��Ʒ��ˮ��С��%d��\n",j,getICSItemProcList[j].sICSId.c_str(),robItemFlow.size());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.1.2�ڡ�%d�������š�%s�������ȡ��Ʒ��ˮ��С��%d��\n",j,getICSItemProcList[j].sICSId.c_str(),robItemFlow.size());

				//3.1.2 ���µ��ݽ���tbICSItemFlow����
				iRet = icsOper.InsertICSRobItemFlowData(robItemFlow,getICSItemProcList[j].sServiceID);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.2���ݡ�%s������tbICSItemFlow���ɹ���\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2���ݡ�%s������tbICSItemFlow���ɹ���\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.2���ݡ�%s������tbICSItemFlow��ʧ�ܡ�\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2���ݡ�%s������tbICSItemFlow��ʧ�ܡ�\n",getICSItemProcList[j].sICSId.c_str());

				}
				//3.1.3 ���µ��ݱ�tbICSComplaint״̬
				iRet = icsOper.UpdateICSProc(getICSItemProcList[j].sID,STEP_ITEMFLOW_RUN_ICS,STEP_ITEMFLOW_RUN_RESULT_TURE,STEP_ITEMFLOW_RUN_STAT_SUCCESS);
				if(iRet==0){
					ItemFlowLog->WriteLog("3.1.3-1���ݡ�%s�����µ��ݱ�tbICSComplaint״̬STEP_ITEMFLOW_RUN_STAT_SUCCESS�ɹ�\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3-1���ݡ�%s�����µ��ݱ�tbICSComplaint״̬STEP_ITEMFLOW_RUN_STAT_SUCCESS�ɹ�\n",getICSItemProcList[j].sICSId.c_str());

				}else{
					ItemFlowLog->WriteLog("3.1.3-1���ݡ�%s�����µ��ݱ�tbICSComplaint״̬STEP_ITEMFLOW_RUN_STAT_SUCCESSʧ��\n",getICSItemProcList[j].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3-1���ݡ�%s�����µ��ݱ�tbICSComplaint״̬STEP_ITEMFLOW_RUN_STAT_SUCCESSʧ��\n",getICSItemProcList[j].sICSId.c_str());

				}
			}
			//3.1.4 ���µ��ݱ�tbICSRoleLogout״̬
			iRet = icsOper.UpdateICSRobSlotProc(getICSItemProcList[j].sServiceID,getICSItemProcList[j].sICSId,STEP_SlotStat_RUN_OVER_STAT);
			if(iRet==0){
				ItemFlowLog->WriteLog("3.1.4���ݡ�%s�����µ��ݱ�tbICSRoleLogout״̬STEP_ITEMFLOW_RUN_STAT_SUCCESS�ɹ�\n",getICSItemProcList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.4���ݡ�%s�����µ��ݱ�tbICSRoleLogout״̬STEP_ITEMFLOW_RUN_STAT_SUCCESS�ɹ�\n",getICSItemProcList[j].sICSId.c_str());

			}else{
				ItemFlowLog->WriteLog("3.1.4���ݡ�%s�����µ��ݱ�tbICSRoleLogout״̬STEP_ITEMFLOW_RUN_STAT_SUCCESSʧ��\n",getICSItemProcList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW ,getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.4���ݡ�%s�����µ��ݱ�tbICSRoleLogout״̬STEP_ITEMFLOW_RUN_STAT_SUCCESSʧ��\n",getICSItemProcList[j].sICSId.c_str());

			}
			//3.1.5 ������Ʒͳ�Ʊ�tbICSItemStats״̬
			iUpdateItemNum=0;
			iUpdateRetrieveNum=0;
			iUpdateItemHighLevelNum=0;
			llUpdateMoney = 0;
			addStatNum.clear();
			//��ʧ��������
			iUpdateItemNum=robItemFlow.size();
			ItemFlowLog->WriteLog("[INFO]: iUpdateItemNum=%d\n",iUpdateItemNum);
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]: iUpdateItemNum=%d\n",iUpdateItemNum);

			for(int t=0;t<robItemFlow.size();t++){
				if(robItemFlow[t].iDefineProc=="1"||robItemFlow[t].iDefineProc=="2"){
					//��Ҫ�һص���Ʒ
					iUpdateRetrieveNum += 1;
					if(robItemFlow[t].iDefineProc=="2"){
						//�ߵȼ���Ʒ���˹��������Ʒ
						iUpdateItemHighLevelNum += 1;
					}
				}
			}
			
			for(int t=0;t<robSlotInfo.size();t++){
				//��Ǯ����
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
			ItemFlowLog->WriteLog("3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s�����͡�%s��\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str(),sUpdateType.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s�����͡�%s��\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str(),sUpdateType.c_str());

			iRet = icsOper.UpdateICSItemStatsProc(sUpdateType,addStatNum,getICSItemProcList[j].sICSId);
			if(iRet==0){
				ItemFlowLog->WriteLog("3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s���ɹ�\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s���ɹ�\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());

			}else{
				ItemFlowLog->WriteLog("3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s��ʧ��\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBITEMFLOW , getICSItemProcList[j].sICSId.c_str(), getICSItemProcList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.5���ݡ�%s�����µ��ݱ�tbICSItemStats��Ʒ������%s��ʧ��\n",getICSItemProcList[j].sICSId.c_str(),sUpdateNum.c_str());

			}
			
		}	
	}
	return 0;
}
