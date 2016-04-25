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
	//初始化装备过滤库
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
	Description:	回复接口的reply的特性list
	@desc			存在map<string,string> vItemProList;
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
	Description:	获取业务的找寻物品的list
	@desc			存在map<string,string> vItemProList;
	@exception		none
*/
void RobRuleLogic::LoadItemProcList(){
	/*获取业务的找寻物品的list*/
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
	/*这里直接LoadTab来实现*/
	string filepath = "./cfg/";
#ifdef dnf2
	filepath+="robItem-NameList_dnf2.conf";
	LoadTabToIDvsName(filepath,vItemProList);
	/*if(0!=LoadTabToMap(filepath,vItemProList))
	{
		RuleLOG->WriteLog("读取道具名称列表文件失败【robItem-NameList_dnf2.conf】\n");

		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, \
			"读取道具名称列表文件失败【robItem-NameList_dnf2.conf】\n");
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
	Description:	获取业务的找寻物品的list的size（）主要验证类的初始化的正确性。
	@desc			存在map<string,string> vItemProList;
	@exception		none
*/
int RobRuleLogic::getItemListSize(){
	return vItemProList.size();
}
/*********************************************************************
	Description:	获取业务的找寻物品的list是否存在制定item_id。
	@desc			存在map<string,string> vItemProList;
	@exception		none
*/
bool RobRuleLogic::checkItemNeedProc(string iItem_id){
	return vItemProList.find((char*)iItem_id.c_str())==vItemProList.end()?false:true;
}
/*********************************************************************
	Description:	获取业务的找寻物品的list的size（）主要验证类的无凭初始化的正确性。
	@desc			存在map<string,string> vItemProList;
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
	int cnt = 0;//统计高等级物品

	RuleLOG->WriteLog("获取玩家的异常登录条数【%d】\n",iSlotCouFlag);

	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, \
		"获取玩家的异常登录条数【%d】\n",iSlotCouFlag);

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
		RuleLOG->WriteLog("--->单号【%s】数据：uin=%s&area=%s&gid=%s&start_time=%s&end_time=%s\n",rob_data[j].ICSID.c_str(),\
							inParams["uin"].c_str(),\
							inParams["area"].c_str(),\
							inParams["gid"].c_str(),\
							inParams["start_time"].c_str(),\
							inParams["end_time"].c_str());
	
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,\
			"--->单号【%s】数据：uin=%s&area=%s&gid=%s&start_time=%s&end_time=%s\n", rob_data[j].ICSID.c_str(), inParams["uin"].c_str(), inParams["area"].c_str(), inParams["gid"].c_str(),inParams["start_time"].c_str(),inParams["end_time"].c_str());

			ics_log_header header(rob_data[j].ICSID, rob_data[j].UserID, TLOG_SERVICE_ID);
		iRet = RuleGet.GetFunction(header,inParams,"ics_role_item_flow_query",theItemListResult);
		if (0 != iRet)
		{
			RuleLOG->WriteLog("Get getRoleItemFlowList Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theItemListResult.size());
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR ,__FUNCTION__,  "Get getRoleItemFlowList Glog Interface Error Res(%d) and DataResult'Size(%d)\n",iRet,theItemListResult.size());
			continue;
		}
		if(theItemListResult.size()==0&&iSlotCouFlag!=1){
			RuleLOG->WriteLog("循环的大小【%d】判断FLAG数据大小【%d】\n",theItemListResult.size(),iSlotCouFlag);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "循环的大小【%d】判断FLAG数据大小【%d】\n",theItemListResult.size(),iSlotCouFlag);
			iSlotCouFlag--;
			RuleLOG->WriteLog("循环的大小【%d】判断FLAG数据大小变为【%d】\n",theItemListResult.size(),iSlotCouFlag);
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC ,rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "循环的大小【%d】判断FLAG数据大小变为【%d】\n",theItemListResult.size(),iSlotCouFlag);
			continue;
		}else if(theItemListResult.size()==0&&iSlotCouFlag==1){
			RuleLOG->WriteLog("theItemListResult的大小【0】判断FLAG数据大小变为【1】\n");
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"theItemListResult的大小【0】判断FLAG数据大小变为【1】\n");
			return STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW;
		}else{
			RuleLOG->WriteLog("获得物品流水数据接口返回【%d】数据大小【%d】\n",iRet,theItemListResult.size());
			WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , rob_data[j].ICSID.c_str(), rob_data[j].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__,"获得物品流水数据接口返回【%d】数据大小【%d】\n",iRet,theItemListResult.size());
			for(int k=0;k<theItemListResult.size();k++){
				struTempItemFlow.iServiceID = rob_data[j].ServiceID;
				struTempItemFlow.iWordId = rob_data[j].WorldID;
				struTempItemFlow.ICSID = rob_data[j].ICSID;
				struTempItemFlow.vRobed_RoleId = rob_data[j].RoleID;
				struTempItemFlow.iRobed_Uin = rob_data[j].UserID;
				struTempItemFlow.vRobed_RoleName =  rob_data[j].RoleName;
				
				
				//这里为以后版本流接口
				struTempItemFlow.iItem_Type = "0";
				struTempItemFlow.iRobTo_Uin = "0";
				struTempItemFlow.vRobTo_RoleName = "0";
				struTempItemFlow.vRobTo_RoleId = "0";
				struTempItemFlow.vFlowDsc="0";
				
				//这里为以后版本流接口
				struTempItemFlow.iDefineProc = "1"; //需要处理
#ifdef dnf2
				//a.只处理基础等级物品，高等级物品不处理---针对全体用户适用
				//b.只处理强化等级<=15等级的物品---针对权限用户适用
				//c.对于加强物品且小于15级的,VIP用户补送，基础用户不补送
				/*d.iDefineProc的定义
					0 未处理定义
					1 需要处理
					2 人工处理
					3 处理完成
					14 高等级物品，不需要处理或者转人工
					15 高加强物品，不需要处理或者转人工
					16 非VIP用户，无法处理VIP补送物品
					40 物品价值更新完毕 
				*/
				struTempItemFlow.dtEventTime = theItemListResult[k]["dtDateTime"];
				struTempItemFlow.iItem_Num =  theItemListResult[k]["iDecNumber"];
				struTempItemFlow.iItem_Id =  theItemListResult[k]["iItemNO"];
				struTempItemFlow.iItem_Name =  vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()]==""?"0":vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()];
				RuleLOG->WriteLog("获得物品流水数据接口装备名称【%s】配置的名称【%s】\n",struTempItemFlow.iItem_Name.c_str(),vItemProList[(char*)theItemListResult[k]["iItemNO"].c_str()].c_str());
				struTempItemFlow.iFlowAction =  theItemListResult[k]["iReason"];
				//基础等级
				struTempItemFlow.iItem_Level = vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()]==""?"0":vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()];
				RuleLOG->WriteLog("获得物品流水数据接口装备价值【%s】配置的价值【%s】\n",struTempItemFlow.iItem_Level.c_str(),vItemValueList[(char*)theItemListResult[k]["iItemNO"].c_str()].c_str());
				struTempItemFlow.iItem_Unique_Id = theItemListResult[k]["vGuid"];
				//强化属性等级
				struTempItemFlow.iItem_Attribute_1 = theItemListResult[k]["iUpgrade"];
				struTempItemFlow.iDefineProc = "1";
				struTempItemFlow.iItem_Price_Value = "0";



				//所有用户的
				//不需要处理，或者转人工, 存在高等级物品
				//if(struTempItemFlow.iFlowAction=="9"&&struTempItemFlow.iItem_Level=="3")
				if(struTempItemFlow.iItem_Level=="3")//modify by v_yfzhu 20121126
				{
					cnt++;
					if (cnt <= 2)
					{
						struTempItemFlow.iDefineProc = "1"; //高等级物品次数不超过2时系统自动处理
					}
					else
					{
						struTempItemFlow.iDefineProc = "2"; //高等级物品次数 超过2时人工处理
					}
				}

				if(StrToInt(struTempItemFlow.iItem_Attribute_1)>= 15){
					struTempItemFlow.iDefineProc = "15";  //不需要处理，该物品有强化，且>=15
					struTempItemFlow.iDefineProc = "2";
				}
				//非VIP
				/*if(vip=="0"&&StrToInt(struTempItemFlow.iItem_Attribute_1)>0){
					struTempItemFlow.iItem_Attribute_1="0";
					struTempItemFlow.iDefineProc = "1";  //不需要处理，该物品有强化
				}
				*/
				if(struTempItemFlow.iItem_Name=="0"){
					struTempItemFlow.iDefineProc = "17";  //不需要处理,该物品不在处理范围内
				}
				if(struTempItemFlow.iFlowAction=="1"||struTempItemFlow.iFlowAction=="15"||struTempItemFlow.iFlowAction=="22")
				{
					struTempItemFlow.iDefineProc = "17";  //不需要处理,该物品不在处理范围内
				}
				
#endif
				
				rob_Item_List.push_back(struTempItemFlow);
			}
		}
	}
	if(rob_Item_List.size()==0){
		return STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW;
	}
  //物品补送上线50
  int item_sums_toplimit=StrToInt(CRobConf::getConf()->GetIcsValid()["item_sums_toplimit"]);
  int item_count_send=0;
  for(int k=0;k<rob_Item_List.size();k++){
     if(item_count_send>item_sums_toplimit){
        rob_Item_List[k].iDefineProc="17"; //不需要处理,该物品不在处理范围内
        continue;
     }
     if(rob_Item_List[k].iDefineProc=="1"){
          item_count_send++;
     }
  }
	return 0;
}
/*********************************************************************
	Description:	获取getRobSlotMoneySub()的金钱丢失量。
	@desc			包括盗号时间段情况下的读取金钱丢失量。
	@exception		none
*/
int RobRuleLogic::getRobSlotMoneySub(Rob_SLOT &rob_data){

	/*查询物品金钱减少日志*/
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
Description:	获取getRobItemTrace()物品追踪日志
@				分为： NoTracking和 ByTracking的两种情况
@desc			包括盗号时间段情况下的读物品追踪。
@exception		none
*/
int RobRuleLogic::getRobItemTraceNoTracking(std::vector< Item_Trace_List > &TraceResult,std::vector< Item_Flow_List > &FlowParams){
	TraceResult.clear();
	RuleLOG->WriteLog("getRobItemTraceNoTracking获取玩家的物品补送流水FlowParams.size():%d",FlowParams.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "getRobItemTraceNoTracking获取玩家的物品补送流水FlowParams.size():%d",FlowParams.size());
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
	
		
		
		//1 需要自动补送
		//2 不处理
		//3 不需要自动补送，手工补送的
		struTempItemTrace.iSendAction = "1";

		//0 初始态
		//1 第一手
		//-1 最后一手（当前在谁的身上）
		struTempItemTrace.iTraceHand = "-1";
		/*
		0 待处理
		1 无须追踪，直接补送
		2 需要补送，手工补送
		3 追踪有结果，在人身上
		追踪有结果在NPC，商店，仓库
		4 追踪物品已消失，
		摧毁，炼化，扔地上……
		5 无法处理
		*/
		struTempItemTrace.iTraceResult = "1";

#ifdef dnf2
		//struTempItemTrace.iSendAction = FlowParams[i].iItem_Level=="0"?"1":"2";
		if(FlowParams[i].iDefineProc=="1"){
			struTempItemTrace.iSendAction="1";//自动处理
		}else if(FlowParams[i].iDefineProc=="2"){
			struTempItemTrace.iSendAction="3";//转人工
		}else{
			struTempItemTrace.iSendAction="2";//不处理
			continue;
		}
		struTempItemTrace.iTraceHand = "-1";
		//暂时不处理
		struTempItemTrace.iTraceResult = FlowParams[i].iDefineProc=="1"?"1":"2";
#endif
		TraceResult.push_back(struTempItemTrace);
	}
}
/*********************************************************************
	Description:	获取doRobItemSend()物品补送接口
	@				分为：各个业务需要单独来定义自己的流程，详细的见代码
	@desc			包括盗号时间段情况下的物品补送接口。
	@exception		none
*/

int RobRuleLogic::doRobItemSend(std::vector< Item_Trace_List > &ItemList){
	map<string,string> inParams;
	vector < map<string, string> > theItemSendResult, theSendResult;
	std::vector< Item_Trace_List > vecNeedWarnItems;//需要邮件告警的物品列表，一起发出去
	std::vector< Item_Trace_List > vecNeedWarnCMLogs; //记敏感日志失败需发警告邮件

	RuleLOG->WriteLog("[PHASE]:::doRobItemSend:ItemList_size()=%d\n",ItemList.size());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[PHASE]:::doRobItemSend:ItemList_size()=%d\n",ItemList.size());
	for(int i=0;i<ItemList.size();i++)
	{
		inParams.clear();
#ifdef dnf2
/*
Cdkey兑换道具接口

cmd=1003&area=1&send_charac_name=system&receive_charac_no=16584622&item_id=2360&endurance=7&item_type=2&item_num=3&upgrade=2&seal_flag=1

result=0&area=1&receive_charac_no=16584622

说明：result＝0表示操作成功，< 0表示操作失败 
发送道具的类型，即item_type的定义如下： typedef enum ITEM_TYPE { T_COIN = 0, //金钱 
											  T_HONOUR = 1, //称号 
											  T_EQUIP = 2, //装备 
											  T_EXPENDABLE = 3, //消耗品 
											  T_AVATAR = 4, //装扮 
											  //T_PET = 5 
											  送宠物请使用专门的接口1020 
											  };
注意： 千万不要搞错道具类型，否则赠送数量会很吓人的，会造成比较大的损失 
seal_flag			是否可交易。默认为0不可交易 
send_charac_name	结果url encode编码 
endurance			普通物品 (耐久度)/复活币(0: 免费, 1: 活动, 2: 收费)/装扮(装扮的选择属性) 
upgrade				物品强化值，慎用
*/		RuleLOG->WriteLog("[PHASE]:::doRobItemSend:icsid=%s\n",ItemList[i].ICSID.c_str());
		string send_charac_name = "盗号系统找回物品";
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
		RuleLOG->WriteLog("[INFO]:::doRobItemSend[%d]：cmd=1003&area=%s&send_charac_name=%s&receive_charac_no=%s&item_id=%s&endurance=%s&item_type=%s&item_num=%s&upgrade=%s&seal_flag=%s&\n",\
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

		//0 防止重复补送， 先查询已进行补送动作物品列表
		RobICSOper ics_oper;
		vector < map<string, string> > theItemSendResult;
		
		iRet = ics_oper.GetReSendList(ItemList[i], theItemSendResult);//按QQ、roleid、guid、dteventtime
		if ( iRet == 0 )//未补过
		{
			iRet = ics_oper.InsertSendInfo(ItemList[i], theItemSendResult);
			if ( iRet != 0 )
			{
				continue;
			}
		}
		else if ( iRet == 1 )//已补过
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

		if(0!=LogCM(ItemList[i],"补送物品",RequestStr(inParams),"盗号丢失找回:"+ItemList[i].iItem_Unique_Id,"ics_item_send",
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

	//若有告警物品，则邮件告警
	if (0 != vecNeedWarnItems.size())
	{
		string title = "【地下城与勇士】物品重复补送预警";
		string ics = vecNeedWarnItems[0].ICSID;
		string uin = vecNeedWarnItems[0].iRobed_Uin;
		string mailInfo("");
		GetCurDateTime(mailInfo);
		mailInfo += ": </br>----------------------</br>DNF盗号自助系统监控到即将重复补送物品，不过已成功阻止！建议及时核查！谢谢！参考明细：</br>单号[";
		mailInfo += ics;
		mailInfo += "]</br>QQ[";
		mailInfo += uin;
		mailInfo += "]";
		/*for (int i = 0; i < vecNeedWarnItems.size(); i++)
		{			
		mailInfo += "</br>item_id:"+vecNeedWarnItems[i].iItem_Id+",unique_id:"+vecNeedWarnItems[i].iItem_Unique_Id + "|";
		}
		mailInfo += "]";*/

		//发送告警邮件
		SendWarnMail(title, mailInfo);
	}

	//若有敏感日志记录失败，则邮件告警
	if(0 != vecNeedWarnCMLogs.size())
	{
		string title = "【地下城与勇士】盗号自助系统记录敏感日志失败告警";
		string ics = vecNeedWarnCMLogs[0].ICSID;
		string uin = vecNeedWarnCMLogs[0].iRobed_Uin;
		string mailInfo("");
		GetCurDateTime(mailInfo);
		mailInfo += ": </br>----------------------</br>DNF盗号自助系统监控到补送物品时记敏感日志失败</br>单据号[";
		mailInfo += ics;
		mailInfo += "]</br>QQ[";
		mailInfo += uin;
		mailInfo += "]";
		//发送告警邮件
		SendWarnMail(title, mailInfo);	
	}

	return 0;
}


int RobRuleLogic::doReplayIcsResult(Rob_Ics_Complaint& autoReplayICS, string& sSource){
	string reply = "";
	RobICSOper ics_oper;
	string itemCount = ics_oper.GetICSItemStats(autoReplayICS.sICSId,autoReplayICS.sServiceID);//added by v_zhtang on 2-13-4-22;;//补送成功数

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
	
	paras="from=dnf&id="+autoReplayICS.sICSId+"&uin="+autoReplayICS.sUin+"&title=DNF地下城与勇士关于您盗号受理结果反馈&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()];
	//only debug
	//seqid=A1201203202215534567
	//uin=44915366
	//paras="from=dnf&id="+autoReplayICS.sICSId+"&uin="+autoReplayICS.sUin+"&title=DNF地下城与勇士关于您盗号受理结果反馈&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()]+"";
	//paras="from=dnf&id=A1201203202215534567&uin=44915366&title=【地下城与勇士】关于您盗号受理结果反馈&content="+mReplyContentList[(char*)autoReplayICS.sICSAnaylseStat.c_str()]+"";
#endif
	string rexsultStr;
	paras = string(GbkToUtf8((char*)paras.c_str()));
	//公司R线填单
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
	//互娱R产品填单
	}
	else if(sSource=="30"/* || sSource=="40" */)//modify by 20121224 v_yfzhu
	{
		//12072713595512598795
		//单据的增加rob-
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
Description	:	回复手机QQ安全中心app地下城与勇士专区
Params		:	ics_info	in	单据信息
				type		in	发送类别 主-0 备-1、其他
result		:	回复结果 0-成功
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
		RuleLOG->WriteLog("无效的IP地址:sHost[%s]\n", sHost);
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "无效的IP地址:sHost[%s]\n", sHost );
		return 1;
	}
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	char send_buf[MAXLINE] = {0};
	char recv_buf[MAXLINE + 1] = {0};
	int send_len = sizeof(send_buf);
	int recv_len = sizeof(recv_buf);
	
	// 连接服务器
	if ( -1 == connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) )
	{
		RuleLOG->WriteLog("连接服务器失败\n");
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "连接服务器失败\n" );
		return 2;
	}

	AppealNotifyReqPkg_t reqPkg;
	memset(&reqPkg, 0, sizeof(AppealNotifyReqPkg_t));

	// head
	reqPkg.stHead.usMsgType = 1006;	// 消息类型:申诉单结果通知
	reqPkg.stHead.ullUin = StrToLLong(ics_info.sUin);	// 被盗用户QQ
	// body
	reqPkg.stBody.usAppealNoLen = 30;	// 申请单号长度
	strncpy((char*)reqPkg.stBody.aszAppealNo, ics_info.sICSId.c_str(), ics_info.sICSId.size());	// 申请单号

	//基本信息
	reqPkg.stBody.stCommReq.ulReserve = 148;//业务ID
	reqPkg.stBody.stBaseInfo.ulWorldId = StrToInt(ics_info.sWordId);	// 服务器Id
	reqPkg.stBody.stBaseInfo.usWorldNameLen = 128;
	strncpy((char*)reqPkg.stBody.stBaseInfo.aszWorldName, mAreaConf[ics_info.sWordId].c_str(), mAreaConf[ics_info.sWordId].size());	// 服务器
	
	reqPkg.stBody.stBaseInfo.ullUin = StrToLLong(ics_info.sUin);		// 用户uin
	reqPkg.stBody.stBaseInfo.ulRoleId = StrToInt(ics_info.sRoleID);	// 角色ID
	reqPkg.stBody.stBaseInfo.usRoleNameLen = 128;
	string name_utf8 = GbkToUtf8((char*)ics_info.sRoleName.c_str());
	strncpy((char*)reqPkg.stBody.stBaseInfo.aszRoleName, name_utf8.c_str(), name_utf8.size());	// 角色名
	reqPkg.stBody.stBaseInfo.vipLevel = StrToInt(ics_info.sIsVip);	// 心悦用户等级
	
	time_t tAppealTime = 0;
	GetTmsecFromUnxfmt( tAppealTime, ics_info.stBookTime.c_str(), SDBL_CONTENT_ALL );
	reqPkg.stBody.stBaseInfo.ulAppealTime = tAppealTime;	// 申诉时间
	
	//详情
	if ( 0 == reqPkg.stBody.stBaseInfo.vipLevel )
	{
		reqPkg.stBody.stDetailInfo.ulAppealRetCode = StrToInt(ics_info.sICSAnaylseStat);	// 业务处理结果
	}
	else
	{
		reqPkg.stBody.stDetailInfo.ulAppealRetCode = StrToInt(ics_info.sICSAnaylseStat) * 10 + 1;	// 业务处理结果
	}
	reqPkg.stBody.stDetailInfo.usAppealResMsgLen = 256;	// 申请单处理结果消息长度
	
	iRet = PduEncode<AppealNotifyReqPkg_t>(send_buf, send_len, reqPkg);
	RuleLOG->WriteLog("PduEncode结果:iRet[%d]\n", iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "PduEncode结果:iRet[%d]\n", iRet );
	
	// 发送消息
	iRet = write(sockfd, send_buf, send_len);
	RuleLOG->WriteLog("发送报文结果: sockfd[%d],send_len[%d],iRet[%d]\n", sockfd, send_len, iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "发送报文结果: sockfd[%d],send_len[%d],iRet[%d]\n", sockfd, send_len, iRet );
	
	// 接受消息
	int n = read(sockfd, recv_buf, recv_len);	// 实际读到的字节数
	RuleLOG->WriteLog("接受消息结果: sockfd[%d],n[%d],recv_buf[%s]\n", sockfd, n, recv_buf);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "接受消息结果: sockfd[%d],n[%d],recv_buf[%s]\n", sockfd, n, recv_buf );
	if ( -1 == n )
	{
		return 3;
	}
	
	AppealNotifyRspPkg_t rspPkg;
	iRet = PduDecode<AppealNotifyRspPkg_t>(rspPkg, recv_buf, n);
	RuleLOG->WriteLog("解析报文结果: sockfd[%d],recv_buf[%s],iRet[%d]\n", sockfd, recv_buf, iRet);
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "解析报文结果: sockfd[%d],recv_buf[%s],iRet[%d]\n", sockfd, recv_buf, iRet );
	if ( 0 == iRet )
	{
		RuleLOG->WriteLog("解析报文成功: errCode[%d],reason[%d],msgLen[%d],msg[%s]\n", rspPkg.stBody.stCommRsp.iRetCode, rspPkg.stBody.stCommRsp.iReason, rspPkg.stBody.stCommRsp.usMsgLen, rspPkg.stBody.stCommRsp.aszMsg );
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ics_info.sICSId.c_str(), ics_info.sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "解析报文成功: errCode[%d],reason[%d],msgLen[%d],msg[%s]\n", rspPkg.stBody.stCommRsp.iRetCode, rspPkg.stBody.stCommRsp.iReason, rspPkg.stBody.stCommRsp.usMsgLen, rspPkg.stBody.stCommRsp.aszMsg );
	}
	return 0==iRet?rspPkg.stBody.stCommRsp.iRetCode:iRet;
}


/*********************************************************************
Description:	记录敏感日志
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

	if( false == CLogManager::GetInstance().WriteCMLog(GbkToUtf8("盗号自助系统"),//操作者
		StrToInt(ItemList.iServiceID),//业务ID
		optId,//操作接口ID
		StrToLLong(iUin),//操作目标QQ
		StrToInt(ItemList.iWordId),
		iResult,//操作结果：成功或失败
		GbkToUtf8((char*)strOptType.c_str()),//操作类型
		"",//ip
		ItemList.ICSID,//seqId 单号
		iRoleId,//操作目标角色ID
		GbkToUtf8((char*)vRoleName.c_str()),//操作目标角色名
		StrToInt(ItemList.iItem_Id),//操作的道具
		StrToInt(ItemList.iItem_Num),//道具数量
		GbkToUtf8((char*)strRequest.c_str()),//itemDes 描述
		"5",//操作原因:投诉处理
		GbkToUtf8((char*)strMemo.c_str()),//备注
		strCmd,//cmd
		6,//需求来源。6：盗号系统
		StrSerial))
	{
		RuleLOG->WriteLog("sorry！记录敏感日志失败！操作目标用户【%s】| 操作角色id【%s】 | 操作目标角色名【%s】| 道具ID【%s】 | 道具数量【%s】 | 操作类型【%s】| 操作原因【%s】 | 操作接口id【%d】|操作结果【%d】|serial:%s\n",
			iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult,StrSerial.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ItemList.ICSID.c_str(),iUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_ERROR, __FUNCTION__, \
			"sorry！记录敏感日志失败！操作目标用户【%s】,操作角色id【%s】, 操作目标角色名【%s】, 道具ID【%s】 | 道具数量【%s】, 操作类型【%s】,操作原因【%s】,操作接口id【%d】,操作结果【%d】\n",
			iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult);

		return -1;
	}
	RuleLOG->WriteLog("记录敏感日志成功！操作目标用户【%s】, 操作角色id【%s】, 操作目标角色名【%s】, 道具ID【%s】, 道具数量【%s】,操作类型【%s】, 操作原因【%s】, 操作接口id【%d】,操作结果【%d】|serial:%s\n\n",
		iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult,StrSerial.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, ItemList.ICSID.c_str(),iUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, \
		"记录敏感日志成功！操作目标用户【%s】, 操作角色id【%s】, 操作目标角色名【%s】, 道具ID【%s】, 道具数量【%s】, 操作类型【%s】, 操作原因【%s】, 操作接口id【%d】,操作结果【%d】\n",
		iUin.c_str(),iRoleId.c_str(),vRoleName.c_str(),ItemList.iItem_Id.c_str(),ItemList.iItem_Num.c_str(),strOptType.c_str(),strMemo.c_str(),optId,iResult);

	return 0;
}

/*********************************************************************
	Description:	发邮件
	@				
	@desc			
	@exception		none
	*/
int RobRuleLogic::SendWarnMail(const string& mailTitle, const string& mailContext)
{
	// 发邮件告警
	string mailTo("");// = "windhe@tencent.com";

	//发送邮件用户列表配置化 @modify by v_sjwang 2012/11/2 10:34:06
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
				WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "补送该物品记录已经存在，成功发送告警邮件到[%s]\n", mailTo.c_str() );
			}
			else
			{
				WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_ERROR, __FUNCTION__, "补送该物品记录已经存在，发送告警邮件到[%s]失败\n", mailTo.c_str() );
			}
			//end modified.
		}
	}
	WriteLogsRUN(TLOG_TYPE_ROBRULELOGIC, "", "", TLOG_SERVICE_ID,TLOG_LEVEL_WARNING, __FUNCTION__, "补送该物品记录已经存在，成功发邮件告警人数为【%d】，邮件内容【%s】\n", userNum ,mailContext.c_str());

	return iRet;
}

/*********************************************************************
	Description:	查询点券 fall_out值
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