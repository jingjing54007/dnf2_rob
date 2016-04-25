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
	Description:	��ȡҵ�����Ѱ��Ʒ��list��size������Ҫ��֤��ĳ�ʼ������ȷ�ԡ�
	@desc			����map<string,string> vItemProList;
	@exception		none
	*/
	int getItemListSize();
	/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list�Ƿ�����ƶ�item_id��
	@desc			����map<string,string> vItemProList;
	@exception		none
	*/
	bool checkItemNeedProc(string iItem_id);
	/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list����Ʒ�����ԡ�
	@desc			����map<string,string> vItemProList;
	@exception		none
	*/
	int getItemValueLevel(string iItem_id);
	/*********************************************************************
	Description:	��ȡgetRoleItemFlowList()����Ʒ��ˮ��
	@desc			���������ڸ�������µĶ�ȡ
	@exception		none
	*/
	int getRoleItemFlowList(std::vector< Item_Flow_List > &rob_Item_List,std::vector< Rob_SLOT> &rob_data);

	/*********************************************************************
	Description:	��ȡgetRobSlotMoneySub()�Ľ�Ǯ��ʧ����
	@desc			��������ʱ�������µĶ�ȡ��Ǯ��ʧ����
	@exception		none
	*/
	int getRobSlotMoneySub(Rob_SLOT &rob_data);

	/*********************************************************************
	Description:	��ȡgetRobItemTrace()��Ʒ׷����־
	@				��Ϊ��NoTracking �� ByTracking���������
	@desc			��������ʱ�������µĶ���Ʒ׷�١�
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