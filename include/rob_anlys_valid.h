/**
@file rob_anlys_valid.h

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		Darrenqiu
@version	1.0
@date		2010/10/22

history:
<PRE>
History ID	:	2
Author    	:	asherzhou
DateTime	:	2011/05/01
Description :	Create
</PRE>
*/

#ifndef __ROB_ANLYS_VALID_H__
#define	__ROB_ANLYS_VALID_H__


#include <iostream>
#include "glog.h"
#include "log.h"
#include "comm_datetime.h"
#include "comm_value.h"
#include "conf.h"
#include "msg.h"
/** 
@class RobRuleValid

	-----------------------------------------
	单据合法性验证普遍性规则：
	1. 等级小于role_level不合法；
	2. 申诉超过create_due不合法；
	3. 最后登陆时间小于被盗时间不合法；
	4. receive_doble天内重复来单不受理；
	5. 角色被封冻大于block_day不合法。
	-----------------------------------------

@brief 
@details 
*/
class RobRuleValid
{
public:
	/**
	Description:	Default constructor
	@exception		none
	*/
	RobRuleValid();

	/**
	Description:	Destructor
	*/
	virtual ~RobRuleValid();

/**************************V1.01************************************/
	/**
	Description:	玩家的角色基本信息更新
	@param	[in]	info
	@return			int
	@exception		none
	*/
	virtual int DoICSSetRoleInfoPro(Rob_Ics_Complaint& ics_info);

	/**
	Description:	验证单据的合法性
	@param	[in]	info
	@return			int
	@exception		none
	*/
	virtual int DoICSCheckPro(Rob_Ics_Complaint& ics_info);

	
	

	
/**************************V1.00************************************/
	/**
	Description:	验证单据的合法性
	@param	[in]	info
	@return			int
	@exception		none
	*/
	virtual int DoICSCheck(ics_info& info);


	/**
	Description:	根据业务的策略是否过滤高价值物品
	@param	[in]	info
	@return			in_size，过滤前的物品股总数 out_size，过滤后的物品的总数 fliter_item_flag 是否进行高价值物品过滤的标志位
	@exception		none
	*/
	virtual int DoGetItemsToPresent(spoil_info all_item_info[],spoil_info get_item_info[],const unsigned int in_size,unsigned int& out_size,int fliter_item_flag);

	/**
	Description:	设定各个物品是否转人工处理；
	@param			[in]	info
	@				(a):    物品是否叠加
	@return			int     接口正确与否
	@exception		none
	*/
	virtual int DoSetItemsToPresent(spoil_info all_item_info[],const unsigned int in_size);
	

	
	CLog *ValidLOG;
	GLog valid_parse;


	



protected:
	/**	返回LEVEL的ics的check过滤函数
	*   params:		ics_info
	* 	return:		int
	*/
	int ProCheckRoleLevel(Rob_Ics_Complaint& info);
	/**	返回BLOCK的ics的check过滤函数
	*   params:		ics_info
	* 	return:		int
	*/
	int ProCheckBlockTime(Rob_Ics_Complaint& info);

	/**  被盗时间超过申诉要求, 普通用户暂定为15天，VIP为20天
	* 		params:		ics_info
	* 		return:		int
	*/
	int ProCheckRobedTime(Rob_Ics_Complaint& info);
	/**  被盗时间超过最后登录的时间, 不符合规则
	* 		params:		ics_info
	* 		return:		int
	*/
	int ProCheckLastTime(Rob_Ics_Complaint& info);
	/**   receive_doble天内重复来单不受理； 普通用户暂定为180天，VIP为90天
	* 		params:		ics_info
	* 		return:		int
	*/
	int ProCheckDobleQuest(Rob_Ics_Complaint& info);


	//查找vip信息
	int ProCheckVIPInfo(Rob_Ics_Complaint& info);//add by v_yfzhu 20121109


private:
	string MAKEVALIDLOG;
	int  iRet;
	
}; //end RobIcsValid


#endif	//__ROB_ANLYS_VALID_H__
