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
	*	����GetLog,��Ӧ��SAP��ܵĲ�ѯ��������д��std::vector
	*	��Ҫ�����þ��ǵ���SAP�Ľӿڲ�
	*/
	int GetLog(
		const ics_log_header& header,
		const int id,
		std::map<string,string>& theParam,
		std::vector< std::map<std::string,std::string> >& result);
	 		
	 
	/* add by asherzhou
	*	GetLogicLog,��Ӧ��SAP��ܵĲ�ѯ��������д��std::vector
	*	��Ҫ�����þ��ǵ���SAP���߼���
	*   ������Ҫ������ʲô��������鿴rob.conf��ȷ������
	*/
	int GetFunction(
		const ics_log_header& header,
		std::map<string,string>& theParamIn,
		const char* functionName,
		std::vector< map<string,string> >& result);

protected:

private:
	//������־
	CLog*  TraceLOG;
	std::string	MAKEFACELOG;
}; 


#endif	//__GLOG_H__
