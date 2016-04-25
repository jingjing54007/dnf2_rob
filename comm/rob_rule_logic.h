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
	int getRoleItemFlowList(std::vector< Item_Flow_List > &rob_Item_List,std::vector< Rob_SLOT> &rob_data,string vip="0");

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
	/*********************************************************************
	Description:	��ȡdoRobItemSend()��Ʒ���ͽӿ�
	@				��Ϊ������ҵ����Ҫ�����������Լ������̣���ϸ�ļ�����
	@desc			��������ʱ�������µ���Ʒ���ͽӿڡ�
	@exception		none
	*/
	int doRobItemSend(std::vector< Item_Trace_List > &ItemList);
	/*********************************************************************
	Description:	��ȡdoReplayIcsResult()���ε��ݻظ�������
	@				���ݲ�ͬ�Ľ��������WEB�ӿڻظ�
	@desc			��������ʱ�������µ���������Ĵ���ظ�
	@exception		none
	*/
	int doReplayIcsResult(Rob_Ics_Complaint& autoReplayICS, string& sSource);

	/*******************************************************************************
	Description	:	�ظ��ֻ�QQ��ȫ����app���³�����ʿר��
	Params		:	ics_info	in	������Ϣ
	result		:	�ظ���� 0-�ɹ�
	exception	:	none
	*/
	int doReplayMobileApp(Rob_Ics_Complaint &ics_info, int type = 0);

	/*********************************************************************
	Description:	��¼������־
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
	Description:	���ʼ�
	@				
	@desc			
	@exception		none
	*/
	int SendWarnMail(const string& mailTitle, const string& mailContext);

	/*********************************************************************
	Description:	��ѯ��ȯ fall_outֵ
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
	std::map<string, string> mAreaConf;	// �������б�
	int iRet ;
	
};

#endif