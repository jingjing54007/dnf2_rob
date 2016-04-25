/**
@file rob_core_anaylse.cpp

@brief 
	---------------------------------------------------------
	@ ������Ʒ׷��ϵͳ---��ѡ������ѡ��
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
	//������־
	CLog* ItemTraceLog;
	string sItemTrace = "./logs/"+GetCurDateDay()+"RobItemTrace.log";
	ItemTraceLog = new CLog((char*)sItemTrace.c_str());
	ItemTraceLog->LogOn();
	ItemTraceLog->WriteLog("===============Start GetRobItemTraceList============\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start GetRobItemTraceList============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	
	//��Ʒͳ��
	string sUpdateType="Item";
	string sUpdateNum="0";
	int iUpdateItemNum=0;
	int iUpdateItemHighLevelNum=0;
	long long llUpdateMoney = 0;
	vector <string> addStatNum;
	//�Ƿ���Ҫ׷�ٵı�־ 0 ����׷�� 1 ��Ҫ׷��
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
	@1.��ȡ���ڵȴ���Ʒ���ٴ���Ľ׶εĵ��ݡ�
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
	@2.���ĵ��ݴ���״̬��Ϊ��Ʒ���ٴ����С���
	@  STEP_ITEMTRACE_RUN_ICS  STEP_ITEMFTRACE_RUNNING_RESULT  STEP_ITEMTRACE_RUNNING_STAT
	*/
	ItemTraceLog->WriteLog("----2.���ݸ���״̬STEP_ITEMTRACE_RUN_ICS������\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----2.���ݸ���״̬STEP_ITEMTRACE_RUN_ICS������\n");

	for(int i=0;i<iSize;i++){
		iRet = icsOper.UpdateICSProc(getICSItemTraceList[i].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUNNING_RESULT,STEP_ITEMTRACE_RUNNING_STAT);
		if(iRet==0){
			ItemTraceLog->WriteLog("2.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_ICS�ɹ�\n",getICSItemTraceList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[i].sICSId.c_str(), getICSItemTraceList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_ICS�ɹ�\n",getICSItemTraceList[i].sICSId.c_str());

		}else{
			ItemTraceLog->WriteLog("2.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_ICSʧ��\n",getICSItemTraceList[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[i].sICSId.c_str(), getICSItemTraceList[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_ICSʧ��\n",getICSItemTraceList[i].sICSId.c_str());

		}
	}
	ItemTraceLog->WriteLog("----3.���ݶ�ʧ��Ʒ��ˮ��ȡ\n");
	WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "----3.���ݶ�ʧ��Ʒ��ˮ��ȡ\n");

	for(int k=0;k<iSize;k++){
			ItemTraceLog->WriteLog("3.1.0���ݡ�%s����ȡ��ˮ��ʼ\n",getICSItemTraceList[k].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.0���ݡ�%s����ȡ��ˮ��ʼ\n",getICSItemTraceList[k].sICSId.c_str());

			iRet=icsOper.ReadICSItemListProc(robItemFlowInfo,getICSItemTraceList[k].sICSId);
			ItemTraceLog->WriteLog("3.1.0���ݡ�%s����ȡ��ˮ��С[%d]\n",getICSItemTraceList[k].sICSId.c_str(),robItemFlowInfo.size());
			WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.0���ݡ�%s����ȡ��ˮ��С[%d]\n",getICSItemTraceList[k].sICSId.c_str(),robItemFlowInfo.size());

			if(iRet==STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS){
				//���µ���ICS״̬
				iRet = icsOper.UpdateICSProc(getICSItemTraceList[k].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUN_RESULT_FALSE,STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.1.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());

				}else{
					ItemTraceLog->WriteLog("3.1.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICSʧ��\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.1���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICSʧ��\n",getICSItemTraceList[k].sICSId.c_str());

				}
				continue;
			}else if(iRet==0){
				//׷����Ʒ
				//�����Ҫ׷����Ʒ����Ҫ׷�٣�������ItemTrace�ڴ�����
				if(iTraceProcFlag){
					//����ʡ��
					//1.��ȡItemFlow
					//2.������ƷID��Ψһ��������robItemTrace�ı�ĸ�ֵ

				}else{
					//3.1.2.1���µ���ItemFlow��ItemTrace����
					iRet=icsRobRuleLogic.getRobItemTraceNoTracking(robItemTrace,robItemFlowInfo);
					if(iRet==0){
						ItemTraceLog->WriteLog("3.1.2.1���ݡ�%s������׷�٣���ȡ��ҵ���Ʒ��ˮ׷�ٳɹ�\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.1���ݡ�%s������׷�٣���ȡ��ҵ���Ʒ��ˮ׷�ٳɹ�\n",getICSItemTraceList[k].sICSId.c_str());

					}else{
						ItemTraceLog->WriteLog("3.1.2.1���ݡ�%s������׷�٣���ȡ��ҵ���Ʒ��ˮ׷��ʧ��\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.1���ݡ�%s������׷�٣���ȡ��ҵ���Ʒ��ˮ׷��ʧ��\n",getICSItemTraceList[k].sICSId.c_str());

					}
					//3.1.2.1��ItemFlow�ĵ���ϵͳ״̬����
					iRet=icsOper.UpdateICSItemFlow(getICSItemTraceList[k].sICSId,getICSItemTraceList[k].sServiceID,STEP_ItemFlow_RUN_OVER_TRACING,STEP_ItemFlow_NONEED_TRACE_RESULT);
					if(iRet==0){
						ItemTraceLog->WriteLog("3.1.2.2���ݡ�%s������׷�٣�������ҵ���Ʒ��ˮ״̬STEP_ItemFlow_RUN_OVER_TRACING�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.2���ݡ�%s������׷�٣�������ҵ���Ʒ��ˮ״̬STEP_ItemFlow_RUN_OVER_TRACING�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());

					}else{
						ItemTraceLog->WriteLog("3.1.2.2���ݡ�%s������׷�٣�������ҵ���Ʒ��ˮ״̬STEP_ItemFlow_RUN_OVER_TRACINGʧ��\n",getICSItemTraceList[k].sICSId.c_str());
						WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.2.2���ݡ�%s������׷�٣�������ҵ���Ʒ��ˮ״̬STEP_ItemFlow_RUN_OVER_TRACINGʧ��\n",getICSItemTraceList[k].sICSId.c_str());

					}
		
				}
				//3.1.3��ItemTrace�ĵ���ϵͳ���뵽ItemTrace���У���������״̬
				iRet = icsOper.InsertICSItemTrace(robItemTrace);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.1.3���ݡ�%s��׷����ɣ���ȡ��ҵ���Ʒ׷�����ݳɹ�\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3���ݡ�%s��׷����ɣ���ȡ��ҵ���Ʒ׷�����ݳɹ�\n",getICSItemTraceList[k].sICSId.c_str());
				}else{
					ItemTraceLog->WriteLog("3.1.3���ݡ�%s��׷����ɣ���ȡ��ҵ���Ʒ׷������ʧ��\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.1.3���ݡ�%s��׷����ɣ���ȡ��ҵ���Ʒ׷������ʧ��\n",getICSItemTraceList[k].sICSId.c_str());

				}
				//3.2���µ���ICS״̬
				iRet = icsOper.UpdateICSProc(getICSItemTraceList[k].sID,STEP_ITEMTRACE_RUN_ICS,STEP_ITEMFTRACE_RUN_RESULT_TURE,STEP_ITEMTRACE_RUN_STAT_SUCCESS);
				if(iRet==0){
					ItemTraceLog->WriteLog("3.2���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_SUCCESS�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE ,getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.2���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_SUCCESS�ɹ�\n",getICSItemTraceList[k].sICSId.c_str());

				}else{
					ItemTraceLog->WriteLog("3.2���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_SUCCESSʧ��\n",getICSItemTraceList[k].sICSId.c_str());
					WriteLogsRUN(TLOG_TYPE_ROBITEMTRACE , getICSItemTraceList[k].sICSId.c_str(),getICSItemTraceList[k].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3.2���ݡ�%s������״̬STEP_ITEMTRACE_RUN_STAT_SUCCESSʧ��\n",getICSItemTraceList[k].sICSId.c_str());
				}

			}
	}
	return 0;
}
