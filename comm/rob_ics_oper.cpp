#include "rob_ics_oper.h"
#include "comm_value.h"
#include "comm_logmanager.h"

RobICSOper::RobICSOper()
{
	//调试日志
	ICSOPERLOG="./logs/"+GetCurDateDay()+"RobICSOper.log";
	ValidLOG = new CLog((const char*)ICSOPERLOG.c_str());
	ValidLOG->LogOn();
	iRet=0;
}

RobICSOper::~RobICSOper()
{
	if(ValidLOG!=NULL)
	{
		delete ValidLOG;
	}
}
/*******************************************************************
Description:	单据tbICSComplaint读取
Params:			单据的大小：size_ics_info，私有变量;
单据的读取数组： get_ics_info[] 传入变量;
return:			真实的获取到处理的单据大小
@exception		none
*/
unsigned int RobICSOper::ReadICSInfoList(ApplyAnaylseStep enumApplyAnaylseStep,Rob_Ics_Complaint get_ics_info[],int get_size)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	int interface_index=0;
	//分析单据阶段，
	//step和result状态值都为0
	//不同阶段读取的参数不同
	theParamIn["size"] = IntToStr(get_size);
	theParamIn["service_id"] = CRobConf::getConf()->GetGroupInfo()["group_id"];
	theParamIn["ics_source"] = CRobConf::getConf()->GetIcsValid()["ics_source"];

	ValidLOG->WriteLog("-------------Read Auto Rob Info Size[%d]---------------\n",get_size);
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "-------------Read Auto Rob Info Size[%d]---------------\n",get_size);
	ics_log_header header("","",TLOG_SERVICE_ID);
	switch(enumApplyAnaylseStep)
	{
	case STEP_FLITER_RUN_ICS:			// 单据过滤处理
		theParamIn["step"]=IntToStr(STEP_WAITING_RUN_ICS);
		theParamIn["result"]=IntToStr(STEP_WAITING_RUN_RESULT);
		break;
	case STEP_ROBSLOT_RUN_ICS:			// 登入登出
		theParamIn["step"]=IntToStr(STEP_FLITER_RUN_ICS);
		theParamIn["result"]=IntToStr(STEP_FLITER_RUN_RESULT_TURE);
		break;
	case STEP_ITEMFLOW_RUN_ICS:			// 物品流水
		theParamIn["step"]=IntToStr(STEP_ROBSLOT_RUN_ICS);
		theParamIn["result"]=IntToStr(STEP_ROBSLOT_RUN_RESULT_TURE);
		break;
	case STEP_ITEMTRACE_RUN_ICS:		// 物品追踪
		theParamIn["step"]=IntToStr(STEP_ITEMFLOW_RUN_ICS);
		theParamIn["result"]=IntToStr(STEP_ITEMFLOW_RUN_RESULT_TURE);
		break;
	case STEP_ITEMSEND_RUN_ICS:			// 物品补送
		theParamIn["step"]=IntToStr(STEP_ITEMTRACE_RUN_ICS);
		theParamIn["result"]=IntToStr(STEP_ITEMFTRACE_RUN_RESULT_TURE);
		break;
	case STEP_REPLAY_RUN_ICS:			// 单据回复
		interface_index=1;
		break;
	case STEP_OFFLINEMODULE_RUN_ICS:	// 离线模型分析
		interface_index = 2;
		theParamIn["step"] = "2,3";
		theParamIn["result"] = "21,31";
	default:
		return 0;
	}
	ValidLOG->WriteLog("[STEP]:::Read Auto Rob Info step[%s]\n", IntToStr(enumApplyAnaylseStep).c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP]:::Read Auto Rob Info step[%s]\n", IntToStr(enumApplyAnaylseStep).c_str());

	if ( 1 == interface_index )
	{
		ICS_Oper.GetFunction(header,theParamIn,"rob_ics_need_replay_del_list",theGetResult);
	}
	else if ( 2 == interface_index )
	{
		ICS_Oper.GetFunction(header, theParamIn, "rob_ics_info_dnf", theGetResult);
	}
	else
	{
		//ICS_Oper.GetFunction(header,theParamIn,"ics_info_list",theGetResult); 
		ICS_Oper.GetFunction(header,theParamIn,"ics_info_list_new",theGetResult); //modified by v_wxywu on 20140319正式测试环境分离
	}
	for ( unsigned int i = 0; i < theGetResult.size(); i++ )
	{
		SaveAutoHalfIcsInfo(get_ics_info[i], theGetResult[i]);
		ValidLOG->WriteLog("读取待处理单据结果:QueryStep[%d],ndx[%d],icsid[%s],uin[%s]\n", enumApplyAnaylseStep, i, get_ics_info[i].sICSId.c_str(), get_ics_info[i].sUin.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "读取待处理单据结果:QueryStep[%d],ndx[%d],icsid[%s],uin[%s]\n", enumApplyAnaylseStep, i, get_ics_info[i].sICSId.c_str(), get_ics_info[i].sUin.c_str() );
	}
	return theGetResult.size();
}


/*******************************************************************
Description:	单据tbICSComplaint读取
Params:			单据的大小：size_ics_info，私有变量;
				单据的读取数组： get_ics_info[] 传入变量;
return:			真实的获取到处理的单据大小
@exception		none
*/
unsigned int RobICSOper::ReadICSInfoOneItem(ApplyAnaylseStep enumApplyAnaylseStep,Rob_Ics_Complaint get_ics_info[],string ics_id)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	int interface_index=0;
	//分析单据阶段，
	//step和result状态值都为0
	//不同阶段读取的参数不同
	theParamIn["service_id"] = CRobConf::getConf()->GetGroupInfo()["group_id"];
	theParamIn["ics_id"] = ics_id;
	switch (enumApplyAnaylseStep)
	{
	case STEP_FLITER_RUN_ICS:			//单据过滤处理
		theParamIn["step"] = IntToStr(STEP_WAITING_RUN_ICS);
		theParamIn["result"] = IntToStr(STEP_WAITING_RUN_RESULT);
		break;
	case STEP_ROBSLOT_RUN_ICS:			//登入登出
		theParamIn["step"] = IntToStr(STEP_FLITER_RUN_ICS);
		theParamIn["result"] = IntToStr(STEP_FLITER_RUN_RESULT_TURE);
		break;
	case STEP_ITEMFLOW_RUN_ICS:			//物品流水
		theParamIn["step"] = IntToStr(STEP_ROBSLOT_RUN_ICS);
		theParamIn["result"] = IntToStr(STEP_ROBSLOT_RUN_RESULT_TURE);
		break;
	case STEP_ITEMTRACE_RUN_ICS:		//物品追踪
		theParamIn["step"] = IntToStr(STEP_ITEMFLOW_RUN_ICS);
		theParamIn["result"] = IntToStr(STEP_ITEMFLOW_RUN_RESULT_TURE);
		break;
	case STEP_ITEMSEND_RUN_ICS:			//物品补送
		theParamIn["step"] = IntToStr(STEP_ITEMTRACE_RUN_ICS);
		theParamIn["result"] = IntToStr(STEP_ITEMFTRACE_RUN_RESULT_TURE);
		break;
	case STEP_REPLAY_RUN_ICS:			//单据回复
		interface_index = 1;
		theParamIn["AnaylseResult"] = "(12,21,31,41,51,52)";
		break;
	case STEP_OFFLINEMODULE_RUN_ICS:	//离线模型分析
		interface_index = 2;
		theParamIn["step"] = "2,3";
		theParamIn["result"] = "21,31";
		break;
	default:
		return 0;
	}
	if ( 1 == interface_index )
	{
		ICS_Oper.GetFunction(ics_log_header("", "", CRobConf::getConf()->GetGroupInfo()["group_id"]), theParamIn, "rob_ics_need_reply_one", theGetResult);
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ics_id.c_str(), "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO, __FUNCTION__, "[rob_ics_need_reply_one]-------------读取指定待回复单据一条---------------\n" );
	}
	else if ( 2 == interface_index )
	{
		ICS_Oper.GetFunction(ics_log_header("", "", CRobConf::getConf()->GetGroupInfo()["group_id"]), theParamIn, "ics_info_one_dnf", theGetResult);
	}
	else
	{
		ICS_Oper.GetFunction(ics_log_header("", "", CRobConf::getConf()->GetGroupInfo()["group_id"]), theParamIn, "ics_info_one", theGetResult);
	}
	for ( unsigned int i = 0; i < theGetResult.size(); i++ )
	{
		SaveAutoHalfIcsInfo(get_ics_info[i], theGetResult[i]);
		ValidLOG->WriteLog("读取指定单据:icsid[%s],uin[%s],RoleId[%s],RoleName[%s]\n", get_ics_info[i].sICSId.c_str(), get_ics_info[i].sUin.c_str(), get_ics_info[i].sRoleID.c_str(), get_ics_info[i].sRoleName.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, get_ics_info[i].sICSId.c_str(), get_ics_info[i].sUin.c_str(),TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "读取指定单据:icsid[%s],uin[%s],RoleId[%s],RoleName[%s]\n", get_ics_info[i].sICSId.c_str(), get_ics_info[i].sUin.c_str(), get_ics_info[i].sRoleID.c_str(), get_ics_info[i].sRoleName.c_str() );
	}
	return theGetResult.size();
}

/*******************************************************************
Description:	单据内存的状态值
@exception		none
@return 		int 操作接口返回结果
*/

int RobICSOper::SaveAutoHalfIcsInfo(Rob_Ics_Complaint& _complaint_list, map<string,
									string>& input_params)
{
	int res = 0;

	_complaint_list.sID = input_params["iID"];
	_complaint_list.sServiceID = input_params["iServiceID"];
	_complaint_list.sICSId = input_params["iICSId"];
	_complaint_list.sUin = input_params["iRobed_Uin"];
	_complaint_list.sRoleID = input_params["vRobed_RoleId"]==""?"0":input_params["vRobed_RoleId"];
	_complaint_list.sRoleName = input_params["vRobed_RoleName"]==""?"0":input_params["vRobed_RoleName"];
	_complaint_list.sRoleLevel = input_params["iRobed_RoleLevel"]==""?"0":input_params["iRobed_RoleLevel"];
	_complaint_list.sWordId = input_params["iWordId"]==""?"0":input_params["iWordId"];
	_complaint_list.stBookTime = input_params["dtBookTime"]==""?"":input_params["dtBookTime"];
	_complaint_list.stLastLogin = input_params["dtLastLogin"]==""?"":input_params["dtLastLogin"];
	_complaint_list.stRobeLogin = input_params["dtRobedTime"]==""?"":input_params["dtRobedTime"];
	_complaint_list.sUpdateTime = input_params["dtUpdateTime"]==""?"":input_params["dtUpdateTime"];
	_complaint_list.sIsVip = input_params["iVip"]==""?"0":input_params["iVip"];
	_complaint_list.siSource = input_params["iSource"]==""?"0":input_params["iSource"];
	_complaint_list.sUndefined1 = input_params["Undefined1"]==""?"0":input_params["Undefined1"];
	_complaint_list.sUndefined2 = input_params["Undefined2"]==""?"0":input_params["Undefined2"];
	_complaint_list.sUndefined3 = input_params["Undefined3"]==""?"0":input_params["Undefined3"];
	_complaint_list.siRobedRoleBan = input_params["iRobed_RoleBan"]==""?"0":input_params["iRobed_RoleBan"];
	_complaint_list.siReplyResult = input_params["iReplyResult"]==""?"0":input_params["iReplyResult"];
	_complaint_list.sICSAnaylseResult = input_params["iICSAnaylseResult"]==""?"0":input_params["iICSAnaylseResult"];
	_complaint_list.sICSAnaylseStat =  input_params["iICSAnaylseStat"]==""?"0":input_params["iICSAnaylseStat"];
	_complaint_list.sICSAnaylseStep = input_params["iICSAnaylseStep"]==""?"0":input_params["iICSAnaylseStep"];

	return res;
}
/*******************************************************************
Description:	单据tbICSInfo的更新，主要是状态，结果，步骤的更新
@desc			包括单据在各种情况下的读取
@exception		none
*/
int RobICSOper::UpdateICSProc(std::string sID, ApplyAnaylseStep enumApplyAnaylseStep, ApplyAnaylseResult enumApplyAnaylseResult, ApplyAnaylseStat enumApplyAnaylseStat)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["id"]= sID;
	theParamIn["step"] = IntToStr(enumApplyAnaylseStep);
	theParamIn["result"] = IntToStr(enumApplyAnaylseResult);
	theParamIn["state"] = IntToStr(enumApplyAnaylseStat);
	theParamIn["service_id"] = CRobConf::getConf()->GetGroupInfo()["group_id"];
	
	ics_log_header header("","", TLOG_SERVICE_ID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_states_update",theGetResult);
}
/**
Description:	单据tbICSComplaint的回复结果和处理结果的更新
@desc			包括单据在各种情况下的更新
@exception		none
*/
int RobICSOper::updateICSReplyResult(Rob_Ics_Complaint& _reply_result){


	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["id"]= _reply_result.sID;
	theParamIn["content"] = string(GbkToUtf8((char*)_reply_result.sContent.c_str()));
	theParamIn["result"] = _reply_result.siReplyResult;
	theParamIn["service_id"] = CRobConf::getConf()->GetGroupInfo()["group_id"];
	ValidLOG->WriteLog("[STEP:INFO]:::RobICSOper::updateICSReplyResult:[service_id=%s&id=%s&content=%s&result=%s]\n",theParamIn["service_id"].c_str(),theParamIn["id"].c_str(),theParamIn["content"].c_str(),theParamIn["result"].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[STEP:INFO]:::RobICSOper::updateICSReplyResult:[service_id=%s&id=%s&content=%s&result=%s]\n",theParamIn["service_id"].c_str(),theParamIn["id"].c_str(),theParamIn["content"].c_str(),theParamIn["result"].c_str());

	ics_log_header header("","", TLOG_SERVICE_ID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_reply_update",theGetResult);
}

/*********************************************************************
Description:	单据tbICSRoleLogout插入
@desc			包括单据在读取异常登录时间段插入
@exception		none
*/
int RobICSOper::InsertICSRobSlotData(Rob_SLOT& robSlotTime){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["iServiceID"]= robSlotTime.ServiceID;
	theParamIn["iWordId"]= robSlotTime.WorldID;
	theParamIn["iICSid"]= robSlotTime.ICSID;
	theParamIn["vIP"] = robSlotTime.sBlackRobIp;
	theParamIn["vIPArea"] = string(GbkToUtf8((char*)robSlotTime.sRobIPName.c_str()));
	theParamIn["iRobed_Uin"] = robSlotTime.UserID;
	theParamIn["vRobed_RoleId"] = robSlotTime.RoleID;
	theParamIn["vRobed_RoleName"] = string(GbkToUtf8((char*)robSlotTime.RoleName.c_str()));
	theParamIn["dtLoginInTime"] = robSlotTime.LoginTime;
	theParamIn["dtLoginOutTime"] = robSlotTime.LogoutTime;
	theParamIn["iDuration"] = robSlotTime.OnLineTime;
	theParamIn["iMoney_Sub"] = robSlotTime.iMoney_Sub;
	ValidLOG->WriteLog("InsertICSRobSlotData:icsid=%s&dtLoginInTime=%s&dtLoginOutTime=%s|\n",\
		theParamIn["iICSid"].c_str(), \
		theParamIn["dtLoginInTime"].c_str(), \
		theParamIn["dtLoginOutTime"].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , theParamIn["iICSid"].c_str(), "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "InsertICSRobSlotData:icsid=%s&dtLoginInTime=%s&dtLoginOutTime=%s|\n",\
		theParamIn["iICSid"].c_str(), \
		theParamIn["dtLoginInTime"].c_str(), \
		theParamIn["dtLoginOutTime"].c_str());

	ics_log_header header(robSlotTime.ICSID, robSlotTime.UserID, robSlotTime.ServiceID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_rob_slot_insert",theGetResult);
}

/**********************************************************************
Description:	单据tbICSRoleLogout获取
@desc			包括单据在读取异常登录时间段获取
@exception		none
*/
int RobICSOper::ReadICSRobSlot(std::vector < Rob_SLOT > &robSlotInfo,std::string sServiceId,std::string sICS_Id){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	Rob_SLOT tempVar;
	robSlotInfo.clear();
	theParamIn["service_id"]= sServiceId;
	theParamIn["ics_id"]= sICS_Id;
	ValidLOG->WriteLog("【函数】ReadICSRobSlot()【入参】:ics_id=%s&service_id=%s\n",sICS_Id.c_str(), sServiceId.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "【函数】ReadICSRobSlot()【入参】:ics_id=%s&service_id=%s\n",sICS_Id.c_str(), sServiceId.c_str());

	ics_log_header header(sICS_Id, "", sServiceId);
	ICS_Oper.GetFunction(header,theParamIn,"ics_rob_logout_proc_list",theGetResult);
	if(theGetResult.size()<=0){
		//这里最好是枚举类型的，稍后修改？？？
		ValidLOG->WriteLog("【函数】ReadICSRobSlot()结果返回值为【0】【%d】",theGetResult.size());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "【函数】ReadICSRobSlot()结果返回值为【0】【%d】",theGetResult.size());

		return -10000;
	}else{
		ValidLOG->WriteLog("【函数】ReadICSRobSlot()结果返回值为【%d】\n",theGetResult.size());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "【函数】ReadICSRobSlot()结果返回值为【%d】\n",theGetResult.size());

		for(int j=0;j<theGetResult.size();j++){
			tempVar.ICSID = theGetResult[j]["iICSId"];
			tempVar.LoginTime = theGetResult[j]["dtLoginInTime"];
			tempVar.LogoutTime = theGetResult[j]["dtLoginOutTime"];
			tempVar.RoleID = theGetResult[j]["vRobed_RoleId"];
			tempVar.RoleName = theGetResult[j]["vRobed_RoleName"];
			tempVar.ServiceID = theGetResult[j]["iServiceID"];
			tempVar.UserID = theGetResult[j]["iRobed_Uin"];
			tempVar.WorldID = theGetResult[j]["iWordId"];
			tempVar.iMoney_Sub = theGetResult[j]["iMoney_Sub"];
			robSlotInfo.push_back(tempVar);
			ValidLOG->WriteLog("【函数】ReadICSRobSlot()循环赋值大小【%d】\n",robSlotInfo.size());
			WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "【函数】ReadICSRobSlot()循环赋值大小【%d】\n",robSlotInfo.size());

		}
		return 0;
	}
}
int RobICSOper::InsertICSRobItemFlowData(std::vector < Item_Flow_List > &robItemFlow,std::string sServiceId){

	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;

	ValidLOG->WriteLog("生成玩家的异常时间段内赃物流水程序\n");
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "生成玩家的异常时间段内赃物流水程序\n");

	for(int t=0;t<robItemFlow.size();t++){
		theParamIn["event_time"]= robItemFlow[t].dtEventTime;
		theParamIn["service_id"]= robItemFlow[t].iServiceID;
		theParamIn["ics_id"]= robItemFlow[t].ICSID;
		theParamIn["area"]= robItemFlow[t].iWordId;
		//被盗者
		theParamIn["uin"]= robItemFlow[t].iRobed_Uin;
		theParamIn["role_id"]= robItemFlow[t].vRobed_RoleId;
		theParamIn["role_name"] =  string(GbkToUtf8((char*)robItemFlow[t].vRobed_RoleName.c_str()));
		//盗号着
		theParamIn["rob_roleid"]= robItemFlow[t].vRobTo_RoleId;
		theParamIn["rob_uin"]= robItemFlow[t].iRobTo_Uin;
		theParamIn["rob_rolename"]=  string(GbkToUtf8((char*)robItemFlow[t].vRobTo_RoleName.c_str()));
		//物品信息
		theParamIn["item_id"]= robItemFlow[t].iItem_Id;
		theParamIn["item_unique"]= robItemFlow[t].iItem_Unique_Id;
		theParamIn["item_name"]= string(GbkToUtf8((char*)robItemFlow[t].iItem_Name.c_str()));
		ValidLOG->WriteLog("找到赃物ID【%s】名字【%s】\n",theParamIn["item_id"].c_str(),theParamIn["item_name"].c_str());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "找到赃物ID【%s】名字【%s】\n",theParamIn["item_id"].c_str(),theParamIn["item_name"].c_str());

		theParamIn["item_upgrade_level"]= robItemFlow[t].iItem_Attribute_1;
		theParamIn["item_type"]= robItemFlow[t].iItem_Type;
		theParamIn["item_value"]= robItemFlow[t].iItem_Price_Value;
		theParamIn["item_num"]= robItemFlow[t].iItem_Num;
		theParamIn["item_base_level"]=robItemFlow[t].iItem_Level;
		//动作信息
		theParamIn["action"]= robItemFlow[t].iFlowAction;
		theParamIn["action_dsc"]=  string(GbkToUtf8((char*)robItemFlow[t].vFlowDsc.c_str()));
		//后续处理信息
		theParamIn["def_action"]= robItemFlow[t].iDefineProc;

#ifdef dnf2
		//a.只处理基础等级物品，高等级物品不处理---针对全体用户适用
		//b.只处理强化等级<=15等级的物品---针对权限用户适用
		//c.对于加强物品且小于15级的,VIP用户补送，基础用户不补送


#endif
		ics_log_header header(robItemFlow[t].ICSID, robItemFlow[t].iRobed_Uin, robItemFlow[t].iServiceID);
		ICS_Oper.GetFunction(header,theParamIn,"ics_item_flow_general",theGetResult);
		ValidLOG->WriteLog("异常时间段内赃物流水程序，单据【%s】\n",theParamIn["ics_id"].c_str());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "异常时间段内赃物流水程序，单据【%s】\n",theParamIn["ics_id"].c_str());

	}
	return 0;
}

/************************************************************************
Description:	单据tbICSRoleLogout更新
@desc			包括单据在的运行阶段
@exception		none
*/
int RobICSOper::UpdateICSRobSlotProc(std::string sServiceId, std::string sICS_Id, ApplySlotStat eRunStat){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	ValidLOG->WriteLog("UpdateICSRobSlotProc单据【%s】,状态【%d】更新\n",sServiceId.c_str(),eRunStat);
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER ,sICS_Id.c_str(), "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "UpdateICSRobSlotProc单据【%s】,状态【%d】更新\n",sServiceId.c_str(),eRunStat);

	theParamIn["ics_id"]= sICS_Id;
	theParamIn["servcie_id"]= sServiceId;
	theParamIn["run_stat"]= IntToStr(eRunStat);

	ics_log_header header(sICS_Id, "", sServiceId);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_slot_runstat_update",theGetResult);
}


/************************************************************************
Description:单据tbICSItemStats的统计库的
@desc		包括单据在各种情况下的读取
@exception	none
*/
int RobICSOper::UpdateICSItemStatsProc(std::string sUpdateType,vector <string>& vUpdateNum,std::string sIcsId){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;

	theParamIn["ics_id"]= sIcsId;

	theParamIn["item_add_num"]= "0";
	theParamIn["item_high_level_num"] = "0";

	theParamIn["arm_add_num"]= "0";

	theParamIn["money_add_num"]= "0";

	//需要更新的字段分类
	if(sUpdateType=="Item"){
		theParamIn["item_add_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Arm"){
		theParamIn["arm_add_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Money"){
		theParamIn["money_add_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Item-HighLevel-Money"){

		ValidLOG->WriteLog("UpdateICSItemStatsProc类型【ItemAndMoney】,种类【%d】更新\n",vUpdateNum.size());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "UpdateICSItemStatsProc类型【ItemAndMoney】,种类【%d】更新\n",vUpdateNum.size());

		theParamIn["item_add_num"] = vUpdateNum[0];
		theParamIn["item_retrieve_num"] = vUpdateNum[1];
		theParamIn["item_high_level_num"] = vUpdateNum[2];
		theParamIn["money_add_num"]= vUpdateNum[3];
		ValidLOG->WriteLog("UpdateICSItemStatsProc类型【ItemAndMoney】,物品总数【%s】需要找回物品数【%s】高等级物品【%s】金钱总数【%s】更新\n",vUpdateNum[0].c_str(),vUpdateNum[1].c_str(),vUpdateNum[2].c_str(),vUpdateNum[3].c_str());
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "UpdateICSItemStatsProc类型【ItemAndMoney】,物品总数【%s】需要找回物品数【%s】高等级物品【%s】金钱总数【%s】更新\n",vUpdateNum[0].c_str(),vUpdateNum[1].c_str(),vUpdateNum[2].c_str(),vUpdateNum[3].c_str());

	}

	ics_log_header header(sIcsId, "", TLOG_SERVICE_ID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_stat_item_add",theGetResult);
}

/**
Description:	单据tbICSItemStats的统计库的最后补送物品统计更新
@desc			包括单据在各种情况下的读取
@exception		none
*/	
int RobICSOper::updateICSItemStatsResult(std::string sUpdateType,vector <string>& vUpdateNum,std::string sIcsId){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["ics_id"]= sIcsId;

	theParamIn["item_retrieve_num"]= "0";
	theParamIn["item_retrieve_level_num"] = "0";
	theParamIn["arm_retrieve_num"]= "0";
	theParamIn["money_retrieve_num"]= "0";

	//需要更新的字段分类
	if(sUpdateType=="Item"){
		theParamIn["item_retrieve_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Arm"){
		theParamIn["arm_retrieve_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Money"){
		theParamIn["money_retrieve_num"] = vUpdateNum[0];
	}else if(sUpdateType=="ItemHighLevel"){
		theParamIn["item_retrieve_level_num"] = vUpdateNum[0];
	}else if(sUpdateType=="Item-ItemHighLevel"){
		theParamIn["item_retrieve_num"] = vUpdateNum[0];
		theParamIn["item_retrieve_level_num"] = vUpdateNum[1];
	}
	ValidLOG->WriteLog("[PHASE]:::updateICSItemStatsResult:ics_id=%s&item_retrieve_num=%s&item_retrieve_level_num=%s&arm_retrieve_num=%s&money_retrieve_num=%s&\n",\
		theParamIn["ics_id"].c_str(),\
		theParamIn["item_retrieve_num"].c_str(),\
		theParamIn["item_retrieve_level_num"].c_str(),\
		theParamIn["arm_retrieve_num"].c_str(),\
		theParamIn["money_retrieve_num"].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , theParamIn["ics_id"].c_str(), "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[PHASE]:::updateICSItemStatsResult:ics_id=%s&item_retrieve_num=%s&item_retrieve_level_num=%s&arm_retrieve_num=%s&money_retrieve_num=%s&\n",\
		theParamIn["ics_id"].c_str(),\
		theParamIn["item_retrieve_num"].c_str(),\
		theParamIn["item_retrieve_level_num"].c_str(),\
		theParamIn["arm_retrieve_num"].c_str(),\
		theParamIn["money_retrieve_num"].c_str());

	ics_log_header header(sIcsId, "", TLOG_SERVICE_ID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_stat_result_update",theGetResult);
}


/*******************************************************************
Description:	单据tbICSItemStats的统计库的初始化插入
@desc			包括单据在各种情况下的读取
@exception		none
*/
int RobICSOper::AddICSItemStatsProc(std::string sIcsID,std::string sServiceID)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["ics_id"]= sIcsID;
	theParamIn["service_id"] = sServiceID;

	ics_log_header header(sIcsID, "", sServiceID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_stat_add",theGetResult);
}
/*******************************************************************
Description:	单据tbICSInfo的状态的基本信息的更新---在过滤系统之后
@desc			包括单据在各种情况下的读取
@exception		none
*/
int RobICSOper::UpdateICSRoleInfo(Rob_Ics_Complaint& new_ics_info)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	//where条件
	theParamIn["sID"] = new_ics_info.sID;
	//更新字段

	theParamIn["sRoleLevel"] = new_ics_info.sRoleLevel;
	theParamIn["sRoleName"] = string(GbkToUtf8((char*)new_ics_info.sRoleName.c_str()));
	theParamIn["sRoleID"] = new_ics_info.sRoleID;
	theParamIn["siRobedRoleBan"] = new_ics_info.siRobedRoleBan;
	theParamIn["sVip"]=new_ics_info.sIsVip;
	ValidLOG->WriteLog("[INFO] [function]UpdateICSRoleInfo::sId=%s&sRoleLevel=%s&sRoleName=%s&sRoleID=%s&siRobedRoleBan=%s&sVip=%s&\n",\
		theParamIn["sID"].c_str(),\
		theParamIn["sRoleLevel"].c_str(),\
		theParamIn["sRoleName"].c_str(),\
		theParamIn["sRoleID"].c_str(),\
		theParamIn["siRobedRoleBan"].c_str(),\
		theParamIn["sVip"].c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , theParamIn["sID"].c_str(), "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "[INFO] [function]UpdateICSRoleInfo::sId=%s&sRoleLevel=%s&sRoleName=%s&sRoleID=%s&siRobedRoleBan=%s&sVip=%s&\n",\
		theParamIn["sID"].c_str(),\
		theParamIn["sRoleLevel"].c_str(),\
		theParamIn["sRoleName"].c_str(),\
		theParamIn["sRoleID"].c_str(),\
		theParamIn["siRobedRoleBan"].c_str(),\
		theParamIn["sVip"].c_str());

	ics_log_header header(new_ics_info.sID, "", TLOG_SERVICE_ID);
	return ICS_Oper.GetFunction(header,theParamIn,"ics_info_role_update",theGetResult);

}

/*******************************************************************
Description:	单据tbICSItemFlow读取---在物品追踪系统之前
@desc			包括单据在各种情况下的读取
@exception		none
*/
int RobICSOper::ReadICSItemListProc(vector < Item_Flow_List >& robItemFlowInfo,std::string sIcsId){
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	Item_Flow_List sTempItemFlow;
	robItemFlowInfo.clear();
	theParamIn["sICSId"] = sIcsId;
	theParamIn["iDefineAction"] = "(1,2)"; //需要处理的单据,自动和人工的

	ValidLOG->WriteLog("---RobICSOper::ReadICSItemListProc---sICSId:%s\n",sIcsId.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::ReadICSItemListProc---sICSId:%s\n",sIcsId.c_str());

	ics_log_header header(sIcsId, "", TLOG_SERVICE_ID);
	iRet = ICS_Oper.GetFunction(header,theParamIn,"ics_item_flow_query_proc",theGetResult);
	if(iRet == 0){
		if(theGetResult.size()==0){
			//没有需要处理追踪的物品
			return STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS;
		}
		for(int j=0;j<theGetResult.size();j++){
			sTempItemFlow.dtEventTime = theGetResult[j]["dtEventTime"];
			sTempItemFlow.ICSID =theGetResult[j]["iICSId"];
			sTempItemFlow.iDefineProc = theGetResult[j]["iDefineAction"];
			sTempItemFlow.iFlowAction = theGetResult[j]["iFlowAction"];
			sTempItemFlow.iItem_Attribute_1 = theGetResult[j]["iItem_Attribute_1"];
			sTempItemFlow.iItem_Id = theGetResult[j]["iItem_Id"];
			sTempItemFlow.iItem_Level = theGetResult[j]["iItem_Level"];
			sTempItemFlow.iItem_Name = theGetResult[j]["iItem_Name"];
			sTempItemFlow.iItem_Num = theGetResult[j]["iItem_Num"];
			sTempItemFlow.iItem_Price_Value = theGetResult[j]["iItem_Price_Value"];
			sTempItemFlow.iItem_Type = theGetResult[j]["iItem_Type"];
			sTempItemFlow.iItem_Unique_Id = theGetResult[j]["iItem_Unique_Id"];
			sTempItemFlow.iRobed_Uin = theGetResult[j]["iRobed_Uin"];
			sTempItemFlow.iRobTo_Uin = theGetResult[j]["iRobTo_Uin"];
			sTempItemFlow.iServiceID = theGetResult[j]["iServiceID"];
			sTempItemFlow.iWordId = theGetResult[j]["iWordId"];
			sTempItemFlow.vFlowDsc = theGetResult[j]["iFlowDesc"];
			sTempItemFlow.vRobed_RoleId = theGetResult[j]["vRobed_RoleId"];
			sTempItemFlow.vRobed_RoleName = theGetResult[j]["vRobed_RoleName"];
			sTempItemFlow.vRobTo_RoleId = theGetResult[j]["vRobTo_RoleId"];
			sTempItemFlow.vRobTo_RoleName = theGetResult[j]["vRobTo_RoleName"];
			sTempItemFlow.iTraceResult = theGetResult[j]["iTraceResult"];
			sTempItemFlow.iTraceStat = theGetResult[j]["iTraceStat"];

			robItemFlowInfo.push_back(sTempItemFlow);
		}
	}
	ValidLOG->WriteLog("---RobICSOper::ReadICSItemListProc---GetItemFlowSize():%d\n",robItemFlowInfo.size());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::ReadICSItemListProc---GetItemFlowSize():%d\n",robItemFlowInfo.size());

	return 0;
}


int RobICSOper::InsertICSItemTrace(vector < Item_Trace_List >& robItemTrace){

	ValidLOG->WriteLog("---RobICSOper::InsertICSItemTrace---robItemTrace.size():%d\n",robItemTrace.size());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::InsertICSItemTrace---robItemTrace.size():%d\n",robItemTrace.size());

	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	for(int j=0;j<robItemTrace.size();j++){
		theParamIn.clear();
#ifdef dnf2
		//不需要补送的
		if(robItemTrace[j].iSendAction=="2")
		{
			continue;
		}
#endif
		theParamIn["trace_hand"]=robItemTrace[j].iTraceHand;
		theParamIn["item_base_level"]=robItemTrace[j].iItem_Level;
		theParamIn["area"]=robItemTrace[j].iWordId;
		theParamIn["role_name"]=string(GbkToUtf8((char*)robItemTrace[j].vRobed_RoleName.c_str()));
		theParamIn["event_time"]=robItemTrace[j].dtEventTime;
		theParamIn["rob_uin"]=robItemTrace[j].iRobTo_Uin;
		theParamIn["rob_roleid"]=robItemTrace[j].vRobTo_RoleId;
		theParamIn["rob_rolename"]=string(GbkToUtf8((char*)robItemTrace[j].vRobTo_RoleName.c_str()));
		theParamIn["item_id"]=robItemTrace[j].iItem_Id;
		theParamIn["item_unique"]=robItemTrace[j].iItem_Unique_Id;
		theParamIn["item_name"]=string(GbkToUtf8((char*)robItemTrace[j].iItem_Name.c_str()));
		theParamIn["item_upgrade_level"]=robItemTrace[j].iItem_Attribute_1;
		theParamIn["action"]=robItemTrace[j].iFlowAction;
		theParamIn["action_dsc"]=string(GbkToUtf8((char*)robItemTrace[j].vFlowDsc.c_str()));
		theParamIn["item_type"]=robItemTrace[j].iItem_Type;
		theParamIn["trace_result"]=robItemTrace[j].iTraceResult;
		theParamIn["item_num"]=robItemTrace[j].iItem_Num;
		theParamIn["ics_id"]=robItemTrace[j].ICSID;
		theParamIn["service_id"]=robItemTrace[j].iServiceID;
		theParamIn["uin"]=robItemTrace[j].iRobed_Uin;
		theParamIn["role_id"]=robItemTrace[j].vRobed_RoleId;
		theParamIn["send_action"]=robItemTrace[j].iSendAction;

		ics_log_header header(robItemTrace[j].ICSID, robItemTrace[j].iRobTo_Uin, robItemTrace[j].iServiceID);
		iRet = ICS_Oper.GetFunction(header,theParamIn,"ics_item_trace_general",theGetResult);
		ValidLOG->WriteLog("---RobICSOper::InsertICSItemTrace---[%d][iRet=%d]\n",j,iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::InsertICSItemTrace---[%d][iRet=%d]\n",j,iRet);

	}
	return 0;
}


/**
Description:	单据UpdateICSItemFlow状态的【全单据】的变更
@desc			包括单据在各种情况下的状态的【全单据】的变更
@exception		none
*/
int RobICSOper::UpdateICSItemFlow(std::string sIcsId,std::string sServiceID,ApplyItemFlowStat stat,ApplyItemFlowResult res){

	ValidLOG->WriteLog("---RobICSOper::UpdateICSItemFlow---sIcsId:%s&sServiceID=%s\n",sIcsId.c_str(),sServiceID.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::UpdateICSItemFlow---sIcsId:%s&sServiceID=%s\n",sIcsId.c_str(),sServiceID.c_str());

	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["ics_id"] = sIcsId;
	theParamIn["service_id"] = sServiceID;
	theParamIn["run_result"] = IntToStr(res);
	theParamIn["run_stat"] = IntToStr(stat);

	ics_log_header header(sIcsId, "", sServiceID);
	return  ICS_Oper.GetFunction(header,theParamIn,"ics_item_flow_runstat_update",theGetResult);
}


/**
Description:	单据tbICSItemTrace的读取赠送列表
@desc			包括单据在各种情况下的读取
@exception		none
*/

int RobICSOper::getICSItemSendList(vector < Item_Trace_List >& robItemList,std::string sIcsId,std::string sServiceId,int type){
	ValidLOG->WriteLog("---RobICSOper::getICSItemSendList---sIcsId:%s&sServiceID=%s&sProcType=%d\n",sIcsId.c_str(),sServiceId.c_str(),type);
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::getICSItemSendList---sIcsId:%s&sServiceID=%s&sProcType=%d\n",sIcsId.c_str(),sServiceId.c_str(),type);

	robItemList.clear();
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	theParamIn["ics_id"] = sIcsId;
	theParamIn["service_id"] = sServiceId;
	theParamIn["run_result"] = IntToStr(type);

	ics_log_header header(sIcsId, "", sServiceId);
	iRet =  ICS_Oper.GetFunction(header,theParamIn,"ics_item_send_query_proc",theGetResult);
	if(theGetResult.size()<=0){
		return STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST;
	}
	ValidLOG->WriteLog("---RobICSOper::getICSItemSendList---theGetResult.size():%d",theGetResult.size());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::getICSItemSendList---theGetResult.size():%d",theGetResult.size());

	for(int j=0;j<theGetResult.size();j++){
		//这里只需要赠送和变更后面数据的参数即可
		//单据信息
		struTempItemTrace.iID = theGetResult[j]["iID"];
		struTempItemTrace.ICSID = theGetResult[j]["iICSId"];
		struTempItemTrace.dtEventTime = theGetResult[j]["dtEventTime"];
		struTempItemTrace.iServiceID = theGetResult[j]["iServiceID"];
		struTempItemTrace.iSendAction = theGetResult[j]["iSendAction"];
		struTempItemTrace.iTraceHand = theGetResult[j]["iTraceHand"];
		struTempItemTrace.iSendResult =  theGetResult[j]["iSendResult"];
		//物品信息
		struTempItemTrace.iItem_Attribute_1 = theGetResult[j]["iItem_Attribute_1"];
		struTempItemTrace.iItem_Attribute_blob = theGetResult[j]["iItem_Attribute_blob"];
		struTempItemTrace.iItem_Id =  theGetResult[j]["iItem_Id"];
		struTempItemTrace.iItem_Level = theGetResult[j]["iItem_Level"];
		struTempItemTrace.iItem_Name = theGetResult[j]["iItem_Name"];
		struTempItemTrace.iItem_Num =  theGetResult[j]["iItem_Num"];
		struTempItemTrace.iItem_Type = theGetResult[j]["iItem_Type"];
		struTempItemTrace.iItem_Unique_Id =theGetResult[j]["iItem_Unique_Id"];
		//角色信息
		struTempItemTrace.iWordId = theGetResult[j]["iWordId"];
		struTempItemTrace.iRobed_Uin = theGetResult[j]["iRobed_Uin"];
		struTempItemTrace.iRobTo_Uin =  theGetResult[j]["iRobTo_Uin"];
		struTempItemTrace.vRobTo_RoleId = theGetResult[j]["vRobTo_RoleId"];
		struTempItemTrace.vRobed_RoleId =  theGetResult[j]["vRobed_RoleId"];
		struTempItemTrace.vRobed_RoleName =  theGetResult[j]["vRobed_RoleName"];
		struTempItemTrace.vRobTo_RoleName =  theGetResult[j]["vRobTo_RoleName"];

		//赋值
		robItemList.push_back(struTempItemTrace);
	}
	ValidLOG->WriteLog("---RobICSOper::getICSItemSendList---over!!!\n");
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::getICSItemSendList---over!!!\n");

	return 0;
}


//获取单据tbICSItemTrace的读取补送成功的数据列表
int RobICSOper::getICSItemSendSuccessList(const string iscid,const string &serid,const string &uin,std::vector< Item_Trace_List > &SuccessItemList)
{

	SuccessItemList.clear();
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;

	theParamIn["ics_id"] = iscid;
	theParamIn["service_id"] = serid;
	theParamIn["iRobed_Uin"] = uin;

	ics_log_header header(iscid, uin, serid);
	iRet =  ICS_Oper.GetFunction(header,theParamIn,"ics_item_send_success_query",theGetResult);
	if( theGetResult.size()<0 )
	{
		return -1;
	}

	ValidLOG->WriteLog("---RobICSOper::getICSItemSendSuccessList---theGetResult.size():%d",theGetResult.size());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::getICSItemSendSuccessList---theGetResult.size():%d",theGetResult.size());

	for(int j=0;j<theGetResult.size();j++)
	{
		//这里只需要赠送和变更后面数据的参数即可
		//单据信息
		struTempItemTrace.iID = theGetResult[j]["iID"];
		struTempItemTrace.ICSID = theGetResult[j]["iICSId"];
		struTempItemTrace.iServiceID = theGetResult[j]["iServiceID"];
		struTempItemTrace.iSendAction = theGetResult[j]["iSendAction"];
		struTempItemTrace.iTraceHand = theGetResult[j]["iTraceHand"];
		struTempItemTrace.iSendResult =  theGetResult[j]["iSendResult"];
		//物品信息
		struTempItemTrace.iItem_Attribute_1 = theGetResult[j]["iItem_Attribute_1"];
		struTempItemTrace.iItem_Attribute_blob = theGetResult[j]["iItem_Attribute_blob"];
		struTempItemTrace.iItem_Id =  theGetResult[j]["iItem_Id"];
		struTempItemTrace.iItem_Level = theGetResult[j]["iItem_Level"];
		struTempItemTrace.iItem_Name = theGetResult[j]["iItem_Name"];
		struTempItemTrace.iItem_Num =  theGetResult[j]["iItem_Num"];
		struTempItemTrace.iItem_Type = theGetResult[j]["iItem_Type"];
		struTempItemTrace.iItem_Unique_Id =theGetResult[j]["iItem_Unique_Id"];
		//角色信息
		struTempItemTrace.iWordId = theGetResult[j]["iWordId"];
		struTempItemTrace.iRobed_Uin = theGetResult[j]["iRobed_Uin"];
		struTempItemTrace.iRobTo_Uin =  theGetResult[j]["iRobTo_Uin"];
		struTempItemTrace.vRobTo_RoleId = theGetResult[j]["vRobTo_RoleId"];
		struTempItemTrace.vRobed_RoleId =  theGetResult[j]["vRobed_RoleId"];
		struTempItemTrace.vRobed_RoleName =  theGetResult[j]["vRobed_RoleName"];
		struTempItemTrace.vRobTo_RoleName =  theGetResult[j]["vRobTo_RoleName"];

		//赋值
		SuccessItemList.push_back(struTempItemTrace);
	}
	ValidLOG->WriteLog("---RobICSOper::getICSItemSendSuccessList---over!!!\n");
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::getICSItemSendSuccessList---over!!!\n");

	return 0;
}


/**
Description:	单据tbICSItemTrace的赠送结果和处理结果的更新
@desc			包括单据在各种情况下的更新
@exception		none
*/
int RobICSOper::updateICSItemSendList(vector < Item_Trace_List >& robItemList){

	ValidLOG->WriteLog("---RobICSOper::updateICSItemSendList---sIcsId:%s&sServiceID=%s\n",robItemList[0].ICSID.c_str(),robItemList[0].iServiceID.c_str());
	WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::updateICSItemSendList---sIcsId:%s&sServiceID=%s\n",robItemList[0].ICSID.c_str(),robItemList[0].iServiceID.c_str());

	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;


	for(int i=0;i<robItemList.size();i++){
		theParamIn.clear();
		theParamIn["i_id"] = robItemList[i].iID;
		theParamIn["service_id"] = robItemList[i].iServiceID;
		theParamIn["send_result"] = robItemList[i].iSendResult;

		ics_log_header header("", "", robItemList[i].iServiceID);
		iRet = ICS_Oper.GetFunction(header,theParamIn,"ics_item_send_runstat_update",theGetResult);
		ValidLOG->WriteLog("---RobICSOper::updateICSItemSendList---iRet:%d\n",iRet);
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "---RobICSOper::updateICSItemSendList---iRet:%d\n",iRet);

	}
	return 0;
}


/*********************************************************************
Description:	插入补送物品信息 
@				
ItemInfo: 输入参数
@desc			theResult : 错误信息
返回值：0-成功 非0-失败
@exception		none
*/
int RobICSOper::InsertSendInfo(Item_Trace_List &ItemInfo, vector < map<string, string> > &theResult)
{
	map<string,string> inParamsIst;
	inParamsIst.clear();
	//inParamsIst["iID"                 ] = ItemInfo.iID
	inParamsIst["iICSId"              ] = ItemInfo.ICSID;
	inParamsIst["iServiceID"          ] = ItemInfo.iServiceID;
	inParamsIst["iRobed_Uin"          ] = ItemInfo.iRobed_Uin;
	inParamsIst["vRobed_RoleId"       ] = ItemInfo.vRobed_RoleId;
	inParamsIst["vRobed_RoleName"     ] = ItemInfo.vRobed_RoleName;
	inParamsIst["iWordId"             ] = ItemInfo.iWordId;
	//inParamsIst["dtUpdateTime"        ] = ItemInfo.dtUpdateTime;
	inParamsIst["dtEventTime"         ] = ItemInfo.dtEventTime;
	inParamsIst["iItem_Id"            ] = ItemInfo.iItem_Id;
	inParamsIst["iItem_Unique_Id"     ] = ItemInfo.iItem_Unique_Id;
	inParamsIst["iItem_Num"           ] = ItemInfo.iItem_Num;
	inParamsIst["iSendResult"         ] = ItemInfo.iSendResult;
	inParamsIst["iItem_Attribute_blob"] = ItemInfo.iItem_Attribute_blob;
	inParamsIst["vMemo"               ] = "";
	inParamsIst["Undefined1"          ] = ItemInfo.iItem_Type;

	ics_log_header headerIst(ItemInfo.ICSID, ItemInfo.iRobTo_Uin, ItemInfo.iServiceID);
	iRet = ICS_Oper.GetFunction( headerIst, inParamsIst, "ics_item_send_info_insert", theResult );
	if ( iRet == 0 )
	{
		ValidLOG->WriteLog("补送物品前，插入补送物品结果成功\n");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "补送物品前，插入补送物品结果成功" );
	}
	else
	{
		ValidLOG->WriteLog("补送物品前，插入补送物品结果失败\n");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "补送物品前，插入补送物品结果失败" );
	}
	return iRet;
}


/*********************************************************************
Description:	更新物品补送结果 
@				
ItemInfo: 输入参数
@desc			theResult : 错误信息
返回值：0-成功 非0-失败
@exception		none
*/
int RobICSOper::UpdateSendResult(Item_Trace_List &ItemInfo, vector < map<string, string> > &theResult)
{
	map<string,string> inParams;
	inParams.clear();
	inParams["iRobed_Uin"] = ItemInfo.iRobed_Uin;
	inParams["Guid"] = ItemInfo.iItem_Unique_Id;
	inParams["iSendResult"] = ItemInfo.iSendResult;
	inParams["iServiceID"] = ItemInfo.iServiceID;
	inParams["dtEventTime"] = ItemInfo.dtEventTime;

	ics_log_header header(ItemInfo.ICSID, ItemInfo.iRobTo_Uin, ItemInfo.iServiceID);
	int iRet = ICS_Oper.GetFunction( header, inParams, "ics_item_send_result_update", theResult );
	if ( 0 == iRet )
	{
		ValidLOG->WriteLog("更新补送结果成功\n");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "更新补送结果成功" );
	}
	else
	{
		ValidLOG->WriteLog("更新补送结果失败\n");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "更新补送结果失败" );
	}
	return iRet;
}

/*********************************************************************
Description:	重复补送物品，邮件预警、更新邮件预警次数 
@				
ItemInfo: 输入参数
@desc			theResult : 错误信息
返回值：0-成功 非0-失败
@exception		none
*/
int RobICSOper::UpdateWarnMailCount(Item_Trace_List &ItemInfo, vector < map<string, string> > &theResult)
{
	map<string,string> inParams;
	inParams.clear();
	inParams["iRobed_Uin"] = ItemInfo.iRobed_Uin;
	inParams["Guid"] = ItemInfo.iItem_Unique_Id;
	inParams["iServiceID"] = ItemInfo.iServiceID;
	inParams["dtEventTime"] = ItemInfo.dtEventTime;

	ics_log_header header(ItemInfo.ICSID, ItemInfo.iRobTo_Uin, ItemInfo.iServiceID);
	int iRet = ICS_Oper.GetFunction( header, inParams, "ics_item_send_warn_mail_update", theResult );
	if ( 0 == iRet )
	{
		ValidLOG->WriteLog("更新预警邮件次数成功");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "更新预警邮件次数成功" );
	}
	else
	{
		ValidLOG->WriteLog("更新预警邮件次数失败");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "更新预警邮件次数失败" );
	}
	return iRet;
}

/*********************************************************************
Description:	查询重复补送物品列表
@				
ItemInfo: 输入参数
@desc			theResult : 查询结果集
返回值：-1	查询失败
0	查询成功，没有重复补送记录
1	查询成功，有重复补送记录
@exception		none
*/
int RobICSOper::GetReSendList(Item_Trace_List &ItemInfo, vector < map<string, string> > &theResult)
{
	map<string,string> inParams;
	inParams.clear();
	inParams["iRobed_Uin"] = ItemInfo.iRobed_Uin;
	inParams["Guid"] = ItemInfo.iItem_Unique_Id;
	inParams["iServiceID"] = ItemInfo.iServiceID;
	inParams["dtEventTime"] = ItemInfo.dtEventTime;
	inParams["item_id"] = ItemInfo.iItem_Id;

	ics_log_header header(ItemInfo.ICSID, ItemInfo.iRobTo_Uin, ItemInfo.iServiceID);
	int iRet = ICS_Oper.GetFunction( header, inParams, "ics_item_send_result_select", theResult );
	if ( 0 != iRet )
	{
		ValidLOG->WriteLog("查询重复补送物品列表失败\n");
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_ERROR, __FUNCTION__, "查询重复补送物品列表失败" );
		return -1;
	}
	else if ( theResult.size() != 0 )
	{
		ValidLOG->WriteLog("查询重复补送物品列表成功，查询到重复补送记录[%d]条\n", theResult.size() );
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_WARNING, __FUNCTION__, "查询重复补送物品列表成功，查询到重复补送记录[%d]条", theResult.size() );
		return 1;
	}
	else
	{
		ValidLOG->WriteLog("查询重复补送物品列表成功，未查询到重复补送记录\n" );
		WriteLogsRUN(TLOG_TYPE_ROBICSOPER, ItemInfo.ICSID.c_str(), ItemInfo.iRobed_Uin.c_str(), TLOG_SERVICE_ID, TLOG_LEVEL_INFO, __FUNCTION__, "查询重复补送物品列表成功，未查询到重复补送记录" );
		return 0;
	}
}

/*******************************************************************
Description:	单据tbICSItemStats的统计库的数据获取added by v_zhtang on 2013-4-22
@desc			
@exception		none
*/
string RobICSOper::GetICSItemStats(std::string sIcsID,std::string sServiceID)
{
	map<string,string> theParamIn;
	vector < map<string, string> > theGetResult;
	int iRet = 0;

	theParamIn["ics_id"]= sIcsID;
	theParamIn["service_id"] = CRobConf::getConf()->GetGroupInfo()["group_id"];
	iRet = ICS_Oper.GetFunction(ics_log_header(sIcsID,"",CRobConf::getConf()->GetGroupInfo()["group_id"]),\
		theParamIn,"ics_stat_item_send",theGetResult);

	if (0 == iRet && theGetResult.size() == 1)
	{
		return theGetResult[0]["iItem_Retrieved_Num_1"] == "" ? "0" : theGetResult[0]["iItem_Retrieved_Num_1"];
	}

	return "0";
}

