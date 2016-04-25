/**
@file msg.h

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		Darrenqiu
@version	1.0
@date		2010/10/09

history:
<PRE>
History ID	:	1
Author		:	Darrenqiu
DateTime	:	2010/10/09
Description :	Create
</PRE>
*/

#ifndef __MSG_H__
#define __MSG_H__


#include <string>


//宏定义，主要是处理标准日志的日志头定义 2-4列
//|时间|单号|QQ|业务ID|接口ID|输入参数列表（IDIP方式）|
struct ics_log_header
{
	std::string ICSID;						//ICS单号
	std::string UserID;						//用户帐号，QQ
	std::string ServiceID;					//大区

	ics_log_header():ICSID(""),
		UserID(""),
		ServiceID("")
	{}

	ics_log_header(const std::string& ICSID,
		const std::string& UserID, 
		const std::string& ServiceID)
		:ICSID(ICSID),
		UserID(UserID),
		ServiceID(ServiceID){}
};
/*****************************************V1.01*************************************************/
/**
@struct Rob_Ics_Complaint,用于对应原始填单数据表
@brief	 
@details 用于半自助盗号数据库信息的内存处理
alert by heyuefeng 2012/01/13
*/
struct Rob_Ics_Complaint
{
	std::string sID;				// 表的主键ID
	std::string sServiceID;			// 业务ID
	std::string sICSId;				// ICS单号
	std::string sUin;				// 用户ID
	std::string sRoleID;			// 角色ID
	std::string sRoleName;			// 角色名
	std::string sRoleLevel;			// 角色等级
	std::string sWordId;			// 大区ID
	std::string stBookTime;			// ICS填单时间
	std::string stLastLogin;		// 玩家最后登录时间
	std::string stRobeLogin;		// 玩家发现被盗时间
	std::string sUpdateTime;		// 单据更新处理时间
	std::string sContent; 			// ICS填单内容
	std::string sSolution; 			// ICS解决内容
	std::string sIsVip;				// 用户VIP等级
	std::string sICSAnaylseStep;	// 自助对单据分析步骤
	std::string sICSAnaylseResult;	// 自助对单据分析结果
	std::string sICSAnaylseStat;	// 自助对单据过滤分析结果描述，主要针对的是过滤结果的反馈
	std::string siRobedRoleBan;		// 是否封号
	std::string siReplyResult;		// 是否回复处理结果
	std::string siSource;			// 来源
	std::string sUndefined1;		// 保留字段1
	std::string sUndefined2;		// 保留字段2
	std::string sUndefined3;		// 保留字段3

	Rob_Ics_Complaint()
		:sID(""),
		sServiceID(""),
		sICSId(""),
		sUin(""),
		sRoleID(""),
		sRoleName(""),
		sRoleLevel(""),
		sWordId(""),
		stBookTime(""),
		stLastLogin(""),
		stRobeLogin(""),
		sContent(""),
		sSolution(""),
		sIsVip(""),
		sICSAnaylseStep(""),
		sICSAnaylseResult(""),
		sICSAnaylseStat(""),
		siRobedRoleBan(""),
		siReplyResult(""),
		siSource(""),
		sUndefined1(""),
		sUndefined2(""),
		sUndefined3("")
		{
		}

	Rob_Ics_Complaint(const std::string &sID,
		const std::string &sServiceID,
		const std::string &sICSId,
		const std::string &sUin,
		const std::string &sRoleID,
		const std::string &sRoleName,
		const std::string &sRoleLevel,
		const std::string &sWordId,
		const std::string &stBookTime,
		const std::string &stLastLogin,
		const std::string &stRobeLogin,
		const std::string &sContent,
		const std::string &sSolution,
		const std::string &sIsVip,
		const std::string &sICSAnaylseStep,
		const std::string &sICSAnaylseResult,
		const std::string &sICSAnaylseStat,
		const std::string &siRobedRoleBan,
		const std::string &siReplyResult,
		const std::string $siSource,
		const std::string &sUndefined1,
		const std::string &sUndefined2,
		const std::string &sUndefined3
		)
		:sID(sID),
		sServiceID(sServiceID),
		sICSId(sICSId),
		sUin(sUin),
		sRoleID(sRoleID),
		sRoleName(sRoleName),
		sRoleLevel(sRoleLevel),
		sWordId(sWordId),
		stBookTime(stBookTime),
		stLastLogin(stLastLogin),
		stRobeLogin(stRobeLogin),
		sContent(sContent),
		sSolution(sSolution),
		sIsVip(sIsVip),
		sICSAnaylseStep(sICSAnaylseStep),
		sICSAnaylseResult(sICSAnaylseResult),
		sICSAnaylseStat(sICSAnaylseStat),
		siRobedRoleBan(siRobedRoleBan),
		siReplyResult(siReplyResult),
		siSource(siSource),
		sUndefined1(sUndefined1),
		sUndefined2(sUndefined2),
		sUndefined3(sUndefined3)
		{
		}
};
/** 
@struct Rob_Login_Logout_SLOT
@brief	 
@details 主要是定位标准日志的全局变量
*/
typedef struct Rob_Login_Logout_SLOT
{
	std::string ICSID;			// ICS单号
	std::string ServiceID;		// 游戏业务ID
	std::string UserID;			// 用户帐号，QQ
	std::string WorldID;		// 大区
	std::string RoleID;			// 角色ID
	std::string RoleName;		// 角色名称
	std::string	sBlackRobIp;	// 盗号的IP
	std::string sRobIPName;		// 盗号者的地区
	std::string	LoginTime;		// 登录时间
	std::string	LogoutTime;		// 登出时间
	std::string	OnLineTime;		// 在线时长
	std::string iMoney_Sub;		// 金钱减少个数
	std::string	iRobFlag;		// 是否是盗号日志

	Rob_Login_Logout_SLOT():ICSID(""),
	ServiceID(""),
	UserID(""),
	WorldID(""),
	RoleID(""),
	RoleName(""),
	sBlackRobIp(""),
	sRobIPName(""),
	LoginTime(""),
	LogoutTime(""),
	OnLineTime(""),
	iMoney_Sub(""),
	iRobFlag("")
	{}

	Rob_Login_Logout_SLOT(const std::string& ICSID,
		const std::string& ServiceID,
		const std::string& UserID, 
		const std::string& WorldID, 
		const std::string& RoleID,
		const std::string& RoleName,
		const std::string& sBlackRobIp,
		const std::string& sRobIPName,
		const std::string& LoginTime,
		const std::string& LogoutTime,
		const std::string& OnLineTime,
		const std::string& iMoney_Sub,
		const std::string& iRobFlag
			)
		:ICSID(ICSID),
		ServiceID(ServiceID),
		UserID(UserID),
		WorldID(WorldID),
		RoleID(RoleID),
		RoleName(RoleName),
		sBlackRobIp(sBlackRobIp),
		sRobIPName(sRobIPName),
		LoginTime(LoginTime),
		LogoutTime(LogoutTime),
		OnLineTime(OnLineTime),
		iMoney_Sub(iMoney_Sub),
		iRobFlag(iRobFlag)
		{}
}Rob_SLOT;


/** 
@struct Rob_Item_Flow
@brief	 
@details	 
*/
/*
///////////////////////////////////////////////////////////
select * from tbMoneySub_9  limit 3;

mysql> select sum(iNumber) from tbMoneySub_9  where dtDateTime between '2012-01-29' and '2012-02-01' and iUserId=729700609;         
+--------------+
| sum(iNumber) |
+--------------+
|         2402 | 
+--------------+
1 row in set (5.84 sec)

mysql> select sum(iNumber) from tbMoneySub_9  where dtDateTime between '2012-01-29' and '2012-02-01' and iUserId=734968009;
+--------------+
| sum(iNumber) |
+--------------+
|     10746634 | 
+--------------+
///////////////////////////////////////////////////////////
tbItemSub
asher，这个表是物品减少的表
yangyihe(贺扬易) 2012-02-24 14:22:58
if(vLostData[j]["iReason"] == "0" ||vLostData[j]["iReason"] == "5" ||vLostData[j]["iReason"] == "9")
                {
                    Replace(vLostData[j]["vGuid"],"\n","");
                    Replace(vLostData[j]["vGuid"],"\r","");
                    Replace(vLostData[j]["vGuid"],"\t","");

                    vDataResult.push_back(vLostD
yangyihe(贺扬易) 2012-02-24 14:22:58
ata[j]);
                }
yangyihe(贺扬易) 2012-02-24 14:23:17
0 5 9的是消失的，直接补送

mysql> select * from tbItemSub_9 where iUpgrade!=0 limit 3;
+------------+---------------------+-------------+-------+---------+-----------+---------+---------+--------------+----------------------+-------+----------+
| iUserId    | dtDateTime          | vCharacName | iKind | iItemNO | iCurTotal | iNumber | iReason | vOCharacName | vGuid                | iSeal | iUpgrade |
+------------+---------------------+-------------+-------+---------+-----------+---------+---------+--------------+----------------------+-------+----------+
| 1336918309 | 2012-02-08 03:30:09 | U628        |     1 |   37332 |         0 |       1 |       0 | NULL         | 9e3a0000ad7b314f249f |      |        4 | 
| 1336918309 | 2012-02-08 03:31:27 | U628        |     1 |   35321 |         0 |       1 |       9 | NULL         | 9e3a0000f275314f249e |      |        5 | 
| 1598881209 | 2012-02-08 03:27:56 | ????B       |     1 |   18865 |         0 |       1 |       9 | NULL         | bb3a0000357b314f1ee1 |      |        2 | 
+------------+---------------------+-------------+-------+---------+-----------+---------+---------+--------------+----------------------+-------+----------+
*/
typedef struct Rob_Item_Flow
{
	std::string ICSID;
	std::string iServiceID;
	std::string iWordId;
	std::string iRobed_Uin;
	std::string vRobed_RoleId;
	std::string vRobed_RoleName;
	std::string dtEventTime;
	std::string iRobTo_Uin;
	std::string vRobTo_RoleId;
	std::string vRobTo_RoleName;
	std::string iItem_Id;
	std::string iItem_Type;
	std::string iItem_Unique_Id;    //0、未处理，需要程序自动处理；1、叠加物品需要人工处理；
	std::string iItem_Name;
	std::string iItem_Num;
	std::string iItem_Level;
	std::string iItem_Price_Value;
	std::string iItem_Attribute_blob;
	std::string iItem_Attribute_1;
	std::string iItem_Attribute_2;
	std::string iItem_Attribute_3;
	std::string iFlowAction;
	std::string vFlowDsc;
	std::string iDefineProc;
	std::string iTraceResult;
	std::string iTraceStat;

	Rob_Item_Flow()
		:ICSID(""),
		iServiceID(""),
		iWordId(""),
		iRobed_Uin(""),
		vRobed_RoleId(""),
		vRobed_RoleName(""),
		dtEventTime(""),
		iRobTo_Uin(""),
		vRobTo_RoleId(""),
		vRobTo_RoleName(""),
		iItem_Id(""),
		iItem_Type(""),
		iItem_Unique_Id(""),
		iItem_Name(""),
		iItem_Num(""),
		iItem_Level(""),
		iItem_Price_Value(""),
		iItem_Attribute_blob(""),
		iItem_Attribute_1(""),
		iItem_Attribute_2(""),
		iItem_Attribute_3(""),
		iFlowAction(""),
		vFlowDsc(""),
		iDefineProc(""),
		iTraceResult(""),
		iTraceStat("")
	{}

	Rob_Item_Flow(const std::string& ICSID,
		const std::string& iServiceID,
		const std::string& iWordId,
		const std::string& iRobed_Uin,
		const std::string& vRobed_RoleId,
		const std::string& vRobed_RoleName,
		const std::string& dtEventTime,
		const std::string& iRobTo_Uin,
		const std::string& vRobTo_RoleId,
		const std::string& vRobTo_RoleName,
		const std::string& iItem_Id,
		const std::string& iItem_Type,
		const std::string& iItem_Unique_Id,
		const std::string& iItem_Name,
		const std::string& iItem_Num,
		const std::string& iItem_Level,
		const std::string& iItem_Price_Value,
		const std::string& iItem_Attribute_blob,
		const std::string& iItem_Attribute_1,
		const std::string& iItem_Attribute_2,
		const std::string& iItem_Attribute_3,
		const std::string& iFlowAction,
		const std::string& vFlowDsc,
		const std::string& iDefineProc,
		const std::string& iTraceResult,
		const std::string& iTraceStat)

		:ICSID(ICSID),
		iServiceID(iServiceID),
		iWordId(iWordId),
		iRobed_Uin(iRobed_Uin),
		vRobed_RoleId(vRobed_RoleId),
		vRobed_RoleName(vRobed_RoleName),
		dtEventTime(dtEventTime),
		iRobTo_Uin(iRobTo_Uin),
		vRobTo_RoleId(vRobTo_RoleId),
		vRobTo_RoleName(vRobTo_RoleName),
		iItem_Id(iItem_Id),
		iItem_Type(iItem_Type),
		iItem_Unique_Id(iItem_Unique_Id),
		iItem_Name(iItem_Name),
		iItem_Num(iItem_Num),
		iItem_Level(iItem_Level),
		iItem_Price_Value(iItem_Price_Value),
		iItem_Attribute_blob(iItem_Attribute_blob),
		iItem_Attribute_1(iItem_Attribute_1),
		iItem_Attribute_2(iItem_Attribute_2),
		iItem_Attribute_3(iItem_Attribute_3),
		iFlowAction(iFlowAction),
		vFlowDsc(vFlowDsc),
		iDefineProc(iDefineProc),
		iTraceResult(iTraceResult),
		iTraceStat(iTraceStat)
	{}
}Item_Flow_List;	// end spoil_info

typedef struct Rob_Trace_Flow
{
	std::string iID;
	std::string ICSID;
	std::string iServiceID;
	std::string iWordId;
	std::string iRobed_Uin;
	std::string vRobed_RoleId;
	std::string vRobed_RoleName;
	std::string dtEventTime;
	std::string iRobTo_Uin;
	std::string vRobTo_RoleId;
	std::string vRobTo_RoleName;
	std::string iItem_Id;
	std::string iItem_Type;
	std::string iItem_Unique_Id;	// 0、未处理，需要程序自动处理；1、叠加物品需要人工处理；
	std::string iItem_Name;
	std::string iItem_Num;
	std::string iItem_Level;
	std::string iItem_Attribute_blob;
	std::string iItem_Attribute_1;
	std::string iItem_Attribute_2;
	std::string iItem_Attribute_3;
	std::string iFlowAction;
	std::string vFlowDsc;
	std::string iTraceHand;
	std::string iSendAction;
	std::string iTraceResult;
	std::string iSendResult;
	std::string Undefined1;
	Rob_Trace_Flow()
		:iID(""),
		ICSID(""),
		iServiceID(""),
		iWordId(""),
		iRobed_Uin(""),
		vRobed_RoleId(""),
		vRobed_RoleName(""),
		dtEventTime(""),
		iRobTo_Uin(""),
		vRobTo_RoleId(""),
		vRobTo_RoleName(""),
		iItem_Id(""),
		iItem_Type(""),
		iItem_Unique_Id(""),
		iItem_Name(""),
		iItem_Num(""),
		iItem_Level(""),
		iItem_Attribute_blob(""),
		iItem_Attribute_1(""),
		iItem_Attribute_2(""),
		iItem_Attribute_3(""),
		iFlowAction(""),
		vFlowDsc(""),
		iTraceHand(""),
		iSendAction(""),
		iTraceResult(""),
		iSendResult(""),
		Undefined1("")
	{}

	Rob_Trace_Flow(
		const std::string& iID,
		const std::string& ICSID,
		const std::string& iServiceID,
		const std::string& iWordId,
		const std::string& iRobed_Uin,
		const std::string& vRobed_RoleId,
		const std::string& vRobed_RoleName,
		const std::string& dtEventTime,
		const std::string& iRobTo_Uin,
		const std::string& vRobTo_RoleId,
		const std::string& vRobTo_RoleName,
		const std::string& iItem_Id,
		const std::string& iItem_Type,
		const std::string& iItem_Unique_Id,
		const std::string& iItem_Name,
		const std::string& iItem_Num,
		const std::string& iItem_Level,
		const std::string& iItem_Price_Value,
		const std::string& iItem_Attribute_blob,
		const std::string& iItem_Attribute_1,
		const std::string& iItem_Attribute_2,
		const std::string& iItem_Attribute_3,
		const std::string& iFlowAction,
		const std::string& vFlowDsc,
		const std::string& iTraceHand,
		const std::string& iSendAction,
		const std::string& iTraceResult,
		const std::string& iSendResult,
		const std::string& Undefined1)
		:iID(iID),
		ICSID(ICSID),
		iServiceID(iServiceID),
		iWordId(iWordId),
		iRobed_Uin(iRobed_Uin),
		vRobed_RoleId(vRobed_RoleId),
		vRobed_RoleName(vRobed_RoleName),
		dtEventTime(dtEventTime),
		iRobTo_Uin(iRobTo_Uin),
		vRobTo_RoleId(vRobTo_RoleId),
		vRobTo_RoleName(vRobTo_RoleName),
		iItem_Id(iItem_Id),
		iItem_Type(iItem_Type),
		iItem_Unique_Id(iItem_Unique_Id),
		iItem_Name(iItem_Name),
		iItem_Num(iItem_Num),
		iItem_Level(iItem_Level),
		iItem_Attribute_blob(iItem_Attribute_blob),
		iItem_Attribute_1(iItem_Attribute_1),
		iItem_Attribute_2(iItem_Attribute_2),
		iItem_Attribute_3(iItem_Attribute_3),
		iFlowAction(iFlowAction),
		vFlowDsc(vFlowDsc),
		iTraceHand(iTraceHand),
		iSendAction(iSendAction),
		iTraceResult(iTraceResult),
		iSendResult(iSendResult),
		Undefined1(Undefined1)
	{}
}Item_Trace_List; //end Item_Trace_List

//add by asherzhou 2012-01-12-----------------------------start----------------------
/*
*	ApplyAnaylseStep 程序运行阶段标记，对应的数据库表中tbICSComplaint中的iICSAnaylseStep
*/
enum ApplyAnaylseStep
{
	STEP_WAITING_RUN_ICS = 0,		// 单据过滤系统前,未处理步骤
	STEP_FLITER_RUN_ICS = 1,		// 单据过滤系统运行阶段
	STEP_ROBSLOT_RUN_ICS = 2,		// 单据登入登出系统运行阶段
	STEP_ITEMFLOW_RUN_ICS = 3,		// 单据物品流水系统运行阶段
	STEP_ITEMTRACE_RUN_ICS = 4,		// 单据物品追踪系统运行阶段
	STEP_ITEMSEND_RUN_ICS = 5,		// 单据物品补送系统运行阶段
	STEP_PUNNISH_RUN_ICS = 6,		// 单据惩罚系统运行阶段
	STEP_OFFLINEMODULE_RUN_ICS = 7,	// 离线模型
	STEP_REPLAY_RUN_ICS = 10,		// 单据回复系统运行阶段
	//.......code add here...
};

/*
*	ApplyAnaylseResult 程序运行阶段标记，对应的数据库表中tbICSComplaint中的iICSAnaylseResult
*/
enum ApplyAnaylseResult
{
	STEP_WAITING_RUN_RESULT = 0,			// 单据过滤系统前,未处理步骤

	STEP_FLITER_RUNNING_RESULT = 11,		// 单据过滤系统运行中,处理中
	STEP_FLITER_RUN_RESULT_FALSE = 12,		// 单据过滤系统运行阶段错误
	STEP_FLITER_RUN_RESULT_TURE = 13,		// 单据过滤系统运行阶段正确

	STEP_ROBSLOT_RUNNING_RESULT = 20,		// 单据登入登出系统运行阶段运行
	STEP_ROBSLOT_RUN_RESULT_FALSE = 21,		// 单据登入登出系统运行阶段错误
	STEP_ROBSLOT_RUN_RESULT_TURE = 22,		// 单据登入登出系统运行阶段正确

	STEP_ITEMFLOW_RUNNING_RESULT = 30,		// 单据物品流水系统运行阶段运行中
	STEP_ITEMFLOW_RUN_RESULT_FALSE = 31,	// 单据物品流水系统运行阶段错误
	STEP_ITEMFLOW_RUN_RESULT_TURE = 32,		// 单据物品流水系统运行阶段正确

	STEP_ITEMFTRACE_RUNNING_RESULT = 40,	// 单据物品追踪系统运行阶段运行中
	STEP_ITEMFTRACE_RUN_RESULT_FALSE = 41,	// 单据物品追踪系统运行阶段错误
	STEP_ITEMFTRACE_RUN_RESULT_TURE = 42,	// 单据物品追踪系统运行阶段正确

	STEP_ITEMFSEND_RUNNING_RESULT = 50,		// 单据物品补送系统运行阶段运行中
	STEP_ITEMFSEND_RUN_RESULT_FALSE = 51,	// 单据物品补送系统运行阶段错误
	STEP_ITEMFSEND_RUN_RESULT_TURE = 52,	// 单据物品补送系统运行阶段正确
	//.......code add here...
};

/*
*	ApplyAnaylseStep 程序运行阶段标记，对应的数据库表中tbICSComplaint中的iICSAnaylseResult
*/
enum ApplyAnaylseStat
{
	STEP_WAITING_RUN_STAT = 0,							// 初始状态

	STEP_FLITER_RUNNING_STAT = 10,						// 单据过滤,处理中
	STEP_FLITER_RUN_STAT_SUCCESS = 11,					// 单据过滤,处理成功
	STEP_FLITER_RUN_STAT_ERROR_ROLE_INFO = 12,			// 单据过滤,角色信息查询错误
	STEP_FLITER_RUN_STAT_ERROR_LEVEL = 13,				// 单据过滤,等级不合法
	STEP_FLITER_RUN_STAT_ERROR_BANNED = 14,				// 单据过滤,玩家处于封号中
	STEP_FLITER_RUN_STAT_ERROR_APPLY_TIME = 15,			// 单据过滤,被盗时间超过申诉要求
	STEP_FLITER_RUN_STAT_ERROR_INPUT_TIME = 16,			// 单据过滤,发现被盗时间大于最后一次登陆时间
	STEP_FLITER_RUN_STAT_ERROR_APPLY_AGAIN = 17,		// 单据过滤,重复来单
	STEP_FLITER_RUN_STAT_ERROR_INTERFACE =18,			// 单据过滤,接口错误
	STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT = 19,			// 单据过滤,刷单

	STEP_ROBSLOT_RUNNING_STAT = 20,						// 登出分析,处理中
	STEP_ROBSLOT_RUN_STAT_SUCCESS = 21,					// 登出分析,处理成功
	STEP_ROBSLOT_RUN_STAT_ERROR_TIMESLOT = 22,			// 登出分析,开始时间大于结束时间
	STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE = 23,			// 登出分析,登录登出日志接口错误
	STEP_ROBSLOT_RUN_STAT_ERROR_COUNT_SMALL = 24,		// 登出分析,登录登出日志总条数过少
	STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_EXCEED = 25,		// 登出分析,太多地域登录无法分析
	STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_ANALYSIS = 26,	// 登出分析,IP转化地域无法分析
	STEP_ROBSLOT_RUN_STAT_ERROR_FIND_ROB_SLOT = 27,		// 登出分析,没有异常登录时间段
	
	STEP_ITEMFLOW_RUNNING_STAT = 30,					// 物品流水,处理中
	STEP_ITEMFLOW_RUN_STAT_SUCCESS = 31,				// 物品流水,处理成功
	STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW = 32,			// 物品流水,没有物品被盗
	
	STEP_ITEMTRACE_RUNNING_STAT = 40,					// 物品追踪,处理中
	STEP_ITEMTRACE_RUN_STAT_SUCCESS = 41,				// 物品追踪,处理成功
	STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ROUTE = 42,		// 物品追踪,无法追踪
	STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS = 43,			// 物品追踪,没有物品需要追踪
	
	STEP_ITEMSEND_RUNNING_STAT = 50,					// 物品补送,处理中
	STEP_ITEMSEND_RUN_STAT_SUCCESS = 51,				// 物品补送,处理成功
	STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST = 52,			// 物品补送,没有物品需要补送	
	
	STEP_ITEMSEND_RUN_STAT_SEND_AGAIN = 54,				// 补送模块重复操作了
	
	STEP_WAITING_RUN_LATER_ICS_STATE = 200,				// 此类单据后续处理
	//.......code add here...
};

enum ApplySlotStat{
	STEP_SlotStat_WAITING_STAT = 0,
	STEP_SlotStat_RUNNING_STAT = 1,
	STEP_SlotStat_RUN_OVER_STAT =2,

};

enum ApplyItemFlowStat{
	STEP_ItemFlow_WAITING_TRACING = 0,
	STEP_ItemFlow_RUNNING_TRACING = 1,
	STEP_ItemFlow_RUN_OVER_TRACING = 2,

};
enum ApplyItemFlowResult{
	STEP_ItemFlow_WAITING_TRACE_RESULT = 0,
	STEP_ItemFlow_NONEED_TRACE_RESULT = 1,
	STEP_ItemFlow_NORMAL_TRACE_RESULT = 2,

};
enum ApplyReplyResult{
	STEP_Reply_WAITING_RESULT = 0,			// 未回复
	STEP_Reply_WAITING_RESULT_SUCCESS = 1,	// 已经回复并成功
	STEP_Reply_WAITING_RESULT_ERROR = 2,	// 回复失败
};

/************************************V1.00******************************************/

//add by asherzhou 2012-01-12-----------------------------end----------------------

//宏定义，主要是处理标准日志的全局变量
struct ics_uin_info
{
	std::string ICSID;		// ICS单号
	std::string UserID;		// 用户帐号，QQ
	std::string WorldID;	// 大区
	std::string RoleID;		// 角色ID
	std::string VIPLevel;	// 用户VIP等级
	
	ics_uin_info():ICSID(""),
	UserID(""),
	WorldID(""),
	RoleID(""),
	VIPLevel(""){}

	ics_uin_info(const std::string& ICSID,
		const std::string& UserID, 
		const std::string& WorldID, 
		const std::string& RoleID,
		const std::string& VIPLevel)
		:ICSID(ICSID),
		UserID(UserID),
		WorldID(WorldID),
		RoleID(RoleID),
		VIPLevel(VIPLevel)
	{}
};


/**
@struct ics_info
@brief
@details
*/
struct ics_info
{
	std::string UserID;			// 用户帐号，QQ
	std::string WorldID;		// 大区
	std::string RoleID;			// 角色ID
	std::string RoleLevel;		// 角色等级
	std::string RoleName;		// 角色昵称
	std::string VIPLevel;		// 用户VIP等级

	std::string ICSID;			// ICS单号
	std::string ICSCreateTime;	// ICS填单时间
	std::string RobedTime;		// 发现被盗时间，作为查询login日志的开始时间
	std::string LastLoginTime;	// 最后一次登陆时间，作为查询login日志的结束时间
		
	std::string AnaylseRes;		// 单据分析结果
								// 0-初始状态，准备系统分析
								// 1-超出申诉15天
								// 2-60天内重复来单
								// 3-角色被封小于5天
								// 4-没有登陆日志
								// 5-最后登陆时间小于被盗时间
								// 7-角色被封大于5天
								// 8-等级小于25
								// 20-符合自助处理条件，进入自助处理流程

	std::string HandRes;		// 自助对单据处理结果
								// 0-初始态，待分析
								// 1-过滤状态分析不符合自助处理标准
								// 2-过滤状态分析符合自助处理标准
								// 3-日志分析完成
								// 3-赃物分析完成，已封盗号者
								// 4-已经补送给盗号者
								// 5-已经回复用户
	ics_info()
		:UserID(""),
		WorldID(""),
		RoleID(""),
		RoleLevel(""),
		RoleName(""),
		VIPLevel(""),
		ICSID(""),
		ICSCreateTime(""),
		RobedTime(""),
		LastLoginTime(""),
		AnaylseRes(""),
		HandRes("")
	{}

	ics_info(const std::string& UserID, 
		const std::string& WorldID, 
		const std::string& RoleID,
		const std::string& RoleLevel, 
		const std::string& RoleName,
		const std::string& VIPLevel, 
		const std::string& ICSID, 
		const std::string& ICSCreateTime,
		const std::string& RobedTime, 
		const std::string& LastLoginTime,
		const std::string& AnaylseRes,
		const std::string& HandRes)
		:UserID(UserID),
		WorldID(WorldID),
		RoleID(RoleID),
		RoleLevel(RoleLevel),
		RoleName(RoleName),
		VIPLevel(VIPLevel),
		ICSID(ICSID),
		ICSCreateTime(ICSCreateTime),
		RobedTime(RobedTime),
		LastLoginTime(LastLoginTime),
		AnaylseRes(AnaylseRes),
		HandRes(HandRes)
	{}

}; //end ics_info

/** 
@struct spoil_info
@brief	 
@details	 
*/
struct spoil_info
{
	std::string ICSID;
	std::string Rober;
	std::string RobRoleID;
	std::string WorldID;
	std::string EventTime;
	std::string ItemGuid;
	std::string ItemId;
	std::string ItemType;
	std::string ItemCou;
	std::string ItemAttr;
	std::string RobType;
	std::string Status;    //0、未处理，需要程序自动处理；1、叠加物品需要人工处理；
	std::string Result;
	std::string TracedTime;

	spoil_info()
		:ICSID(""),
		Rober(""),
		RobRoleID(""),
		EventTime(""),
		ItemGuid(""),
		ItemId(""),
		ItemType(""),
		ItemCou(""),
		ItemAttr(""),
		RobType(""),
		Status(""),
		Result(""),
		TracedTime("")
	{}

	spoil_info(const std::string& ICSID,
		const std::string& Rober,
		const std::string& RobRoleID,
		const std::string& WorldID,
		const std::string& EventTime,
		const std::string& ItemGuid,
		const std::string& ItemId,
		const std::string& ItemType,
		const std::string& ItemCou,
		const std::string& ItemAttr,
		const std::string& RobType,
		const std::string& Status,
		const std::string& Result,
		const std::string& TracedTime)
		:ICSID(ICSID),
		Rober(Rober),
		RobRoleID(RobRoleID),
		WorldID(WorldID),
		EventTime(EventTime),
		ItemGuid(ItemGuid),
		ItemId(ItemId),
		ItemType(ItemType),
		ItemCou(ItemCou),
		ItemAttr(ItemAttr),
		RobType(RobType),
		Status(Status),
		Result(Result),
		TracedTime(TracedTime)
	{}

}; //end spoil_info

typedef  	bool			MAUL_FLAG;		//< 是否手动标记

typedef struct ExistedItems {
	std::string ItemGuid;
};


#endif	//__MSG_H__
