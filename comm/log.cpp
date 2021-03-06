#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>
#include "fcntl.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <algorithm>
#include <stack>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iconv.h>
#include "pal/tos.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "tloghelp/tlogload.h"
#include "log.h"
#include "conf.h"
#include "comm_strings.h"

using namespace std;

CLog::CLog()
{
	switchLock = 0;
	pstFile = stdout;
}
/**
*	调试日志开关
*  add by asherzhou
*
*/
CLog::CLog(const char * LogFile)
{
	switchLock = 0;
	OpenLog(LogFile);
}
/**
*	调试日志开关
*  add by asherzhou
*
*/
CLog::~CLog()
{
	if(NULL!=pstFile)
	{
		CloseLog();
	}
}
void CLog::OpenLog(const char * LogFile)
{
		unsigned long	file_size = 0;
		struct	stat	file_stats;
		stat(LogFile, &file_stats);
		file_size = file_stats.st_size;
	
		if(file_size > 1024*1024*100)
		{
			pstFile = fopen(LogFile, "w");//重新开始写
		}
		else
		{
			pstFile = fopen(LogFile, "a+");
		}
}

void CLog::WriteLog(const char *LogStr, ...)
{
	//日志开关add by asherzhou
	if(switchLock)
	{
		time_t          curr;
		struct tm		current_time;
		char            time_buffer[128];

		curr = time(NULL);
		localtime_r(&curr, &current_time);
		strftime(time_buffer, 128, "%Y-%m-%d %H:%M:%S", &current_time);
		//fprintf(pstFile, "[%s|%s|%s]",time_buffer,ICS_STRING_CODE.c_str(),ICS_UIN_CODE.c_str());
		fprintf(pstFile, "[%s]",time_buffer);
		va_list ap;
		va_start(ap, LogStr);
		vfprintf(pstFile, LogStr, ap);
		va_end(ap);
		fflush(pstFile);//刷新内存
	}
}

void CLog::CloseLog()
{
	fclose(pstFile);
	pstFile = NULL;
}
void CLog::LogOn()
{
	switchLock=1;
}
void CLog::LogOff()
{
	switchLock=0;
}

#ifdef __cplusplus
extern "C"
{
#endif

	void WriteLogs(char* logId,const char *LogStr,...)
	{
		char buf[1024*100];
		va_list ap;
		va_start(ap, LogStr);
		vsprintf(buf, LogStr, ap);
		va_end(ap);

		string theLogId = logId;
		int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
		int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());

		int iRet = 0;
		LPTLOGCTX			pstLogCtx;
		LPTLOGCATEGORYINST	pstCat;

		pstLogCtx = tlog_init_from_file(DEFAULTTLOG);
		if(NULL == pstLogCtx)
		{
			return;		
		}
		pstCat = tlog_get_category(pstLogCtx, "test");
		if (NULL == pstCat)
		{
			return;
		}

		tlog_info_bin(pstCat, theSysId,theSerId,0,0, buf,strlen(buf));

		tlog_fini_ctx(&pstLogCtx);
	}

	void WriteLogsT(char* logId,const char *LogStr,...)
	{
		char buf[1024*100];
		va_list ap;
		va_start(ap, LogStr);
		vsprintf(buf, LogStr, ap);
		va_end(ap);

		string theLogId = logId;
		int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
		int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());

		time_t tCurTime = time(0);
		tm* pTime = localtime(&tCurTime);
		char aryTime[64] = {0};
		strftime(aryTime, 64, "%Y-%m-%d %H:%M:%S", pTime);
		char logStr[1024*100];
		sprintf(logStr,"wlp_%s|%s|%s",logId,aryTime,buf);//logid,time,defined desc

		int iRet = 0;
		LPTLOGCTX			pstLogCtx;
		LPTLOGCATEGORYINST	pstCat;

		pstLogCtx = tlog_init_from_file(DEFAULTTLOG);
		if(NULL == pstLogCtx)
		{
			return;		
		}
		pstCat = tlog_get_category(pstLogCtx, "test");
		if (NULL == pstCat)
		{
			return;
		}

		tlog_info_bin(pstCat, theSysId,theSerId,0,0, logStr,strlen(logStr));

		tlog_fini_ctx(&pstLogCtx);
	}

	void WriteLogsC(char* conf,char* logId,const char *LogStr,...)
	{
		char buf[1024*100];
		va_list ap;
		va_start(ap, LogStr);
		vsprintf(buf, LogStr, ap);
		va_end(ap);

		string theLogId = logId;
		int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
		int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());

		int iRet = 0;
		LPTLOGCTX			pstLogCtx;
		LPTLOGCATEGORYINST	pstCat;

		pstLogCtx = tlog_init_from_file(conf);
		if(NULL == pstLogCtx)
		{
			return;		
		}
		pstCat = tlog_get_category(pstLogCtx, "test");
		if (NULL == pstCat)
		{
			return;
		}

		tlog_info_bin(pstCat, theSysId,theSerId,0,0, buf,strlen(buf));

		tlog_fini_ctx(&pstLogCtx);
	}

	void WriteLogsCT(char* conf,char* logId,const char *LogStr,...)
	{
		char buf[1024*100];
		va_list ap;
		va_start(ap, LogStr);
		vsprintf(buf, LogStr, ap);
		va_end(ap);

		string theLogId = logId;
		int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
		int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());

		time_t tCurTime = time(0);
		tm* pTime = localtime(&tCurTime);
		char aryTime[64] = {0};
		strftime(aryTime, 64, "%Y-%m-%d %H:%M:%S", pTime);
		char logStr[1024*100];
		sprintf(logStr,"%s %s",aryTime,buf);

		int iRet = 0;
		LPTLOGCTX			pstLogCtx;
		LPTLOGCATEGORYINST	pstCat;

		pstLogCtx = tlog_init_from_file(conf);
		if(NULL == pstLogCtx)
		{
			return;		
		}
		pstCat = tlog_get_category(pstLogCtx, "test");
		if (NULL == pstCat)
		{
			return;
		}

		tlog_info_bin(pstCat, theSysId,theSerId,0,0, logStr,strlen(logStr));

		tlog_fini_ctx(&pstLogCtx);
	}

	//void WriteLogsIDIPStart(char* logId, const char *ics_id, const char *uin, const char *serviceId, const char *idipId, const char *Parms, ...)
	//{
	//	if ( NULL == logId || NULL == ics_id || NULL == uin || NULL == serviceId || NULL == idipId || NULL == Parms )
	//		return;
	//	char buf[1024*100];
	//	va_list ap;
	//	va_start(ap, Parms);
	//	vsprintf(buf, Parms, ap);
	//	va_end(ap);
	//
	//	string theLogId = logId;
	//	int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
	//	int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());
	//
	//	time_t tCurTime = time(0);
	//	tm* pTime = localtime(&tCurTime);
	//	char aryTime[64] = {0};
	//	strftime(aryTime, 64, "%Y-%m-%d %H:%M:%S", pTime);
	//	char logStr[1024*100];
	//	sprintf(logStr,"wlp_%s|%s|%s|%s|%s|%s|[%05d]%s\n", logId, aryTime, ics_id, uin, serviceId, idipId, strlen(buf), buf);//logid,time,defined desc
	//
	//	int iRet = 0;
	//	LPTLOGCTX			pstLogCtx;
	//	LPTLOGCATEGORYINST	pstCat;
	//
	//	pstLogCtx = tlog_init_from_file(DEFAULTTLOG);
	//	if(NULL == pstLogCtx)
	//	{
	//		return;		
	//	}
	//	pstCat = tlog_get_category(pstLogCtx, "test");
	//	if (NULL == pstCat)
	//	{
	//		return;
	//	}
    //
	//	tlog_info_bin(pstCat, theSysId,theSerId,0,0, logStr,strlen(logStr));
    //
	//	tlog_fini_ctx(&pstLogCtx);
	//}
	//void WriteLogsIDIPEnd(char* logId, const char *ics_id, const char *uin, const char *serviceId, const char *idipId, const char *dataNum, const char *errCode, const char *dataResult, ...)
	//{
	//	if ( NULL == logId || NULL == ics_id || NULL == uin || NULL == serviceId || NULL == idipId || NULL == dataNum || NULL == errCode || NULL == dataResult )
	//		return;
	//	char buf[1024*100];
	//	va_list ap;
	//	va_start(ap, dataResult);
	//	vsprintf(buf, dataResult, ap);
	//	va_end(ap);
	//	
	//	string theLogId = logId;
	//	int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
	//	int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());
	//
	//	time_t tCurTime = time(0);
	//	tm* pTime = localtime(&tCurTime);
	//	char aryTime[64] = {0};
	//	strftime(aryTime, 64, "%Y-%m-%d %H:%M:%S", pTime);
	//	char logStr[1024*100];
	//	sprintf(logStr,"wlp_%s|%s|%s|%s|%s|%s|%s|%s|[%05d]%s\n", logId, aryTime, ics_id, uin, serviceId, idipId, dataNum, errCode, strlen(buf), buf);//logid,time,defined desc
	//	
	//	int iRet = 0;
	//	LPTLOGCTX			pstLogCtx;
	//	LPTLOGCATEGORYINST	pstCat;
	//
	//	pstLogCtx = tlog_init_from_file(DEFAULTTLOG);
	//	if(NULL == pstLogCtx)
	//	{
	//		return;		
	//	}
	//	pstCat = tlog_get_category(pstLogCtx, "test");
	//	if (NULL == pstCat)
	//	{
	//		return;
	//	}
    //
	//	tlog_info_bin(pstCat, theSysId,theSerId,0,0, logStr,strlen(logStr));
    //
	//	tlog_fini_ctx(&pstLogCtx);
	//}
	void WriteLogsRUN(char* logId, const char *ics_id, const char *uin, const char *serviceId, int logLevel, const char *FuncName, const char *LogStr,...)
	{
		if ( NULL == logId || NULL == ics_id || NULL == uin || NULL == serviceId || NULL == FuncName || NULL == LogStr )
			return;
		if ( logLevel >= TLOG_LEVEL_CONF )	// 添加 日志级别、业务ID 初始化 @add by v_sjwang 2012/10/30 16:38:57
		{
			logLevel -= TLOG_LEVEL_CONF;
		}
		else if ( initFlg == 0 )
		{
			iServiceId = CRobConf::getConf()->GetGroupInfo()["group_id"];
			
			tlogLevelSign[0] = StrToInt(CRobConf::getConf()->GetLogInfo()["DEBUG"  ]);
			tlogLevelSign[1] = StrToInt(CRobConf::getConf()->GetLogInfo()["INFO"   ]);
			tlogLevelSign[2] = StrToInt(CRobConf::getConf()->GetLogInfo()["WARNING"]);
			tlogLevelSign[3] = StrToInt(CRobConf::getConf()->GetLogInfo()["ERROR"  ]);
			
			initFlg = 1;
		}
		if ( logLevel < 0 || logLevel > ( sizeof(tlogLevelSign)/sizeof(int) -1 ) || tlogLevelSign[logLevel] != 1 )
			return;
		char buf[1024*100];
		va_list ap;
		va_start(ap, LogStr);
		vsprintf(buf, LogStr, ap);
		va_end(ap);
		while(1)
		{
			if ( buf[strlen(buf)-1] == '\n' )
				buf[strlen(buf)-1] = '\0';
			else
				break;
		}
		string theLogId = logId;
		int theSysId = atoi(theLogId.substr(0,theLogId.find('_')).c_str());
		int theSerId = atoi(theLogId.substr(theLogId.find('_')+1).c_str());
		
		///* 
		// * serviceId改为读取rob.xml，参数传入的serviceId不使用 @add by v_sjwang 2012/10/29 15:11:08
		// */
		//std::string iServiceId = CRobConf::getConf()->GetGroupInfo()["group_id"];
								 
		time_t tCurTime = time(0);
		tm* pTime = localtime(&tCurTime);
		char aryTime[64] = {0};
		strftime(aryTime, 64, "%Y-%m-%d %H:%M:%S", pTime);
		char logStr[1024*100];
		//sprintf(logStr,"wlp_%s|%s|%-3s|%-24s|%-10s|%-6s|%-16s|%05d|%s\n", logId, aryTime, serviceId, ics_id, uin, tlogLevelArr[logLevel], FuncName, strlen(buf), buf);
		//sprintf(logStr,"wlp_%s|%s|%-3s|%-24s|%-10s|%-7s|%s|%s\n", logId, aryTime, serviceId, ics_id, uin, tlogLevelArr[logLevel], FuncName, buf);
		
		sprintf(logStr,"wlp_%s|%s|%s|%s|%s|%s|%s|%s\n", logId, aryTime, iServiceId.c_str(), ics_id, uin, tlogLevelArr[logLevel], FuncName, buf);
	
		int iRet = 0;
		LPTLOGCTX			pstLogCtx;
		LPTLOGCATEGORYINST	pstCat;
	
		pstLogCtx = tlog_init_from_file(DEFAULTTLOG);
		if(NULL == pstLogCtx)
		{
			return;		
		}
		pstCat = tlog_get_category(pstLogCtx, "test");
		if (NULL == pstCat)
		{
			return;
		}

		tlog_info_bin(pstCat, theSysId,theSerId,0,0, logStr,strlen(logStr));

		tlog_fini_ctx(&pstLogCtx);
	}

#ifdef __cplusplus
}
#endif
