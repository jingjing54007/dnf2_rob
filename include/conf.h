/**
@file conf.h

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		Darrenqiu
@version	1.0
@date		2010/10/09

history:
<PRE>
History ID	:	1
Author    	:	Darrenqiu
DateTime	:	2010/10/09
Description :	Create
</PRE>
*/

#ifndef __CONF_H__
#define	__CONF_H__


#include <map>
#include <vector>
#include <string>

#include "log.h"


/** 
@class CRobConf
@brief 
@details 
*/
class CRobConf
{
public:
	static CRobConf* getConf();
protected:
	
private:
	//conf读取，static的意思，防止重复获取？
	static CRobConf* RobConf;
	std::string MAKECONFLOG;
	std::string CONFPATH;
public:
	CRobConf();
	~CRobConf();

	//调试日志
	static CLog* ConfLOG;
	//读取的文档的params
	std::vector<std::string> params_table;
	
	//ICS的单据的信息和过滤配置
	std::map< std::string, std::string > Ics_Valid;
	
	//业务信息配置
	std::map< std::string, std::string > GroupInfo;
	
	//权限人白名单
	std::map< std::string, std::string > Authority;

	//ICS的单据的信息和过滤配置
	std::map< std::string, std::string > Ics_Oper;

}; //end CRobConf

#endif	//__CONF_H__

