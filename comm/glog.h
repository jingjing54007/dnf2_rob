/**
@file glog.h

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		Darrenqiu
@version	1.0
@date		2010/10/12

history:
<PRE>
History ID	:	1
Author    	:	Darrenqiu
DateTime	:	2010/10/12
Description :	Create
</PRE>
*/

#ifndef __GLOG_H__
#define	__GLOG_H__


#include <string>
#include <vector>
#include <map>
#include <iostream>



#include "log.h"


#define GETROLENAME 584



using namespace std;

/** 
@class GLog
@brief 
@details 
*/
class GLog
{
public:
	GLog();
	~GLog();
	
	/* add by asherzhou
	*	重载GetLog,适应新SAP框架的查询结果。结果写入std::vector
	*	主要的作用就是调用SAP的接口层
	*/
	int GetLog(
		const ics_log_header& header,
		const int id,
		std::map<string,string>& theParam,
		std::vector< std::map<std::string,std::string> >& result);
	 		
	 
	/* add by asherzhou
	*	GetLogicLog,适应新SAP框架的查询结果。结果写入std::vector
	*	主要的作用就是调用SAP的逻辑层
	*   具体需要功能项什么参数，请查看rob.conf来确定输入
	*/
	int GetFunction(
		const ics_log_header& header,
		std::map<string,string>& theParamIn,
		const char* functionName,
		std::vector< map<string,string> >& result);

protected:

private:
	//调试日志
	CLog*  TraceLOG;
	std::string	MAKEFACELOG;
}; 


#endif	//__GLOG_H__
