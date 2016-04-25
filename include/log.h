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
	Description:	打开文件
	@param	[in]	LogFile
	@return			void
	@exception		none
	*/
	void OpenLog(const char * LogFile);


	/**
	Description:	写日志
	@param	[in]	LogStr
	@param	[in]	
	@return			void
	@exception		none
	*/
	void WriteLog(const char *LogStr,...);


	/**
	Description:	关闭文件
	@return			void
	@exception		none
	*/
	void CloseLog();
	
	/**
	Description:	日志开关
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

