/**
@file rob_core_offline_module.cpp
@brief 
	---------------------------------------------------------
	盗号自助分析功能----离线模型分析处理
	1、抽取待离线分析处理的单据，如登出分析时未通过，包括（无盗号时间段、太多异常登录等）；
	2、离线shell过滤；
	3、查logdb得出异常登录时间段；
	4、入盗号库；
	完毕
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
	WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "DNF2离线分析模块开始运行" );
	
	int i = 0;
	int j = 0;
	int iRet = 0;
	int iSize_IcsConf = 0;	// 当前模块处理单据数限制
	int iSize_IcsRead = 0;	// 读取到的需要处理的单据数
	
	RobICSOper icsOper;			// 盗号数据库操作(select/insert/update)
	RobAnlysIP icsRobAnlysIP;	// 登出分析
	
	iSize_IcsConf = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_offline"]);
	if ( iSize_IcsConf < 1 )
	{
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "离线分析读取单据限额错误:iSize_IcsConf[%d]", iSize_IcsConf );
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
	WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "读取待处理单据结果:iSize_IcsConf[%d],iSize_IcsRead[%d]", iSize_IcsConf, iSize_IcsRead );
	
	// 更新单据处理状态为处理中
	for ( i = 0; i < iSize_IcsRead; i++ )
	{
		iRet = icsOper.UpdateICSProc(Ics_Info[i].sICSId, STEP_ROBSLOT_RUN_ICS, STEP_ROBSLOT_RUNNING_RESULT, STEP_ROBSLOT_RUNNING_STAT);
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, (0==iRet)?TLOG_LEVEL_INFO:TLOG_LEVEL_ERROR, __FUNCTION__, "更新单据处理状态为处理中: icsid[%s],iRet[%s]", Ics_Info[i].sICSId.c_str(), iRet );
	}
	
	char ShCmd[256] = {0};
	char OfflineFile[256] = {0};
	vector< map<string, string> > vOffSrc;
	vector<Rob_SLOT> vRobOffDisTime;	// 离线分析得出的异常时间段
	// 逻辑处理
	for ( i = 0; i < iSize_IcsRead; i++ )
	{
		vRobOffDisTime.clear();
		sprintf(OfflineFile, "./tools/data/%s_%s.data", Ics_Info[i].sWordId.c_str(), Ics_Info[i].sUin.c_str());
		
		// SH脚本过滤异常登录
		sprintf(ShCmd, "sh ./OfflineFilter.sh %s %s %s ", Ics_Info[i].sWordId.c_str(), Ics_Info[i].sUin.c_str(), OfflineFile);
		iRet = system(ShCmd);
		WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "SH脚本过滤:ShCmd[%s],iRet[%d]", ShCmd, iRet);
		if ( iRet < 0 )
		{
			WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, Ics_Info[i].sICSId.c_str(), Ics_Info[i].sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "call shell fail.\n" );
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			continue;
		}
		LoadTabForOfflineData(OfflineFile, vOffSrc);
		
		if ( 0 == vOffSrc.size() )	// 离线分析,没有被盗
		{
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			Ics_Info[i].siReplyResult = IntToStr(STEP_Reply_WAITING_RESULT);
			icsOper.updateICSReplyResult(Ics_Info[i]);
			continue;
		}
		
		// 查询异常登录日志
		iRet = icsRobAnlysIP.getRobLogInOutLogOffline(Ics_Info[i], vOffSrc, vRobOffDisTime);
		if ( 0 != iRet )	// 查询异常登录日志失败
		{
			icsOper.UpdateICSProc(Ics_Info[i].sICSId, (ApplyAnaylseStep)StrToInt(Ics_Info[i].sICSAnaylseStep), (ApplyAnaylseResult)StrToInt(Ics_Info[i].sICSAnaylseResult), (ApplyAnaylseStat)StrToInt(Ics_Info[i].sICSAnaylseStat));
			continue;
		}
		
		// 入盗号库；
		if ( vRobOffDisTime.size() > 0 )
		{
			for ( j = 0; j < vRobOffDisTime.size(); j++ )
			{
				iRet = icsOper.InsertICSRobSlotData(vRobOffDisTime[j]);
				WriteLogsRUN(TLOG_TYPE_ROBOFFLINEANALYSE, vRobOffDisTime[j].ICSID.c_str(), vRobOffDisTime[j].UserID.c_str(), TLOG_SERVICE_ID, (0==iRet)?TLOG_LEVEL_INFO:TLOG_LEVEL_ERROR, __FUNCTION__, "插入登出分析结果: index[%s],icsid[%s],iRet[%s]\n", j, vRobOffDisTime[j].ICSID.c_str(), iRet );
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
