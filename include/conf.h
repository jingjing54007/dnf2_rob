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
	//conf��ȡ��static����˼����ֹ�ظ���ȡ��
	static CRobConf* RobConf;
	std::string MAKECONFLOG;
	std::string CONFPATH;
public:
	CRobConf();
	~CRobConf();

	//������־
	static CLog* ConfLOG;
	//��ȡ���ĵ���params
	std::vector<std::string> params_table;
	
	//ICS�ĵ��ݵ���Ϣ�͹�������
	std::map< std::string, std::string > Ics_Valid;
	
	//ҵ����Ϣ����
	std::map< std::string, std::string > GroupInfo;
	
	//Ȩ���˰�����
	std::map< std::string, std::string > Authority;

	//ICS�ĵ��ݵ���Ϣ�͹�������
	std::map< std::string, std::string > Ics_Oper;

}; //end CRobConf

#endif	//__CONF_H__

