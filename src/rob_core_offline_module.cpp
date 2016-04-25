/**
@file rob_core_offline_module.cpp
@brief 
	---------------------------------------------------------
	����������������----����ģ�ͷ�������
	1����ȡ�����߷�������ĵ��ݣ���ǳ�����ʱδͨ�����������޵���ʱ��Ρ�̫���쳣��¼�ȣ���
	2������shell���ˣ�
	3����logdb�ó��쳣��¼ʱ��Σ�
	4������ſ⣻
	���
	---------------------------------------------------------
@details
Copyright (c) 2010 Tencent. All rights reserved.

history:
<PRE>
History ID	:	1
Author		:	v_sjwang
DateTime	:	2013/10/23 17:46:10
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
using namespace std;

void LoadTabForOfflineData(const string &filename, vector< map<string, string> > &result)
{
	result.clear();
	ifstream inStream(filename.c_str());
	string sLine;

	while ( getline(inStream, sLine) )
	{
		istringstream tmpStream(sLine);
		sLine="";
		string valueTmp = "";
		map<string, string> mData;
		while( tmpStream >> valueTmp )
		{
			mData.clear();
			mData["login_time"] = valueTmp;

			valueTmp = "";
			tmpStream >> valueTmp;

			valueTmp = "";
			tmpStream >> valueTmp;
			mData["area"] = valueTmp;

			valueTmp = "";
			tmpStream >> valueTmp;
			mData["uin"] = valueTmp;

			valueTmp = "";
			tmpStream >> valueTmp;
			mData["login_ip"] = valueTmp;
		}
		result.push_back(mData);
	}
}

int main(int argc, char **argv)
{
	WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "DNF2���߷���ģ�鿪ʼ����" );
	
	int i = 0;
	int j = 0;
	int iRet = 0;
	int iSize_IcsConf = 0;	// ��ǰģ�鴦����������
	int iSize_IcsRead = 0;	// ��ȡ������Ҫ����ĵ�����
	
	RobICSOper icsOper;			// �������ݿ����(select/insert/update)
	RobAnlysIP icsRobAnlysIP;	// �ǳ�����
	
	iSize_IcsConf = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_offline"]);
	if ( iSize_IcsConf < 1 )
	{
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "���߷�����ȡ�����޶����:iSize_IcsConf[%d]", iSize_IcsConf );
		return 0;
	}
	
	Rob_Ics_Complaint Ics_Info[iSize_IcsConf];
	if ( 1 == iSize_IcsConf )
	{
		iSize_IcsRead = icsOper.ReadICSInfoOneItem(STEP_OFFLINEMODULE_RUN_ICS, Ics_Info, CRobConf::getConf()->GetIcsOper()["ics_id_send_only_one"]);
	}
	else
	{
		iSize_IcsRead = icsOper.ReadICSInfoList(STEP_OFFLINEMODULE_RUN_ICS, Ics_Info, iSize_IcsConf);
	}
	WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "��ȡ�������ݽ��:iSize_IcsConf[%d],iSize_IcsRead[%d]", iSize_IcsConf, iSize_IcsRead );
	
	// ���µ��ݴ���״̬Ϊ������
	for ( i = 0; i < iSize_IcsRead; i++ )
	{
		iRet = icsOper.UpdateICSProc(Ics_Info[i].sICSId, STEP_ROBSLOT_RUN_ICS, STEP_ROBSLOT_RUNNING_RESULT, STEP_ROBSLOT_RUNNING_STAT);
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, (0==iRet)?TLOG_LEVEL_INFO:TLOG_LEVEL_ERROR, __FUNCTION__, "���µ��ݴ���״̬Ϊ������: icsid[%s],iRet[%s]", Ics_Info[i].sICSId.c_str(), iRet );
	}
	
	char ShCmd[256] = {0};
	char OfflineFile[256] = {0};
	vector< map<string, string> > vOffSrc;
	vector<Rob_SLOT> vRobOffDisTime;	// ���߷����ó����쳣ʱ���
	// �߼�����
	for ( i = 0; i < iSize_IcsRead; i++ )
	{
		vRobOffDisTime.clear();
		sprintf(OfflineFile, "./tools/data/%s_%s.data", Ics_Info[i].sWordId.c_str(), Ics_Info[i].sUin.c_str());
		
		// SH�ű������쳣��¼
		sprintf(ShCmd, "sh ./OfflineFilter.sh %s %s %s ", Ics_Info[i].sWordId.c_str(), Ics_Info[i].sUin.c_str(), OfflineFile);
		iRet = system(ShCmd);
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "SH�ű�����:ShCmd[%s],iRet[%d]", ShCmd, iRet);
		if ( iRet < 0 )
		{
			WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "call shell fail.\n" );
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			continue;
		}
		LoadTabForOfflineData(OfflineFile, vOffSrc);
		
		if ( 0 == vOffSrc.size() )	// ���߷���,û�б���
		{
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			Ics_Info[i].siReplyResult = IntToStr(STEP_Reply_WAITING_RESULT);
			icsOper.updateICSReplyResult(Ics_Info[i]);
			continue;
		}
		
		// ��ѯ�쳣��¼��־
		iRet = icsRobAnlysIP.getRobLogInOutLogOffline(Ics_Info[i], vOffSrc, vRobOffDisTime);
		if ( 0 != iRet )	// ��ѯ�쳣��¼��־ʧ��
		{
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			continue;
		}
		
		// ����ſ⣻
		if ( vRobOffDisTime.size() > 0 )
		{
			for ( j = 0; j < vRobOffDisTime.size(); j++ )
			{
				iRet = icsOper.InsertICSRobSlotData(vRobOffDisTime[j]);
				WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, vRobOffDisTime[j].ICSID.c_str(), vRobOffDisTime[j].UserID.c_str(), TLOG_SERVICE_ID, (0==iRet)?TLOG_LEVEL_INFO:TLOG_LEVEL_ERROR, __FUNCTION__, "����ǳ��������: index[%s],icsid[%s],iRet[%s]\n", j, vRobOffDisTime[j].ICSID.c_str(), iRet );
			}
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, STEP_ROBSLOT_RUN_ICS, STEP_ROBSLOT_RUN_RESULT_TURE, STEP_ROBSLOT_RUN_STAT_SUCCESS);
			Ics_Info[i].siReplyResult = IntToStr(STEP_Reply_WAITING_RESULT);
			icsOper.updateICSReplyResult(Ics_Info[i]);
		}
		else
		{
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			Ics_Info[i].siReplyResult = IntToStr(STEP_Reply_WAITING_RESULT);
			icsOper.updateICSReplyResult(Ics_Info[i]);
		}
	}
	return 0;
}
