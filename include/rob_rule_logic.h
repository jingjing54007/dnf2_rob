#ifndef __ROB_RULE_LOGIC_H__
#define __ROB_RULE_LOGIC_H__

#include <iostream>
#include "msg.h"
#include "glog.h"
#include "log.h"

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
	int getRoleItemFlowList(std::vector< Item_Flow_List > &rob_Item_List,std::vector< Rob_SLOT> &rob_data);

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




	CLog *RuleLOG;
	GLog RuleGet;
  protected:
	void LoadItemProcList();
  private:
	string ROBRULELOG;
	std::map<string,string> vItemProList;
	std::map<string,string> vItemValueList;
	string iItemValueType;
	Item_Flow_List struTempItemFlow;
	Item_Trace_List struTempItemTrace;
	int iRet ;
};

#endif