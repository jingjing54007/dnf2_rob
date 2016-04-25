/**
@file log.h

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		Darrenqiu
@version	1.0
@date		2010/10/09

history:
<PRE>
History ID	:	1
Author    	:	asherzhou
DateTime	:	2011/04/29
Description :	Create
</PRE>
*/

#ifndef __LOG_H__
#define	__LOG_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include "msg.h"
//#include "conf.h"


///////////////////////////////tlog ID define///////////////////////////////////////////
#define DEFAULTTLOG "./cfg/tlog.xml"

#define TLOG_TYPE_DBMANAGER			"71_62"		// �ӿ�ִ��ǰ�����

#define TLOG_TYPE_ROBICSFILTER		"71_65"		// ���ݹ���ģ�飬����״̬��־
#define TLOG_TYPE_ROBLOGOUTANALYSE	"71_66"		// �ǳ�����ģ�飬����״̬��־
#define TLOG_TYPE_ROBITEMFLOW		"71_67"		// ��ˮ����ģ�飬����״̬��־
#define TLOG_TYPE_ROBITEMTRACE		"71_68"		// ��Ʒ׷��ģ�飬����״̬��־
#define TLOG_TYPE_ROBITEMSEND		"71_69"		// ��Ʒ����ģ�飬����״̬��־
#define TLOG_TYPE_ROBREPLYAUTO		"71_70"		// ���Ŷ��λظ�ϵͳ
#define TLOG_TYPE_ROBOFFLINEANALYSE	"71_152"	// �������߷���ģ��

#define TLOG_TYPE_ROBICSOPER		"71_63"		// ���ݲ����࣬�߼�������־
#define TLOG_TYPE_ROBANLYSVALID		"71_71"		// ��Ч��������࣬�߼�������
#define TLOG_TYPE_ROBRULELOGIC		"71_72"		// �߼�������־
#define TLOG_TYPE_ROBANLYSIP		"71_73"		// IP�����࣬������־
#define TLOG_TYPE_ROBCONF			"71_74"		// rob�����ļ�������־


//#define TLOG_SERVICE_ID		CRobConf::getConf()->getGroupInfo()["group_id"].c_str()
#define TLOG_SERVICE_ID		"148"

#define TLOG_LEVEL_DEBUG	0
#define TLOG_LEVEL_INFO		1
#define TLOG_LEVEL_WARNING	2
#define TLOG_LEVEL_ERROR	3

#define TLOG_LEVEL_CONF		10


static int initFlg = 0;						// @add by v_sjwang 2012/10/30 16:39:17
static std::string iServiceId = "301";		// @add by v_sjwang 2012/10/30 16:39:30
static int tlogLevelSign[4] = { 1, 1, 1, 1 };
static char *tlogLevelArr[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};

//////////////////////////////////////////////////////////////////////////
#ifdef __cpluscplus
#define STRING_API extern
#else
#define STRING_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	STRING_API  void WriteLogs(char* logId,const char *LogStr,...);
	STRING_API  void WriteLogsT(char* logId,const char *LogStr,...);
	STRING_API  void WriteLogsC(char* conf,char* logId,const char *LogStr,...);
	STRING_API  void WriteLogsCT(char* conf,char* logId,const char *LogStr,...);

	//STRING_API  void WriteLogsIDIPStart(char* logId, const char *ics_id, const char *uin, const char *serviceId, const char *idipId, const char *Parms, ...);
	//STRING_API  void WriteLogsIDIPEnd(char* logId, const char *ics_id, const char *uin, const char *serviceId, const char *idipId, const char *dataNum, const char *errCode, const char *dataResult, ...);
	STRING_API  void WriteLogsRUN(char* logId, const char *ics_id, const char *uin, const char *serviceId, int logLevel, const char *FuncName, const char *LogStr,...);
#ifdef __cplusplus
}
#endif
/** 
@class CLog
@brief 
@details 
*/
class CLog
{
public:
	
	/**
	Description:	Default constructor
	@exception		none
	*/
	CLog();
	CLog(const char * LogFile);
	/**
	Description:	Destructor
	*/
	~CLog();

	/**
	Description:	���ļ�
	@param	[in]	LogFile
	@return			void
	@exception		none
	*/
	void OpenLog(const char * LogFile);


	/**
	Description:	д��־
	@param	[in]	LogStr
	@param	[in]	
	@return			void
	@exception		none
	*/
	void WriteLog(const char *LogStr,...);


	/**
	Description:	�ر��ļ�
	@return			void
	@exception		none
	*/
	void CloseLog();
	
	/**
	Description:	��־����
	@return			void
	@exception		none
	*/
	void LogOn();
	void LogOff();

protected:	
	
private:
	FILE  *pstFile;
	int   switchLock;


}; //end CLog


#endif	//__LOG_H__

