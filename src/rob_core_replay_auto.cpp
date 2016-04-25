/**
@file rob_core_anaylse.cpp

@brief 
	---------------------------------------------------------
	@ ���Ŷ��λظ�ϵͳ
	@ 1.������Ҫ�ظ��ĵ��������Ѿ���ɴ���ĵ��ݣ�Ȼ��ظ�
	@ 2.���д����еĵ��ݲ���ظ�
	@ 3.�ظ���ĵ������ս���
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
	CLog* ReplayLog;
	string sReplayLog = "./logs/"+GetCurDateDay()+"RobReplayAuto.log";
	ReplayLog = new CLog((char*)sReplayLog.c_str());
	ReplayLog->LogOn();
	ReplayLog->WriteLog("===============Start RobCoreReplayAuto============\n");
	WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "===============Start RobCoreReplayAuto============\n");


	RobICSOper icsOper;
	RobRuleLogic icsRobRuleLogic;
	

	int iRet = 0;
	int sizeOfReplay = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_six"]);
	if(sizeOfReplay<=0)
	{
		WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::Please Check Your Rob.conf ,'size_of_ics_six' must be larger 0\n");
		ReplayLog->WriteLog("[STEP:FALTA]:::Please Check Your Rob.conf ,'size_of_ics_six' must be larger 0\n");

		return -110;
	}
	/********************************************************************************
	@1.��ȡ���ڵȴ���Ʒ���ٴ���Ľ׶εĵ��ݡ�
	*/
	Rob_Ics_Complaint getICSReplayList[sizeOfReplay];
	
	int iSize = icsOper.ReadICSInfoList(STEP_REPLAY_RUN_ICS,getICSReplayList,sizeOfReplay);
	if(iSize > 0 && iSize <= sizeOfReplay)
	{
		ReplayLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size:%d and less than sizeOfReplay Successfully\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size:%d and less than sizeOfReplay Successfully\n",iSize);

	}
	else if(iSize > sizeOfReplay)
	{
		ReplayLog->WriteLog("[STEP:WARNNING]:::ReadICSInfoList GET sizeOfReplay size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfReplay:%d\n",iSize,sizeOfReplay);
		WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:WARNNING]:::ReadICSInfoList GET sizeOfReplay size:%d is too larger, Plesse Check SAP Systerm if bigger than sizeOfReplay:%d\n",iSize,sizeOfReplay);

		return -111;
	}
	else if(iSize == 0)
	{
		ReplayLog->WriteLog("[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size 0 and No Info to Proccess,So Return\n");
		WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::ReadICSInfoList GET sizeOfReplay size 0 and No Info to Proccess,So Return\n");

		return 0;
	}
	else
	{
		ReplayLog->WriteLog("[STEP:FALTA]:::ReadICSInfoList GET sizeOfReplay size:%d Happend Error\n",iSize);
		WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::ReadICSInfoList GET sizeOfReplay size:%d Happend Error\n",iSize);

		return -112;
	}
	/********************************************************************************
	@2.���ĵ��ݴ���״̬��Ϊ��Ʒ���ٴ����С���
	@  STEP_ITEMTRACE_RUN_ICS  STEP_ITEMFTRACE_RUNNING_RESULT  STEP_ITEMTRACE_RUNNING_STAT
	*/
	ReplayLog->WriteLog("[STEP::STAGE]:::2.���ݴ���ظ��ĸ���״̬STEP_REPLAY_RUN_ICS������\n");
	WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP::STAGE]:::2.���ݴ���ظ��ĸ���״̬STEP_REPLAY_RUN_ICS������\n");

	/*for(int i=0;i<iSize;i++){
		ReplayLog->WriteLog("[STEP::DEBUG]:::[id=%d&result=%s&stat=%s]\n",i,getICSReplayList[i].sICSAnaylseResult.c_str(),getICSReplayList[i].sICSAnaylseStat.c_str());
		iRet = icsOper.UpdateICSProc(getICSReplayList[i].sID,(StrToInt(getICSReplayList[i].sICSAnaylseStep)),(ApplyAnaylseResult)(StrToInt(getICSReplayList[i].sICSAnaylseResult)),(ApplyAnaylseStat)(StrToInt(getICSReplayList[i].sICSAnaylseStat)));
		if(iRet==0){
			ReplayLog->WriteLog("[STEP:PHASE]:::2.1���ݡ�%s������ID��%s�����½׶�STEP_REPLAY_RUN_ICS�ɹ�\n",getICSReplayList[i].sID.c_str(),getICSReplayList[i].sICSId.c_str());
		}else{
			ReplayLog->WriteLog("[STEP:ERROR]:::2.1���ݡ�%s������ID��%s�����½׶�STEP_REPLAY_RUN_ICSʧ��\n",getICSReplayList[i].sID.c_str(),getICSReplayList[i].sICSId.c_str());
		}
	}*/
	ReplayLog->WriteLog("[STEP::STAGE]:::3.���ݴ���ظ��Ĵ���ʼ\n");
	WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP::STAGE]:::3.���ݴ���ظ��Ĵ���ʼ\n");

	for(int j=0;j<iSize;j++){
		//O�ߵ�ICS��ظ�
		if(getICSReplayList[j].siSource=="10"){
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS){
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else if(iRet==STEP_Reply_WAITING_RESULT_ERROR){
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:ERROR]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::3.1O�ߵ�ICS��ظ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}
		//����ͷ�R��Ʒ�
		}
		else if(getICSReplayList[j].siSource=="30" || getICSReplayList[j].siSource=="40")//modify by 20121224 v_yfzhu
		{
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS){
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else if(iRet==STEP_Reply_WAITING_RESULT_ERROR){
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO ,getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:ERROR]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::3.1����ͷ�R��Ʒ����ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}
		}else{
			iRet = icsRobRuleLogic.doReplayIcsResult(getICSReplayList[j],getICSReplayList[j].siSource);
			if(iRet==STEP_Reply_WAITING_RESULT_SUCCESS){
				ReplayLog->WriteLog("[STEP:PHASE]:::3.1���ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::3.1���ݡ�%s������ID��%s������ظ��ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else if(iRet==STEP_Reply_WAITING_RESULT_ERROR){
				ReplayLog->WriteLog("[STEP:ERROR]:::3.1���ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO ,getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:ERROR]:::3.1���ݡ�%s������ID��%s������ظ�ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}else{
				ReplayLog->WriteLog("[STEP:FALTA]:::3.1���ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:FALTA]:::3.1���ݡ�%s������ID��%s������ظ��쳣\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

			}
			//�ظ���ȫ����
			/*******************begin*******************/
			// �ֻ�QQ��ȫ����app���³�����ʿר��
			iRet = icsRobRuleLogic.doReplayMobileApp(getICSReplayList[j], 0);
			ReplayLog->WriteLog("�ظ��ֻ�QQ��ȫ����app���³�����ʿר�����[��]: getICSReplayList[%s],iRet[%d]\n", getICSReplayList[j].sICSId.c_str(), iRet);
			WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO, getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "�ظ��ֻ�QQ��ȫ����app���³�����ʿר�����[��]: ics_id[%s],iRet[%d]\n", getICSReplayList[j].sICSId.c_str(), iRet );
			
			if ( 0 != iRet )	// �ظ�����ʧ�ܺ󣬻ظ�����
			{
				iRet = icsRobRuleLogic.doReplayMobileApp(getICSReplayList[j], 1);
				ReplayLog->WriteLog("�ظ��ֻ�QQ��ȫ����app���³�����ʿר�����[��]: getICSReplayList[%s],iRet[%d]\n", getICSReplayList[j].sICSId.c_str(), iRet);
				WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO, getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "�ظ��ֻ�QQ��ȫ����app���³�����ʿר�����[��]: ics_id[%s],iRet[%d]\n", getICSReplayList[j].sICSId.c_str(), iRet );
			}
			/*******************end*******************/
		
		}
		iRet = icsOper.updateICSReplyResult(getICSReplayList[j]);
		if(iRet==0){
			ReplayLog->WriteLog("[STEP:PHASE]:::3.2���ݡ�%s������ID��%s�����»ظ�����ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO , getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHASE]:::3.2���ݡ�%s������ID��%s�����»ظ�����ɹ�\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());

		}else{
			ReplayLog->WriteLog("[STEP:ERROR]:::3.2���ݡ�%s������ID��%s�����»ظ����ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBREPLYAUTO ,getICSReplayList[j].sICSId.c_str(), getICSReplayList[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__, "[STEP:ERROR]:::3.2���ݡ�%s������ID��%s�����»ظ����ʧ��\n",getICSReplayList[j].sID.c_str(),getICSReplayList[j].sICSId.c_str());
		}
	}
	return 0;
}
