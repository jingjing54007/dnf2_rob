#include "rob_rule_logic.h"
#include "msg.h"
#include "iplib.h"
#include "comm_datetime.h"
#include "comm_strings.h"
#include "comm_value.h"
#include "conf.h"
#include "comm.h"
#include "rob_ics_oper.h"

#include "safe_protocol_jianling_zone.h"
#include "sec_jianling_zone_protocol.h"
#include "notify_proto.h"
#define MAXLINE 4096
using namespace SEC_JIANLING_ZONE;

RobRuleLogic::RobRuleLogic()
{
	ROBRULELOG ="./logs/"+GetCurDateDay()+"RobLogicGlog.log";
	RuleLOG = new CLog((char*)ROBRULELOG.c_str());
	RuleLOG->LogOn();
	iRet=0;
	//��ʼ��װ�����˿�
	//vItemProList;
#ifdef dnf2
	iItemValueType="0";
#endif
	mAreaConf.clear();
	vItemProList.clear();
	vItemValueList.clear();
	LoadItemProcList();
	
	_rob_httpRpc_proxy =CRobConf::getConf()->GetCommonInterface()["web_http_proxy"];
	_rob_httpRpc_url = CRobConf::getConf()->GetCommonInterface()["web_http_url"];
	_rob_httpRpc.SetProxy(_rob_httpRpc_proxy);

	_rob_httpRpc_proxy_R = CRobConf::getConf()->GetCommonInterface()["web_http_proxy_R"];
	_rob_httpRpc_url_R = CRobConf::getConf()->GetCommonInterface()["web_http_url_R"];
	_rob_httpRpc_R.SetProxy(_rob_httpRpc_proxy_R);
	//added by v_zhtang on 2012-12-26
	_rob_httpRpc_proxy_T = CRobConf::getConf()->GetCommonInterface()["web_http_proxy_T"];
	_rob_httpRpc_url_T = CRobConf::getConf()->GetCommonInterface()["web_http_url_T"];
	_rob_httpRpc_T.SetProxy(_rob_httpRpc_proxy_T);
	//end add
	mReplyContentList.clear();
	LoadReplyProcList();
}

RobRuleLogic::~RobRuleLogic()
{
	if(RuleLOG!=NULL)
	{
		delete RuleLOG;
	}
}
/*********************************************************************
	Description:	�ظ��ӿڵ�reply������list
	@desc			����map<string,string> vItemProList;
	@exception		none
*/
void RobRuleLogic::LoadReplyProcList(){
	string filepath = "./cfg/";
#ifdef dnf2
	filepath+="dnfAutoReplay.conf";
#endif
	LoadTabToIDvsName(filepath,mReplyContentList);
}

/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list
	@desc			����map<string,string> vItemProList;
	@exception		none
*/
void RobRuleLogic::LoadItemProcList(){
	/*��ȡҵ�����Ѱ��Ʒ��list*/
	/*map<string,string> inParams;
	vector < map<string, string> > theItemListResult;
	inParams["service_name"] = CRobConf::getConf()->GroupInfo["group_name"];;
	inParams["value_type"] = iItemValueType;
	int ires = RuleGet.GetFunction(inParams,"ics_item_conf_info",theItemListResult);
	if (0 != ires)
	{
		RuleLOG->WriteLog("Get LoadItemProcList Glog Interface Error Res(%d) and DataResult'Size(%d)\n",ires,theItemListResult.size());
	}
	if(theItemListResult.size()>0){
		RuleLOG->WriteLog("Get LoadItemProcList Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",ires,theItemListResult.size());
		for(int k=0;k<theItemListResult.size();k++){
			vItemProList[(char*)theItemListResult[k]["vItemID"].c_str()] = theItemListResult[k]["iItemName"];
			vItemValueList[(char*)theItemListResult[k]["vItemID"].c_str()] = theItemListResult[k]["iValueLevel"];
		}
	}*/
	/*����ֱ��LoadTab��ʵ��*/
	string filepath = "./cfg/";
#ifdef dnf2
	filepath+="robItem-NameList_dnf2.conf";
	LoadTabToIDvsName(filepath,vItemProList);
	/*if(0!=LoadTabToMap(filepath,vItemProList))
	{
		RuleLOG->WriteLog("��ȡ���������б��ļ�ʧ�ܡ�robItem-NameList_dnf2.conf��\n");

		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, \
			"��ȡ���������б��ļ�ʧ�ܡ�robItem-NameList_dnf2.conf��\n");
	}*/
	filepath = "./cfg/";
	filepath+="robItem-ValueList_dnf2.conf";

	LoadTabToIDvsName(filepath,vItemValueList);

	filepath = "./cfg/";
	filepath+="AreaList.conf";
	LoadTabToIDvsName(filepath,mAreaConf);
#endif

}
/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list��size������Ҫ��֤��ĳ�ʼ������ȷ�ԡ�
	@desc			����map<string,string> vItemProList;
	@exception		none
*/
int RobRuleLogic::getItemListSize(){
	return vItemProList.size();
}
/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list�Ƿ�����ƶ�item_id��
	@desc			����map<string,string> vItemProList;
	@exception		none
*/
bool RobRuleLogic::checkItemNeedProc(string iItem_id){
	return vItemProList.find((char*)iItem_id.c_str())==vItemProList.end()?false:true;
}
/*********************************************************************
	Description:	��ȡҵ�����Ѱ��Ʒ��list��size������Ҫ��֤�����ƾ��ʼ������ȷ�ԡ�
	@desc			����map<string,string> vItemProList;
	@exception		none
*/
int RobRuleLogic::getItemValueLevel(string iItem_id){
	return StrToInt(vItemValueList[(char*)iItem_id.c_str()]);
}

int RobRuleLogic::getRoleItemFlowList(std::vector< Item_Flow_List > &rob_Item_List,std::vector< Rob_SLOT> &rob_data,string vip){
	map<string,string> inParams;
	vector < map<string, string> > theItemListResult;
	int iSlotCouFlag = rob_data.size();
	rob_Item_List.clear();
	int cnt = 0;//ͳ�Ƹߵȼ���Ʒ

	RuleLOG->WriteLog("��ȡ��ҵ��쳣��¼������%d��\n",iSlotCouFlag);

	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, \
		"��ȡ��ҵ��쳣��¼������%d��\n",iSlotCouFlag);

	for(int j=0;j<rob_data.size();j++){
		inParams.clear();
		inParams["uin"] = rob_data[j].UserID;
		inParams["area"] = rob_data[j].WorldID;
		inParams["gid"] = rob_data[j].RoleID;
		inParams["start_time"] = rob_data[j].LoginTime;
		inParams["end_time"] = rob_data[j].LogoutTime;
		inParams["action"] ="0";
#ifdef dnf2
		inParams["gid"] = string(GbkToUtf8((char*)rob_data[j].RoleName.c_str()));
		inParams["action"] = "(0,5,9,1,15,22)";
#endif
		RuleLOG->WriteLog("--->���š�%s�����ݣ�uin=%s&area=%s&gid=%s&start_time=%s&end_time=%s\n",rob_data[j].ICSID.c_str(),\
							inParams["uin"].c_str(),\
							inParams["area"].c_str(),\
							inParams["gid"].c_str(),\
							inParams["start_time"].c_str(),\
							inParams["end_time"].c_str());
	
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,\
			"--->���š�%s�����ݣ�uin=%s&area=%s&gid=%s&start_time=%s&end_time=%s\n", rob_data[j].ICSID.c_str(), inParams["uin"].c_str(), inParams["area"].c_str(), inParams["gid"].c_str(),inParams["start_time"].c_str(),inParams["end_time"].c_str());

			ics_log_header header(rob_data[j].ICSID, rob_data[j].UserID, TLOG_SERVICE_ID);
		iRet = RuleGet.GetFunction(header,inParams,"ics_role_item_flow_query",theItemListResult);
		if (0 != iRet)
		{
			RuleLOG->WriteLog("Get getRoleItemFlowList Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theItemListResult.size());
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__,  "Get getRoleItemFlowList Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theItemListResult.size());
			continue;
		}
		if(theItemListResult.size()==0&&iSlotCouFlag!=1){
			RuleLOG->WriteLog("ѭ���Ĵ�С��%d���ж�FLAG���ݴ�С��%d��\n",theItemListResult.size(),iSlotCouFlag);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ѭ���Ĵ�С��%d���ж�FLAG���ݴ�С��%d��\n",theItemListResult.size(),iSlotCouFlag);
			iSlotCouFlag--;
			RuleLOG->WriteLog("ѭ���Ĵ�С��%d���ж�FLAG���ݴ�С��Ϊ��%d��\n",theItemListResult.size(),iSlotCouFlag);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "ѭ���Ĵ�С��%d���ж�FLAG���ݴ�С��Ϊ��%d��\n",theItemListResult.size(),iSlotCouFlag);
			continue;
		}else if(theItemListResult.size()==0&&iSlotCouFlag==1){
			RuleLOG->WriteLog("theItemListResult�Ĵ�С��0���ж�FLAG���ݴ�С��Ϊ��1��\n");
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"theItemListResult�Ĵ�С��0���ж�FLAG���ݴ�С��Ϊ��1��\n");
			return STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW;
		}else{
			RuleLOG->WriteLog("�����Ʒ��ˮ���ݽӿڷ��ء�%d�����ݴ�С��%d��\n",iRet,theItemListResult.size());
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"�����Ʒ��ˮ���ݽӿڷ��ء�%d�����ݴ�С��%d��\n",iRet,theItemListResult.size());
			for(int k=0;k<theItemListResult.size();k++){
				struTempItemFlow.iServiceID = rob_data[j].ServiceID;
				struTempItemFlow.iWordId = rob_data[j].WorldID;
				struTempItemFlow.ICSID = rob_data[j].ICSID;
				struTempItemFlow.vRobed_RoleId = rob_data[j].RoleID;
				struTempItemFlow.iRobed_Uin = rob_data[j].UserID;
				struTempItemFlow.vRobed_RoleName =  rob_data[j].RoleName;
				
				
				//����Ϊ�Ժ�汾���ӿ�
				struTempItemFlow.iItem_Type = "0";
				struTempItemFlow.iRobTo_Uin = "0";
				struTempItemFlow.vRobTo_RoleName = "0";
				struTempItemFlow.vRobTo_RoleId = "0";
				struTempItemFlow.vFlowDsc="0";
				
				//����Ϊ�Ժ�汾���ӿ�
				struTempItemFlow.iDefineProc = "1"; //��Ҫ����
#ifdef dnf2
				//a.ֻ��������ȼ���Ʒ���ߵȼ���Ʒ������---���ȫ���û�����
				//b.ֻ����ǿ���ȼ�<=15�ȼ�����Ʒ---���Ȩ���û�����
				//c.���ڼ�ǿ��Ʒ��С��15����,VIP�û����ͣ������û�������
				/*d.iDefineProc�Ķ���
					0 δ������
					1 ��Ҫ����
					2 �˹�����
					3 �������
					14 �ߵȼ���Ʒ������Ҫ�������ת�˹�
					15 �߼�ǿ��Ʒ������Ҫ�������ת�˹�
					16 ��VIP�û����޷�����VIP������Ʒ
					40 ��Ʒ��ֵ������� 
				*/
				struTempItemFlow.dtEventTime = theItemListResult[k]["dtDateTime"];
				struTempItemFlow.iItem_Num =  theItemListResult[k]["iDecNumber"];
				struTempItemFlow.iItem_Id =  theItemListResult[k]["iItemNO"];
				struTempItemFlow.iItem_Name =  vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()]==""?"0":vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()];
				RuleLOG->WriteLog("�����Ʒ��ˮ���ݽӿ�װ�����ơ�%s�����õ����ơ�%s��\n",struTempItemFlow.iItem_Name.c_str(),vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()].c_str());
				struTempItemFlow.iFlowAction =  theItemListResult[k]["iReason"];
				//�����ȼ�
				struTempItemFlow.iItem_Level = vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()]==""?"0":vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()];
				RuleLOG->WriteLog("�����Ʒ��ˮ���ݽӿ�װ����ֵ��%s�����õļ�ֵ��%s��\n",struTempItemFlow.iItem_Level.c_str(),vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()].c_str());
				struTempItemFlow.iItem_Unique_Id = theItemListResult[k]["vGuid"];
				//ǿ�����Եȼ�
				struTempItemFlow.iItem_Attribute_1 = theItemListResult[k]["iUpgrade"];
				struTempItemFlow.iDefineProc = "1";
				struTempItemFlow.iItem_Price_Value = "0";



				//�����û���
				//����Ҫ��������ת�˹�, ���ڸߵȼ���Ʒ
				//if(struTempItemFlow.iFlowAction=="9"&&struTempItemFlow.iItem_Level=="3")
				if(struTempItemFlow.iItem_Level=="3")//modify by v_yfzhu 20121126
				{
					cnt++;
					if (cnt <= 2)
					{
						struTempItemFlow.iDefineProc = "1"; //�ߵȼ���Ʒ����������2ʱϵͳ�Զ�����
					}
					else
					{
						struTempItemFlow.iDefineProc = "2"; //�ߵȼ���Ʒ���� ����2ʱ�˹�����
					}
				}

				if(StrToInt(struTempItemFlow.iItem_Attribute_1)>= 15){
					struTempItemFlow.iDefineProc = "15";  //����Ҫ��������Ʒ��ǿ������>=15
					struTempItemFlow.iDefineProc = "2";
				}
				//��VIP
				/*if(vip=="0"&&StrToInt(struTempItemFlow.iItem_Attribute_1)>0){
					struTempItemFlow.iItem_Attribute_1="0";
					struTempItemFlow.iDefineProc = "1";  //����Ҫ��������Ʒ��ǿ��
				}
				*/
				if(struTempItemFlow.iItem_Name=="0"){
					struTempItemFlow.iDefineProc = "17";  //����Ҫ����,����Ʒ���ڴ���Χ��
				}
				if(struTempItemFlow.iFlowAction=="1"||struTempItemFlow.iFlowAction=="15"||struTempItemFlow.iFlowAction=="22")
				{
					struTempItemFlow.iDefineProc = "17";  //����Ҫ����,����Ʒ���ڴ���Χ��
				}
				
#endif
				
				rob_Item_List.push_back(struTempItemFlow);
			}
		}
	}
	if(rob_Item_List.size()==0){
		return STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW;
	}
  //��Ʒ��������50
  int item_sums_toplimit=StrToInt(CRobConf::getConf()->GetIcsValid()["item_sums_toplimit"]);
  int item_count_send=0;
  for(int k=0;k<rob_Item_List.size();k++){
     if(item_count_send>item_sums_toplimit){
        rob_Item_List[k].iDefineProc="17"; //����Ҫ����,����Ʒ���ڴ���Χ��
        continue;
     }
     if(rob_Item_List[k].iDefineProc=="1"){
          item_count_send++;
     }
  }
	return 0;
}
/*********************************************************************
	Description:	��ȡgetRobSlotMoneySub()�Ľ�Ǯ��ʧ����
	@desc			��������ʱ�������µĶ�ȡ��Ǯ��ʧ����
	@exception		none
*/
int RobRuleLogic::getRobSlotMoneySub(Rob_SLOT &rob_data){

	/*��ѯ��Ʒ��Ǯ������־*/
	map<string,string> inParams;
	vector < map<string, string> > theMoneySubResult;
	string sMoney_Flag = "sumMoneySub";
	RuleLOG->WriteLog("Get getRobSlotMoneySub Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",\
							rob_data.UserID.c_str(),\
							rob_data.RoleName.c_str(),\
							rob_data.WorldID.c_str(),\
							rob_data.LoginTime.c_str(),\
							rob_data.LogoutTime.c_str());
	inParams["uin"] = rob_data.UserID;
	inParams["gid"] = rob_data.RoleID;
	inParams["area"] = rob_data.WorldID;
	inParams["start_time"] = rob_data.LoginTime;
	inParams["end_time"] = rob_data.LogoutTime;
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,rob_data.ICSID.c_str(), rob_data.UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, 
		"Get getRobSlotMoneySub Info=>|QQ:%s|RoleId:%s|Area:%s|Time_Begin:%s|Time_End:%s|\n",\
		rob_data.UserID.c_str(),\
		rob_data.RoleName.c_str(),\
		rob_data.WorldID.c_str(),\
		rob_data.LoginTime.c_str(),\
		rob_data.LogoutTime.c_str());

#ifdef dnf2
	sMoney_Flag = "sumMoneySub";
	inParams["gid"] = GbkToUtf8((char*)rob_data.RoleName.c_str());
#endif

	 ics_log_header header(rob_data.ICSID, rob_data.UserID, rob_data.ServiceID);
	iRet = RuleGet.GetFunction(header,inParams,"ics_role_money_sub_query",theMoneySubResult);
	if (0 != iRet)
	{
		RuleLOG->WriteLog("Get getRobSlotMoneySub Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theMoneySubResult.size());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data.ICSID.c_str(), rob_data.UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getRobSlotMoneySub Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theMoneySubResult.size());
		return iRet;
	}
	RuleLOG->WriteLog("Get getRobSlotMoneySub Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",iRet,theMoneySubResult.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data.ICSID.c_str(), rob_data.UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Get getRobSlotMoneySub Glog Interface Successfully Res(%d) and DataResult'Size(%d)\n",iRet,theMoneySubResult.size());
	rob_data.iMoney_Sub = theMoneySubResult[0][(char*)sMoney_Flag.c_str()]==""?"0":theMoneySubResult[0][(char*)sMoney_Flag.c_str()];
	return 0;
}

/*********************************************************************
Description:	��ȡgetRobItemTrace()��Ʒ׷����־
@				��Ϊ�� NoTracking�� ByTracking���������
@desc			��������ʱ�������µĶ���Ʒ׷�١�
@exception		none
*/
int RobRuleLogic::getRobItemTraceNoTracking(std::vector< Item_Trace_List > &TraceResult,std::vector< Item_Flow_List > &FlowParams){
	TraceResult.clear();
	RuleLOG->WriteLog("getRobItemTraceNoTracking��ȡ��ҵ���Ʒ������ˮFlowParams.size():%d",FlowParams.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "getRobItemTraceNoTracking��ȡ��ҵ���Ʒ������ˮFlowParams.size():%d",FlowParams.size());
	for(int i=0;i<FlowParams.size();i++)
	{
		struTempItemTrace.dtEventTime = FlowParams[i].dtEventTime;
		struTempItemTrace.ICSID = FlowParams[i].ICSID;
		struTempItemTrace.iFlowAction = FlowParams[i].iFlowAction;
		struTempItemTrace.iItem_Attribute_1 = FlowParams[i].iItem_Attribute_1;
		struTempItemTrace.iItem_Attribute_blob = FlowParams[i].iItem_Attribute_blob;
		struTempItemTrace.iItem_Id = FlowParams[i].iItem_Id;
		struTempItemTrace.iItem_Level = FlowParams[i].iItem_Level;
		struTempItemTrace.iItem_Name = FlowParams[i].iItem_Name;
		struTempItemTrace.iItem_Num =  FlowParams[i].iItem_Num;
		struTempItemTrace.iItem_Type =  FlowParams[i].iItem_Type;
		struTempItemTrace.iItem_Unique_Id = FlowParams[i].iItem_Unique_Id;
		struTempItemTrace.iRobed_Uin = FlowParams[i].iRobed_Uin;
		struTempItemTrace.iRobTo_Uin = FlowParams[i].iRobTo_Uin;
		struTempItemTrace.iWordId = FlowParams[i].iWordId;
		struTempItemTrace.vFlowDsc = FlowParams[i].vFlowDsc;
		struTempItemTrace.iServiceID = FlowParams[i].iServiceID;
		struTempItemTrace.vRobed_RoleId = FlowParams[i].vRobed_RoleId;
		struTempItemTrace.vRobed_RoleName = FlowParams[i].vRobed_RoleName;
		struTempItemTrace.vRobTo_RoleId =  FlowParams[i].vRobTo_RoleId==""?"0":FlowParams[i].vRobTo_RoleId;
		struTempItemTrace.vRobTo_RoleName = FlowParams[i].vRobTo_RoleName==""?"0":FlowParams[i].vRobTo_RoleName;
	
		
		
		//1 ��Ҫ�Զ�����
		//2 ������
		//3 ����Ҫ�Զ����ͣ��ֹ����͵�
		struTempItemTrace.iSendAction = "1";

		//0 ��ʼ̬
		//1 ��һ��
		//-1 ���һ�֣���ǰ��˭�����ϣ�
		struTempItemTrace.iTraceHand = "-1";
		/*
		0 ������
		1 ����׷�٣�ֱ�Ӳ���
		2 ��Ҫ���ͣ��ֹ�����
		3 ׷���н������������
		׷���н����NPC���̵꣬�ֿ�
		4 ׷����Ʒ����ʧ��
		�ݻ٣��������ӵ��ϡ���
		5 �޷�����
		*/
		struTempItemTrace.iTraceResult = "1";

#ifdef dnf2
		//struTempItemTrace.iSendAction = FlowParams[i].iItem_Level=="0"?"1":"2";
		if(FlowParams[i].iDefineProc=="1"){
			struTempItemTrace.iSendAction="1";//�Զ�����
		}else if(FlowParams[i].iDefineProc=="2"){
			struTempItemTrace.iSendAction="3";//ת�˹�
		}else{
			struTempItemTrace.iSendAction="2";//������
			continue;
		}
		struTempItemTrace.iTraceHand = "-1";
		//��ʱ������
		struTempItemTrace.iTraceResult = FlowParams[i].iDefineProc=="1"?"1":"2";
#endif
		TraceResult.push_back(struTempItemTrace);
	}
}
/*********************************************************************
	Description:	��ȡdoRobItemSend()��Ʒ���ͽӿ�
	@				��Ϊ������ҵ����Ҫ�����������Լ������̣���ϸ�ļ�����
	@desc			��������ʱ�������µ���Ʒ���ͽӿڡ�
	@exception		none
*/

int RobRuleLogic::doRobItemSend(std::vector< Item_Trace_List > &ItemList){
	map<string,string> inParams;
	vector < map<string, string> > theItemSendResult, theSendResult;
	std::vector< Item_Trace_List > vecNeedWarnItems;//��Ҫ�ʼ��澯����Ʒ�б�һ�𷢳�ȥ
	std::vector< Item_Trace_List > vecNeedWarnCMLogs; //��������־ʧ���跢�����ʼ�

	RuleLOG->WriteLog("[PHASE]:::doRobItemSend:ItemList_size()=%d\n",ItemList.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[PHASE]:::doRobItemSend:ItemList_size()=%d\n",ItemList.size());
	for(int i=0;i<ItemList.size();i++)
	{
		inParams.clear();
#ifdef dnf2
/*
Cdkey�һ����߽ӿ�

cmd=1003&area=1&send_charac_name=system&receive_charac_no=16584622&item_id=2360&endurance=7&item_type=2&item_num=3&upgrade=2&seal_flag=1

result=0&area=1&receive_charac_no=16584622

˵����result��0��ʾ�����ɹ���< 0��ʾ����ʧ�� 
���͵��ߵ����ͣ���item_type�Ķ������£� typedef enum ITEM_TYPE { T_COIN = 0, //��Ǯ 
											  T_HONOUR = 1, //�ƺ� 
											  T_EQUIP = 2, //װ�� 
											  T_EXPENDABLE = 3, //����Ʒ 
											  T_AVATAR = 4, //װ�� 
											  //T_PET = 5 
											  �ͳ�����ʹ��ר�ŵĽӿ�1020 
											  };
ע�⣺ ǧ��Ҫ���������ͣ�������������������˵ģ�����ɱȽϴ����ʧ 
seal_flag			�Ƿ�ɽ��ס�Ĭ��Ϊ0���ɽ��� 
send_charac_name	���url encode���� 
endurance			��ͨ��Ʒ (�;ö�)/�����(0: ���, 1: �, 2: �շ�)/װ��(װ���ѡ������) 
upgrade				��Ʒǿ��ֵ������
*/		RuleLOG->WriteLog("[PHASE]:::doRobItemSend:icsid=%s\n",ItemList[i].ICSID.c_str());
		string send_charac_name = "����ϵͳ�һ���Ʒ";
		inParams["area"] = ItemList[i].iWordId;
		inParams["send_charac_name"] = string(GbkToUtf8((char*)send_charac_name.c_str()));
		inParams["receive_charac_no"] = ItemList[i].vRobed_RoleId;
		inParams["item_id"] = ItemList[i].iItem_Id;
		inParams["endurance"] = "0";
		inParams["item_type"] = ItemList[i].iItem_Type=="0"?"2":ItemList[i].iItem_Type;
		inParams["item_num"] = ItemList[i].iItem_Num=="0"?"1":ItemList[i].iItem_Num;
		inParams["upgrade"] = ItemList[i].iItem_Attribute_1;
		inParams["seal_flag"] = "0";
		
		//only for test and debug
		//now going 2012-05-16
		//inParams["area"] = "15";
		//inParams["receive_charac_no"] = "20779433";
		RuleLOG->WriteLog("[INFO]:::doRobItemSend[%d]��cmd=1003&area=%s&send_charac_name=%s&receive_charac_no=%s&item_id=%s&endurance=%s&item_type=%s&item_num=%s&upgrade=%s&seal_flag=%s&\n",\
							i,
							inParams["area"].c_str(),\
							inParams["send_charac_name"].c_str(),\
							inParams["receive_charac_no"].c_str(),\
							inParams["item_id"].c_str(),\
							inParams["endurance"].c_str(),\
							inParams["item_type"].c_str(),\
							inParams["item_num"].c_str(),\
							inParams["upgrade"].c_str(),\
							inParams["seal_flag"].c_str());
#endif

		//0 ��ֹ�ظ����ͣ� �Ȳ�ѯ�ѽ��в��Ͷ�����Ʒ�б�
		RobICSOper ics_oper;
		vector < map<string, string> > theItemSendResult;
		
		iRet = ics_oper.GetReSendList(ItemList[i], theItemSendResult);//��QQ��roleid��guid��dteventtime
		if ( iRet == 0 )//δ����
		{
			iRet = ics_oper.InsertSendInfo(ItemList[i], theItemSendResult);
			if ( iRet != 0 )
			{
				continue;
			}
		}
		else if ( iRet == 1 )//�Ѳ���
		{
			vecNeedWarnItems.push_back(ItemList[i]);
			ics_oper.UpdateWarnMailCount(ItemList[i], theItemSendResult);
			ItemList[i].iSendResult = IntToStr(STEP_ITEMSEND_RUN_STAT_SEND_AGAIN);
			continue;
		}
		else
		{
			continue;
		}
		//end added

		ics_log_header header(ItemList[i].ICSID, ItemList[i].iRobTo_Uin, ItemList[i].iServiceID);
		iRet = RuleGet.GetFunction(header,inParams,"ics_item_send",theSendResult);  
		RuleLOG->WriteLog("[INFO]:::doRobItemSend[%d]:iRet=%d\n",i,iRet);
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , ItemList[i].ICSID.c_str(),  ItemList[i].iRobTo_Uin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::doRobItemSend[%d]:iRet=%d\n",i,iRet);

		if(0!=LogCM(ItemList[i],"������Ʒ",RequestStr(inParams),"���Ŷ�ʧ�һ�:"+ItemList[i].iItem_Unique_Id,"ics_item_send",
			ItemList[i].iRobed_Uin,ItemList[i].vRobed_RoleId,ItemList[i].vRobed_RoleName,"1003",iRet,inParams["serial"]))
		{
			vecNeedWarnCMLogs.push_back(ItemList[i]);

		}
		
		if(iRet==0&&theSendResult[0]["result"]=="0"){
			RuleLOG->WriteLog("[INFO]:::doRobItemSend[%d]:SUCCESS\n",i);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,ItemList[i].ICSID.c_str(), ItemList[i].iRobTo_Uin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO]:::doRobItemSend[%d]:SUCCESS\n",i);
			ItemList[i].iSendResult = "1";
		}else if(iRet==0&&theSendResult[0]["result"]!="0"){
			RuleLOG->WriteLog("[ERROR]:::doRobItemSend[%d]:ERROR:IDIP_RETURN_CODE[%s]\n",i,theSendResult[0]["result"].c_str());
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,ItemList[i].ICSID.c_str(),  ItemList[i].iRobTo_Uin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__, "[ERROR]:::doRobItemSend[%d]:ERROR:IDIP_RETURN_CODE[%s]\n",i,theItemSendResult[0]["result"].c_str());
			ItemList[i].iSendResult = "2";
		}else{
			RuleLOG->WriteLog("[ERROR]:::doRobItemSend[%d]:SAP_Res_CODE[%d]\n",i,iRet);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , ItemList[i].ICSID.c_str(),  ItemList[i].iRobTo_Uin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__, "[ERROR]:::doRobItemSend[%d]:SAP_Res_CODE[%d]\n",i,iRet);
			ItemList[i].iSendResult = "2";
		}

		ics_oper.UpdateSendResult(ItemList[i], theItemSendResult);
	}

	//���и澯��Ʒ�����ʼ��澯
	if (0 != vecNeedWarnItems.size())
	{
		string title = "�����³�����ʿ����Ʒ�ظ�����Ԥ��";
		string ics = vecNeedWarnItems[0].ICSID;
		string uin = vecNeedWarnItems[0].iRobed_Uin;
		string mailInfo("");
		GetCurDateTime(mailInfo);
		mailInfo += ": </br>----------------------</br>DNF��������ϵͳ��ص������ظ�������Ʒ�������ѳɹ���ֹ�����鼰ʱ�˲飡лл���ο���ϸ��</br>����[";
		mailInfo += ics;
		mailInfo += "]</br>QQ[";
		mailInfo += uin;
		mailInfo += "]";
		/*for (int i = 0; i < vecNeedWarnItems.size(); i++)
		{			
		mailInfo += "</br>item_id:"+vecNeedWarnItems[i].iItem_Id+",unique_id:"+vecNeedWarnItems[i].iItem_Unique_Id + "|";
		}
		mailInfo += "]";*/

		//���͸澯�ʼ�
		SendWarnMail(title, mailInfo);
	}

	//����������־��¼ʧ�ܣ����ʼ��澯
	if(0 != vecNeedWarnCMLogs.size())
	{
		string title = "�����³�����ʿ����������ϵͳ��¼������־ʧ�ܸ澯";
		string ics = vecNeedWarnCMLogs[0].ICSID;
		string uin = vecNeedWarnCMLogs[0].iRobed_Uin;
		string mailInfo("");
		GetCurDateTime(mailInfo);
		mailInfo += ": </br>----------------------</br>DNF��������ϵͳ��ص�������Ʒʱ��������־ʧ��</br>���ݺ�[";
		mailInfo += ics;
		mailInfo += "]</br>QQ[";
		mailInfo += uin;
		mailInfo += "]";
		//���͸澯�ʼ�
		SendWarnMail(title, mailInfo);	
	}

	return 0;
}


int RobRuleLogic::doReplayIcsResult(Rob_Ics_Complaint& autoReplayICS, string& sSource){
	string reply = "";
	RobICSOper ics_oper;
	string itemCount = ics_oper.GetICSItemStats(autoReplayICS.sICSId,autoReplayICS.sServiceID);//added by v_zhtang on 2-13-4-22;;//���ͳɹ���

	RuleLOG->WriteLog("[STEP:PHSEA]:::RobRuleLogic::doReplayIcsResult:mReplyContentList.size()=[%d]\n",mReplyContentList.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , autoReplayICS.sICSId.c_str(),autoReplayICS.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:PHSEA]:::RobRuleLogic::doReplayIcsResult:mReplyContentList.size()=[%d]\n",mReplyContentList.size());

	if(mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()]==""){
		autoReplayICS.siReplyResult=IntToStr(STEP_Reply_WAITING_RESULT_ERROR);
		return STEP_Reply_WAITING_RESULT_ERROR;
	}
	autoReplayICS.sContent=mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()];
	RuleLOG->WriteLog("[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:mReplyContentList:reply=%s&replay_content=%s\n",autoReplayICS.sICSAnaylseStat.c_str(),mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,  autoReplayICS.sICSId.c_str(),autoReplayICS.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:mReplyContentList:reply=%s&replay_content=%s\n",autoReplayICS.sICSAnaylseStat.c_str(),mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()].c_str());
	string paras="0";
#ifdef dnf2
	
	paras="from=dnf&id="+autoReplayICS.sICSId+"&uin="+autoReplayICS.sUin+"&title=DNF���³�����ʿ��������������������&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()];
	//only debug
	//seqid=A1201203202215534567
	//uin=44915366
	//paras="from=dnf&id="+autoReplayICS.sICSId+"&uin="+autoReplayICS.sUin+"&title=DNF���³�����ʿ��������������������&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()]+"";
	//paras="from=dnf&id=A1201203202215534567&uin=44915366&title=�����³�����ʿ����������������������&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()]+"";
#endif
	string rexsultStr;
	paras = string(GbkToUtf8((char*)paras.c_str()));
	//��˾R���
	if(sSource=="10")
	{
		paras+="<br><script type%3d'text/javascript' src%3d'http://kf.qq.com/js/questionnaire.js?n_type%3d1' ></script>";
		RuleLOG->WriteLog("[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url=%s&paras=%s]\n",_rob_httpRpc_url.c_str(),paras.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , autoReplayICS.sICSId.c_str(),autoReplayICS.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url=%s&paras=%s]\n",_rob_httpRpc_url.c_str(),paras.c_str());

		iRet = _rob_httpRpc.Post(_rob_httpRpc_url,paras,rexsultStr);
		if(iRet!=0)
		{
			autoReplayICS.siReplyResult=IntToStr(STEP_Reply_WAITING_RESULT_ERROR);
			return STEP_Reply_WAITING_RESULT_ERROR;
		}
	//����R��Ʒ�
	}
	else if(sSource=="30"/* || sSource=="40" */)//modify by 20121224 v_yfzhu
	{
		//12072713595512598795
		//���ݵ�����rob-
		string temp_icsid=autoReplayICS.sICSId;
		if(autoReplayICS.sICSId.length()==20)
		{
			temp_icsid="rob-"+temp_icsid;
		}
		//add by v_yfzhu  20121024 --begin
		int ret=0;
		RobICSOper  icsopr;
		vector< Item_Trace_List > vSucSendItem;
		string stItemid="";
		string stDetail="";
		ret=icsopr.getICSItemSendSuccessList(autoReplayICS.sICSId,"148",autoReplayICS.sUin,vSucSendItem);
		if (ret !=0)
		{
			return STEP_Reply_WAITING_RESULT_ERROR;
		}
		for(int k=0;k<vSucSendItem.size();k++)
		{
			stDetail+=vSucSendItem[k].iItem_Name+":"+vSucSendItem[k].iItem_Level+",";
		}
		
		char szDetail[1021]={0};
		if (stDetail.length()>=1024)
		{
			strncpy(szDetail,stDetail.c_str(),1020);
			stDetail=szDetail;
		}
		
		//add by v_yfzhu  20121024  --  end
		//paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content="+autoReplayICS.sICSAnaylseStat+"&detail="
		
		//modified by v_wxywu 20130723
		if(autoReplayICS.sICSAnaylseStat == IntToStr(STEP_ITEMSEND_RUN_STAT_SUCCESS) && GetFall_Out(autoReplayICS.sUin)>30000)
		{
			paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content=151&detail="+stDetail + "&count=" + itemCount;
			
		}else
		{
			paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content="+autoReplayICS.sICSAnaylseStat+"&detail="+stDetail + "&count=" + itemCount;
		}

		RuleLOG->WriteLog("[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url_R=%s&paras=%s]\n",_rob_httpRpc_url_R.c_str(),paras.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , autoReplayICS.sICSId.c_str(),autoReplayICS.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url_R=%s&paras=%s]\n",_rob_httpRpc_url_R.c_str(),paras.c_str());

		iRet = _rob_httpRpc_R.Post(_rob_httpRpc_url_R,paras,rexsultStr);
		if(iRet!=0){
			autoReplayICS.siReplyResult=IntToStr(STEP_Reply_WAITING_RESULT_ERROR);
			return STEP_Reply_WAITING_RESULT_ERROR;
		}
	}else{
		string temp_icsid=autoReplayICS.sICSId;
		if(autoReplayICS.sICSId.length()==20)
		{
			temp_icsid="rob-"+temp_icsid;
		}
		//add by v_yfzhu  20121024 beging
		int ret=0;
		RobICSOper  icsopr;
		vector< Item_Trace_List > vSucSendItem;
		string stItemid="";
		string stDetail="";
		ret=icsopr.getICSItemSendSuccessList(autoReplayICS.sICSId,"148",autoReplayICS.sUin,vSucSendItem);
		if (ret !=0)
		{
			return STEP_Reply_WAITING_RESULT_ERROR;
		}
		for(int k=0;k<vSucSendItem.size();k++)
		{
			stDetail+=vSucSendItem[k].iItem_Name+":"+vSucSendItem[k].iItem_Level+",";
		}

		char szDetail[1021]={0};
		if (stDetail.length()>=1024)
		{
			strncpy(szDetail,stDetail.c_str(),1020);
			stDetail=szDetail;
		}

		//add by v_yfzhu  20121024    end
		//paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content="+autoReplayICS.sICSAnaylseStat+"&detail="

		//modified by v_wxywu 20130723
		if(autoReplayICS.sICSAnaylseStat == IntToStr(STEP_ITEMSEND_RUN_STAT_SUCCESS) && GetFall_Out(autoReplayICS.sUin)>30000)
		{
			paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content=151&detail="+stDetail + "&count=" + itemCount;
			
		}else
		{
			paras="from=dnf&id="+temp_icsid+"&uin="+autoReplayICS.sUin+"&content="+autoReplayICS.sICSAnaylseStat+"&detail="+stDetail + "&count=" + itemCount;
		}
		RuleLOG->WriteLog("[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url_T=%s&paras=%s]\n",_rob_httpRpc_url_T.c_str(),paras.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , autoReplayICS.sICSId.c_str(),autoReplayICS.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:INFO]:::RobRuleLogic::doReplayIcsResult:[_rob_httpRpc_url_T=%s&paras=%s]\n",_rob_httpRpc_url_T.c_str(),paras.c_str());
		iRet = _rob_httpRpc_T.Post(_rob_httpRpc_url_T,paras,rexsultStr);
		if(iRet!=0){
			autoReplayICS.siReplyResult=IntToStr(STEP_Reply_WAITING_RESULT_ERROR);
			return STEP_Reply_WAITING_RESULT_ERROR;
		}
	}

	autoReplayICS.siReplyResult=IntToStr(STEP_Reply_WAITING_RESULT_SUCCESS);
	return STEP_Reply_WAITING_RESULT_SUCCESS;
}

/*******************************************************************************
Description	:	�ظ��ֻ�QQ��ȫ����app���³�����ʿר��
Params		:	ics_info	in	������Ϣ
				type		in	������� ��-0 ��-1������
result		:	�ظ���� 0-�ɹ�
exception	:	none
*/
int RobRuleLogic::doReplayMobileApp(Rob_Ics_Complaint &ics_info, int type)
{
	sleep(1);
	int sockfd;
	struct sockaddr_in servaddr;
	
	char sHost[20] = {0};
	
	int iPort = 8888;
	if ( 0 == type )
	{
		strcpy(sHost, CRobConf::getConf()->GetCommonInterface()["reply_mobile_app_host_main"].c_str());
		iPort = StrToInt(CRobConf::getConf()->GetCommonInterface()["reply_mobile_app_port_main"]);
	}
	else
	{
		strcpy(sHost, CRobConf::getConf()->GetCommonInterface()["reply_mobile_app_host_prepare"].c_str());
		iPort = StrToInt(CRobConf::getConf()->GetCommonInterface()["reply_mobile_app_port_prepare"]);
	}

	// init servaddr
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(iPort);
	if ( inet_pton(AF_INET, sHost, &servaddr.sin_addr) <= 0 )
	{
		RuleLOG->WriteLog("��Ч��IP��ַ:sHost[%s]\n", sHost);
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "��Ч��IP��ַ:sHost[%s]\n", sHost );
		return 1;
	}
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	char send_buf[MAXLINE] = {0};
	char recv_buf[MAXLINE + 1] = {0};
	int send_len = sizeof(send_buf);
	int recv_len = sizeof(recv_buf);
	
	// ���ӷ�����
	if ( -1 == connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) )
	{
		RuleLOG->WriteLog("���ӷ�����ʧ��\n");
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "���ӷ�����ʧ��\n" );
		return 2;
	}

	AppealNotifyReqPkg_t reqPkg;
	memset(&reqPkg, 0, sizeof(AppealNotifyReqPkg_t));

	// head
	reqPkg.stHead.usMsgType = 1006;	// ��Ϣ����:���ߵ����֪ͨ
	reqPkg.stHead.ullUin = StrToLLong(ics_info.sUin);	// �����û�QQ
	// body
	reqPkg.stBody.usAppealNoLen = 30;	// ���뵥�ų���
	strncpy((char*)reqPkg.stBody.aszAppealNo, ics_info.sICSId.c_str(), ics_info.sICSId.size());	// ���뵥��

	//������Ϣ
	reqPkg.stBody.stCommReq.ulReserve = 148;//ҵ��ID
	reqPkg.stBody.stBaseInfo.ulWorldId = StrToInt(ics_info.sWordId);	// ������Id
	reqPkg.stBody.stBaseInfo.usWorldNameLen = 128;
	strncpy((char*)reqPkg.stBody.stBaseInfo.aszWorldName, mAreaConf[ics_info.sWordId].c_str(), mAreaConf[ics_info.sWordId].size());	// ������
	
	reqPkg.stBody.stBaseInfo.ullUin = StrToLLong(ics_info.sUin);		// �û�uin
	reqPkg.stBody.stBaseInfo.ulRoleId = StrToInt(ics_info.sRoleID);	// ��ɫID
	reqPkg.stBody.stBaseInfo.usRoleNameLen = 128;
	string name_utf8 = GbkToUtf8((char*)ics_info.sRoleName.c_str());
	strncpy((char*)reqPkg.stBody.stBaseInfo.aszRoleName, name_utf8.c_str(), name_utf8.size());	// ��ɫ��
	reqPkg.stBody.stBaseInfo.vipLevel = StrToInt(ics_info.sIsVip);	// �����û��ȼ�
	
	time_t tAppealTime = 0;
	GetTmsecFromUnxfmt( tAppealTime, ics_info.stBookTime.c_str(), SDBL_CONTENT_ALL );
	reqPkg.stBody.stBaseInfo.ulAppealTime = tAppealTime;	// ����ʱ��
	
	//����
	if ( 0 == reqPkg.stBody.stBaseInfo.vipLevel )
	{
		reqPkg.stBody.stDetailInfo.ulAppealRetCode = StrToInt(ics_info.sICSAnaylseStat);	// ҵ������
	}
	else
	{
		reqPkg.stBody.stDetailInfo.ulAppealRetCode = StrToInt(ics_info.sICSAnaylseStat) * 10 + 1;	// ҵ������
	}
	reqPkg.stBody.stDetailInfo.usAppealResMsgLen = 256;	// ���뵥��������Ϣ����
	
	iRet = PduEncode<AppealNotifyReqPkg_t>(send_buf, send_len, reqPkg);
	RuleLOG->WriteLog("PduEncode���:iRet[%d]\n", iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "PduEncode���:iRet[%d]\n", iRet );
	
	// ������Ϣ
	iRet = write(sockfd, send_buf, send_len);
	RuleLOG->WriteLog("���ͱ��Ľ��: sockfd[%d],send_len[%d],iRet[%d]\n", sockfd, send_len, iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "���ͱ��Ľ��: sockfd[%d],send_len[%d],iRet[%d]\n", sockfd, send_len, iRet );
	
	// ������Ϣ
	int n = read(sockfd, recv_buf, recv_len);	// ʵ�ʶ������ֽ���
	RuleLOG->WriteLog("������Ϣ���: sockfd[%d],n[%d],recv_buf[%s]\n", sockfd, n, recv_buf);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "������Ϣ���: sockfd[%d],n[%d],recv_buf[%s]\n", sockfd, n, recv_buf );
	if ( -1 == n )
	{
		return 3;
	}
	
	AppealNotifyRspPkg_t rspPkg;
	iRet = PduDecode<AppealNotifyRspPkg_t>(rspPkg, recv_buf, n);
	RuleLOG->WriteLog("�������Ľ��: sockfd[%d],recv_buf[%s],iRet[%d]\n", sockfd, recv_buf, iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "�������Ľ��: sockfd[%d],recv_buf[%s],iRet[%d]\n", sockfd, recv_buf, iRet );
	if ( 0 == iRet )
	{
		RuleLOG->WriteLog("�������ĳɹ�: errCode[%d],reason[%d],msgLen[%d],msg[%s]\n", rspPkg.stBody.stCommRsp.iRetCode, rspPkg.stBody.stCommRsp.iReason, rspPkg.stBody.stCommRsp.usMsgLen, rspPkg.stBody.stCommRsp.aszMsg );
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "�������ĳɹ�: errCode[%d],reason[%d],msgLen[%d],msg[%s]\n", rspPkg.stBody.stCommRsp.iRetCode, rspPkg.stBody.stCommRsp.iReason, rspPkg.stBody.stCommRsp.usMsgLen, rspPkg.stBody.stCommRsp.aszMsg );
	}
	return 0==iRet?rspPkg.stBody.stCommRsp.iRetCode:iRet;
}


/*********************************************************************
Description:	��¼������־
@				
@desc			
@exception		none
*/
int RobRuleLogic::LogCM(const Item_Trace_List &ItemList,
	const string& strOptType, 
	const string& strRequest, 
	const string& strMemo,
	const string& strFuntionName,
	const string& iUin,
	const string& iRoleId,
	const string& vRoleName,
	const string& strCmd,
	const int& iResult,
	const string& StrSerial)
{
	if (strFuntionName.empty())
	{
		return -1;
	}
	int optId=StrToInt(CRobConf::getConf()->GetIcsOper()[strFuntionName]);

	if( false == CLogManager::GetInstance().WriteCMLog(GbkToUtf8("��������ϵͳ"),//������
		StrToInt(ItemList.iServiceID),//ҵ��ID
		optId,//�����ӿ�ID
		StrToLLong(iUin),//����Ŀ��QQ
		StrToInt(ItemList.iWordId),
		iResult,//����������ɹ���ʧ��
		GbkToUtf8((char*)strOptType.c_str()),//��������
		"",//ip
		ItemList.ICSID,//seqId ����
		iRoleId,//����Ŀ���ɫID
		GbkToUtf8((char*)vRoleName.c_str()),//����Ŀ���ɫ��
		StrToInt(ItemList.iItem_Id),//�����ĵ���
		StrToInt(ItemList.iItem_Num),//��������
		GbkToUtf8((char*)strRequest.c_str()),//itemDes ����
		"5",//����ԭ��:Ͷ�ߴ���
		GbkToUtf8((char*)strMemo.c_str()),//��ע
		strCmd,//cmd
		6,//������Դ��6������ϵͳ
		StrSerial))
	{
		RuleLOG->WriteLog("sorry����¼������־ʧ�ܣ�����Ŀ���û���%s��| ������ɫid��%s�� | ����Ŀ���ɫ����%s��| ����ID��%s�� | ����������%s�� | �������͡�%s��| ����ԭ��%s�� | �����ӿ�id��%d��|���������%d��|serial:%s\n",
			iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult,StrSerial.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ItemList.ICSID.c_str(),iUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR, __FUNCTION__, \
			"sorry����¼������־ʧ�ܣ�����Ŀ���û���%s��,������ɫid��%s��, ����Ŀ���ɫ����%s��, ����ID��%s�� | ����������%s��, �������͡�%s��,����ԭ��%s��,�����ӿ�id��%d��,���������%d��\n",
			iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult);

		return -1;
	}
	RuleLOG->WriteLog("��¼������־�ɹ�������Ŀ���û���%s��, ������ɫid��%s��, ����Ŀ���ɫ����%s��, ����ID��%s��, ����������%s��,�������͡�%s��, ����ԭ��%s��, �����ӿ�id��%d��,���������%d��|serial:%s\n\n",
		iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult,StrSerial.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ItemList.ICSID.c_str(),iUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, \
		"��¼������־�ɹ�������Ŀ���û���%s��, ������ɫid��%s��, ����Ŀ���ɫ����%s��, ����ID��%s��, ����������%s��, �������͡�%s��, ����ԭ��%s��, �����ӿ�id��%d��,���������%d��\n",
		iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult);

	return 0;
}

/*********************************************************************
	Description:	���ʼ�
	@				
	@desc			
	@exception		none
	*/
int RobRuleLogic::SendWarnMail(const string& mailTitle, const string& mailContext)
{
	// ���ʼ��澯
	string mailTo("");// = "windhe@tencent.com";

	//�����ʼ��û��б����û� @modify by v_sjwang 2012/11/2 10:34:06
	int userNum = 0;
	map<string, string> userlist = CRobConf::getConf()->GetUserList();
	string name("");
	string flg("");
	for ( map<string, string>::iterator it = userlist.begin(); it != userlist.end(); it++ )
	{
		mailTo = "";
		name = it->first;
		flg = it->second;
		Trim(name);
		Trim(flg);
		if ( name.size() > 0 && 1 == StrToInt(flg) )
		{
			mailTo = mailTo + name + "@tencent.com ";
			//modified by v_zhtang on 2012-1-5;
			Trim(mailTo);
			bool result = CLogManager::SendEmail(mailTo, mailTitle, mailContext);
			if(result)
			{
				userNum++;		
				WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "���͸���Ʒ��¼�Ѿ����ڣ��ɹ����͸澯�ʼ���[%s]\n", mailTo.c_str() );
			}
			else
			{
				WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_ERROR, __FUNCTION__, "���͸���Ʒ��¼�Ѿ����ڣ����͸澯�ʼ���[%s]ʧ��\n", mailTo.c_str() );
			}
			//end modified.
		}
	}
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_WARNING, __FUNCTION__, "���͸���Ʒ��¼�Ѿ����ڣ��ɹ����ʼ��澯����Ϊ��%d�����ʼ����ݡ�%s��\n", userNum ,mailContext.c_str());

	return iRet;
}

/*********************************************************************
	Description:	��ѯ��ȯ fall_outֵ
	@				
	@desc			
	@exception		none
	*/
int RobRuleLogic::GetFall_Out(const string &uin)
{

	map <string,string> inParams;
	vector < map<string, string> > theResult;
	inParams["uin"] = uin;
	ics_log_header header;
	int iRet = RuleGet.GetFunction(header,inParams,"ics_get_fall_out",theResult);
	if(iRet!=0)
	{    	  	    				    		    		    		    	       			    			  				
			return 0;
	}
	else
	{
		if(theResult.size()>0)
		{
			return	StrToInt(theResult[0]["fall_out"]);
		}
	}

	return 0;
}