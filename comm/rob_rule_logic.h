#ifndef __ROB_RULE_LOGIC_H__
#define __ROB_RULE_LOGIC_H__

#include <iostream>
#include "msg.h"
#include "glog.h"
#include "log.h"
#include "HttpRpc.h"
#include "comm_logmanager.h"


class RobRuleLogic
{
  public:
	/*********************************************************************
	Description:	Default constructor
	@exception		none
	*/
	RobRuleLogic();
	virtual ~RobRuleLogic();

	/*********************************************************************
	Description:	获取业务的找寻物品的list的size（）主要验证类的初始化的正确性。
	@desc			存在map<string,string> vItemProList;
	@exception		none
	*/
	int getItemListSize();
	/*********************************************************************
	Description:	获取业务的找寻物品的list是否存在制定item_id。
	@desc			存在map<string,string> vItemProList;
	@exception		none
	*/
	bool checkItemNeedProc(string iItem_id);
	/*********************************************************************
	Description:	获取业务的找寻物品的list的物品贵重性。
	@desc			存在map<string,string> vItemProList;
	@exception		none
	*/
	int getItemValueLevel(string iItem_id);
	/*********************************************************************
	Description:	获取getRoleItemFlowList()的物品流水。
	@desc			包括单据在各种情况下的读取
	@exception		none
	*/
	int getRoleItemFlowList(std::vector< Item_Flow_List > &rob_Item_List,std::vector< Rob_SLOT> &rob_data,string vip="0");

	/*********************************************************************
	Description:	获取getRobSlotMoneySub()的金钱丢失量。
	@desc			包括盗号时间段情况下的读取金钱丢失量。
	@exception		none
	*/
	int getRobSlotMoneySub(Rob_SLOT &rob_data);

	/*********************************************************************
	Description:	获取getRobItemTrace()物品追踪日志
	@				分为：NoTracking 和 ByTracking的两种情况
	@desc			包括盗号时间段情况下的读物品追踪。
	@exception		none
	*/
	int getRobItemTraceNoTracking(std::vector< Item_Trace_List > &TraceResult,std::vector< Item_Flow_List > &FlowParams);
	/*********************************************************************
	Description:	获取doRobItemSend()物品补送接口
	@				分为：各个业务需要单独来定义自己的流程，详细的见代码
	@desc			包括盗号时间段情况下的物品补送接口。
	@exception		none
	*/
	int doRobItemSend(std::vector< Item_Trace_List > &ItemList);
	/*********************************************************************
	Description:	获取doReplayIcsResult()二次单据回复处理结果
	@				依据不同的结果来发送WEB接口回复
	@desc			包括盗号时间段情况下的所有情况的处理回复
	@exception		none
	*/
	int doReplayIcsResult(Rob_Ics_Complaint& autoReplayICS, string& sSource);

	/*******************************************************************************
	Description	:	回复手机QQ安全中心app地下城与勇士专区
	Params		:	ics_info	in	单据信息
	result		:	回复结果 0-成功
	exception	:	none
	*/
	int doReplayMobileApp(Rob_Ics_Complaint &ics_info, int type = 0);

	/*********************************************************************
	Description:	记录敏感日志
	@				
	@desc			
	@exception		none
	*/
	int LogCM(const Item_Trace_List &ItemList,
			  const string& strOptType, 
			  const string& strReason, 
			  const string& strMemo,
			  const string& strFuntionName,
			  const string& iUin,
			  const string& iRoleId,
			  const string& vRoleName,
			  const string& strCmd,
			  const int& iResult,
			  const string& StrSerial);

	/*********************************************************************
	Description:	发邮件
	@				
	@desc			
	@exception		none
	*/
	int SendWarnMail(const string& mailTitle, const string& mailContext);

	/*********************************************************************
	Description:	查询点券 fall_out值
	@				
	@desc			
	@exception		none
	*/
	int GetFall_Out(const string &uin);
	
	
	CLog *RuleLOG;
	GLog RuleGet;
	CLogManager RobCMLog;
  protected:
	void LoadItemProcList();
	void LoadReplyProcList();
  private:
	string ROBRULELOG;
	std::map<string,string> vItemProList;
	std::map<string,string> vItemValueList;
	std::map<string,string> mReplyContentList;
	string iItemValueType;
	Item_Flow_List struTempItemFlow;
	Item_Trace_List struTempItemTrace;
	HttpRpc _rob_httpRpc;
	HttpRpc _rob_httpRpc_R;
	string _rob_httpRpc_proxy;
	string _rob_httpRpc_url;
	string _rob_httpRpc_proxy_R;
	string _rob_httpRpc_url_R;
	//added by v_zhtang on 2012-12-26
	HttpRpc _rob_httpRpc_T;
	string _rob_httpRpc_proxy_T;
	string _rob_httpRpc_url_T;
	//end add;
	std::map<string, string> mAreaConf;	// 服务器列表
	int iRet ;
	
};

#endif