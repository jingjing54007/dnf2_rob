/**
@file rob_core_anaylse.cpp

@brief 

	
	---------------------------------------------------------
	����������������
	
	�����ĵ��ݱ����ǵ��ݺϷ��ģ�
	����������£�
	1) ��ȡҪ�����ĵ��ݣ�
	2) ���ݹ���ϵͳ; 
	3) ����״̬����;
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
	//������־
	CLog* ICSFliterLOG;
	
	//����ģ����
	RobICSOper icsOper;
	RobRuleValid icsCheckValid;
	RobRuleLogic icsRobRuleLogic;
	int iRet = 0;

	//������־�ĳ�ʼ��
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
	@   A: �ڴ洢�棬Rob_Ics_Complaint
	@	1.��ȡSIZE_OF_ICS�ĵ�����Ϣ������Ics_Info������
	*/
	Rob_Ics_Complaint Ics_Info[SIZE_OF_ICS];

	map<string,string> theParamIn;
	vector < map<string,string> > icsResult;
	
	
	// int size_of_ics_info = icsOper.ReadICSInfoList(STEP_FLITER_RUN_ICS,Ics_Info,SIZE_OF_ICS);
	// modified by v_yilyan 20160225 beg
	int size_of_ics_info = 0;
	if (SIZE_OF_ICS == 1){ // 2.1��ȡSIZE_OF_ICS�ĵ�����Ϣ��SIZE����1����ȡָ������
		string ics_id = CRobConf::getConf()->GetIcsOper()["ics_id_send_only_one"];//����id
		ics_id.erase(remove_if(ics_id.begin(),ics_id.end(), bind2nd(equal_to<char>(), '\t')), ics_id.end());//�Ƴ����пո��Ʊ����
		if (ics_id.empty()){
			ICSFliterLOG->WriteLog("��ȡָ���ĵ���ID����ics_id_send_only_one����Ϊ�գ����飡\n");// ics_id_send_only_one
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "��ȡָ���ĵ���ID����ics_id_send_only_one����Ϊ�գ����飡\n");
		}
		size_of_ics_info = icsOper.ReadICSInfoOneItem(STEP_FLITER_RUN_ICS, Ics_Info, ics_id);
	}else{ // 2.2��ȡSIZE_OF_ICS�ĵ�����Ϣ��SIZE����1������Ics_Info������
		size_of_ics_info = icsOper.ReadICSInfoList(STEP_FLITER_RUN_ICS, Ics_Info, SIZE_OF_ICS);
	}
	// modified by v_yilyan 20160225 end

	if(size_of_ics_info >= 0 && size_of_ics_info <= SIZE_OF_ICS){
		ICSFliterLOG->WriteLog("ReadICSInfo GET ics_info size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"ReadICSInfo GET ics_info size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);

		//2.���µ���״̬Ϊ������
		for(int j=0;j<size_of_ics_info;j++){
			iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUNNING_RESULT,STEP_FLITER_RUNNING_STAT);
			ICSFliterLOG->WriteLog("UpdateICSProc iRet:%d\n",iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[j].sID.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"UpdateICSProc iRet:%d\n",iRet);

			if(iRet!=0){
				//������Ҫ�ʼ��������������汾����
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
	@   B: ���ö�ȡ����Ϣ������ع���ϵͳ�����жϣ�Rob_Ics_Complaint
	@	1.ѭ���жϺͶ�ȡ
	*/
	for(int i=0;i<size_of_ics_info;i++){
		iRet = icsCheckValid.DoICSCheckPro(Ics_Info[i]);
		ICSFliterLOG->WriteLog("icsCheckValid.DoICSCheck iRetResult:%d\n",iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"icsCheckValid.DoICSCheck iRetResult:%d\n",iRet);

		if((ApplyAnaylseResult)iRet==STEP_FLITER_RUN_STAT_SUCCESS){
			ICSFliterLOG->WriteLog("���ݡ�%s����֤��ȷ!!!\n",Ics_Info[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s����֤��ȷ!!!\n",Ics_Info[i].sICSId.c_str());

			icsOper.UpdateICSProc(Ics_Info[i].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUN_RESULT_TURE,(ApplyAnaylseStat)iRet);
		}else{
			ICSFliterLOG->WriteLog("���ݡ�%s����֤ʧ��!!!\n",Ics_Info[i].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s����֤ʧ��!!!\n",Ics_Info[i].sICSId.c_str());

			icsOper.UpdateICSProc(Ics_Info[i].sID,STEP_FLITER_RUN_ICS,STEP_FLITER_RUN_RESULT_FALSE,(ApplyAnaylseStat)iRet);
		}
		// 2.ͳ�ƿ�Ĳ���
		iRet = icsOper.AddICSItemStatsProc(Ics_Info[i].sICSId,Ics_Info[i].sServiceID);
		if(iRet!=0){
			ICSFliterLOG->WriteLog("���ݡ�%s��ͳ�ƿ�Ĳ���ʧ��!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s��ͳ�ƿ�Ĳ���ʧ��!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		}
		ICSFliterLOG->WriteLog("���ݡ�%s��ͳ�ƿ�Ĳ���ɹ�!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s��ͳ�ƿ�Ĳ���ɹ�!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		// 3.��ɫ������Ϣ����
		iRet = icsOper.UpdateICSRoleInfo(Ics_Info[i]);
		if(iRet!=0){
			ICSFliterLOG->WriteLog("���ݡ�%s����ɫ������Ϣ����ʧ��!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);
			WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s����ɫ������Ϣ����ʧ��!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);

		}
		ICSFliterLOG->WriteLog("���ݡ�%s����ɫ������Ϣ���³ɹ�!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSFILTER , Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"���ݡ�%s����ɫ������Ϣ���³ɹ�!!!�����룺%d\n",Ics_Info[i].sICSId.c_str(),iRet);

	}
	delete ICSFliterLOG;
	return 0;

}
