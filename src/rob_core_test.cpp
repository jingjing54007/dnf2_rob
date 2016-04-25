/**
Description :	test modefun
Time            :    20121024
Author    	    :	v_yfzhu
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
	CLog* ReplayLog;
	string sReplayLog = "./logs/RobTest_zyf.log";
	ReplayLog = new CLog((char*)sReplayLog.c_str());
	ReplayLog->LogOn();
	ReplayLog->WriteLog("===============Start RobFunTestAuto======eeeeee======\n");
	WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO  , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start RobCoreReplayAuto============\n");

	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;


	int iRet = 0;
	int sizeOfReplay = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_six"]);
	if(sizeOfReplay<=0)
	{
		ReplayLog->WriteLog("[STEP:FALTA]:::Please Check Your Rob.conf ,'size_of_ics_six' must be larger 0\n");
		return -110;
	}
	return 0;
	/********************************************************************************
	@1.��ȡ���ڵȴ���Ʒ���ٴ���Ľ׶εĵ��ݡ�
	*/
	Rob_Ics_Complaint getICSReplayList[sizeOfReplay];


	int iSize = icsOper.ReadICSInfoList(STEP_REPLAY_RUN_ICS,getICSReplayList,sizeOfReplay);
	if(iSize > 0 && iSize <= sizeOfReplay)
	{
		ReplayLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size:%d and less than sizeOfReplay Successfully\n",iSize);
	}
	else if(iSize > sizeOfReplay)
	{
		ReplayLog->WriteLog("[STEP:WARNNING]:::ReadICSInfoList GET sizeOfReplay size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfReplay:%d\n",iSize,sizeOfReplay);
		return -111;
	}
	else if(iSize == 0)
	{
		ReplayLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size 0 and No Info to Proccess,So Return\n");
		return 0;
	}
	else
	{
		ReplayLog->WriteLog("[STEP:FALTA]:::ReadICSInfoList GET sizeOfReplay size:%d Happend Error\n",iSize);
		return -112;
	}
	/********************************************************************************/

	for(int j=0;j<iSize;j++)
	{
		if(getICSReplayList[j].siSource=="10")
		{
			getICSReplayList[j].siSource=="30";
		}
		//O�ߵ�ICS��ظ�
		if(getICSReplayList[j].siSource=="10")
		{
			ReplayLog->WriteLog("---------------------:3.1����ͷ�O��Ʒ�����\n");
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS)
			{
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			else if(iRet==STEP_Reply_WAITING_RESULT_ERROR)
			{
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			else
			{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			//����ͷ�R��Ʒ�
		}
		else if(getICSReplayList[j].siSource=="30")
		{
			ReplayLog->WriteLog("---------------------:3.1����ͷ�R��Ʒ�����\n");
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS)
			{
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			else if(iRet==STEP_Reply_WAITING_RESULT_ERROR)
			{
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}
			else
			{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
		}
		else
		{
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS)
			{
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1���ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			else if(iRet==STEP_Reply_WAITING_RESULT_ERROR)
			{
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1���ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}
			else
			{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1���ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			}

		}

	}
	return 0;
}
