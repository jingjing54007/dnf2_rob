/**
@file rob_ics_oper.h
@brief 
@details	Copyright (c) 2011 Tencent. All rights reserved.
@author		asherzhou
@version	2.0
@date		2011/05/20

history:
<PRE>
History ID	:	2
Author    	:	asherzhou
DateTime	:	2011/05/21
Description :	Create
</PRE>
*/

#ifndef __ROB_ICS_OPER_H__
#define	__ROB_ICS_OPER_H__

#include <iostream>
#include <string>
#include "comm_datetime.h"
#include "comm_strings.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"
#include "log.h"


class RobICSOper
{

public:
	/**
	Description:	Default constructor
	@exception		none
	*/
	RobICSOper();
	~RobICSOper();

	CLog *ValidLOG;
	GLog ICS_Oper;

	/**
	Description:	����tbICSInfo��ȡ
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	unsigned int ReadICSInfoList(ApplyAnaylseStep enumApplyAnaylseStep,Rob_Ics_Complaint get_ics_info[],int get_size);

	/**
	Description:	�����ڴ����
	@exception		none
	@return 		int �����ӿڷ��ؽ��
	*/

	int SaveAutoHalfIcsInfo(Rob_Ics_Complaint& present_item_res, map<string,string>& input_params);

	/**
	Description:	����tbICSInfo�ĸ��£���Ҫ��״̬�����������ĸ���
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int UpdateICSProc(std::string sIcsID,ApplyAnaylseStep enumApplyAnaylseStep,ApplyAnaylseResult enumApplyAnaylseResult,ApplyAnaylseStat enumApplyAnaylseStat);


	/**
	Description:	����tbICSRoleLogout����
	@desc			���������ڶ�ȡ�쳣��¼ʱ��β���
	@exception		none
	*/
	int InsertICSRobSlotData(Rob_SLOT& robSlotTime);
	

	/**
	Description:	����tbICSRoleLogout��ȡ
	@desc			���������ڶ�ȡ�쳣��¼ʱ��λ�ȡ
	@exception		none
	*/
	int ReadICSRobSlot(std::vector < Rob_SLOT > &robSlotList,std::string sServiceId,std::string sICS_Id);
	/**
	Description:	����tbICSRoleLogout����
	@desc			���������ڵ����н׶�
	@exception		none
	*/
	int UpdateICSRobSlotProc(std::string sServiceId,std::string sICS_Id, ApplySlotStat eRunStat);


	/**
	Description:	����tbICSItemFlow����
	@desc			�����������쳣��¼ʱ����ڵ���Ʒ��ˮ����
	@exception		none
	*/
	int InsertICSRobItemFlowData(std::vector < Item_Flow_List > &robItemFlow,std::string sServiceId);




	/**
	Description:	����tbICSItemStats��ͳ�ƿ�ĳ�ʼ������
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int AddICSItemStatsProc(std::string sIcsID,std::string sServiceID);


	/**
	Description:	����tbICSItemStats��ͳ�ƿ��
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int UpdateICSItemStatsProc(std::string sUpdateType,vector <string>& vUpdateNum,std::string sIcsId);

	/**
	Description:	����tbICSInfo��״̬�Ļ�����Ϣ�ĸ���---�ڹ���ϵͳ֮ǰ
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int UpdateICSRoleInfo(Rob_Ics_Complaint& new_ics_info);
	
	/**
	Description:	����tbICSItemFlow��ȡ---����Ʒ׷��ϵͳ֮ǰ
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int ReadICSItemListProc(vector < Item_Flow_List >& robItemFlowInfo,std::string sIcsId);


	/**
	Description:	����UpdateICSItemFlow״̬�ġ�ȫ���ݡ��ı��
	@desc			���������ڸ�������µ�״̬�ġ�ȫ���ݡ��ı��
	@exception		none
	*/
	int UpdateICSItemFlow(std::string sIcsId,std::string sServiceID,ApplyItemFlowStat stat,ApplyItemFlowResult res);



	/**
	Description:	����tbICSItemTrace����
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int InsertICSItemTrace(vector < Item_Trace_List >& robItemTrace);

private:
	std::string ICSOPERLOG;
	int iRet;
};



#endif