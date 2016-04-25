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


//�궨�壬��Ҫ�Ǵ����׼��־����־ͷ���� 2-4��
//|ʱ��|����|QQ|ҵ��ID|�ӿ�ID|��������б�IDIP��ʽ��|
struct ics_log_header
{
	std::string ICSID;						//ICS����
	std::string UserID;						//�û��ʺţ�QQ
	std::string ServiceID;					//����

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
@struct Rob_Ics_Complaint,���ڶ�Ӧԭʼ����ݱ�
@brief	 
@details ���ڰ������������ݿ���Ϣ���ڴ洦��
alert by heyuefeng 2012/01/13
*/
struct Rob_Ics_Complaint
{
	std::string sID;				// �������ID
	std::string sServiceID;			// ҵ��ID
	std::string sICSId;				// ICS����
	std::string sUin;				// �û�ID
	std::string sRoleID;			// ��ɫID
	std::string sRoleName;			// ��ɫ��
	std::string sRoleLevel;			// ��ɫ�ȼ�
	std::string sWordId;			// ����ID
	std::string stBookTime;			// ICS�ʱ��
	std::string stLastLogin;		// �������¼ʱ��
	std::string stRobeLogin;		// ��ҷ��ֱ���ʱ��
	std::string sUpdateTime;		// ���ݸ��´���ʱ��
	std::string sContent; 			// ICS�����
	std::string sSolution; 			// ICS�������
	std::string sIsVip;				// �û�VIP�ȼ�
	std::string sICSAnaylseStep;	// �����Ե��ݷ�������
	std::string sICSAnaylseResult;	// �����Ե��ݷ������
	std::string sICSAnaylseStat;	// �����Ե��ݹ��˷��������������Ҫ��Ե��ǹ��˽���ķ���
	std::string siRobedRoleBan;		// �Ƿ���
	std::string siReplyResult;		// �Ƿ�ظ�������
	std::string siSource;			// ��Դ
	std::string sUndefined1;		// �����ֶ�1
	std::string sUndefined2;		// �����ֶ�2
	std::string sUndefined3;		// �����ֶ�3

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
@details ��Ҫ�Ƕ�λ��׼��־��ȫ�ֱ���
*/
typedef struct Rob_Login_Logout_SLOT
{
	std::string ICSID;			// ICS����
	std::string ServiceID;		// ��Ϸҵ��ID
	std::string UserID;			// �û��ʺţ�QQ
	std::string WorldID;		// ����
	std::string RoleID;			// ��ɫID
	std::string RoleName;		// ��ɫ����
	std::string	sBlackRobIp;	// ���ŵ�IP
	std::string sRobIPName;		// �����ߵĵ���
	std::string	LoginTime;		// ��¼ʱ��
	std::string	LogoutTime;		// �ǳ�ʱ��
	std::string	OnLineTime;		// ����ʱ��
	std::string iMoney_Sub;		// ��Ǯ���ٸ���
	std::string	iRobFlag;		// �Ƿ��ǵ�����־

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
asher�����������Ʒ���ٵı�
yangyihe(������) 2012-02-24 14:22:58
if(vLostData[j]["iReason"] == "0" ||vLostData[j]["iReason"] == "5" ||vLostData[j]["iReason"] == "9")
                {
                    Replace(vLostData[j]["vGuid"],"\n","");
                    Replace(vLostData[j]["vGuid"],"\r","");
                    Replace(vLostData[j]["vGuid"],"\t","");

                    vDataResult.push_back(vLostD
yangyihe(������) 2012-02-24 14:22:58
ata[j]);
                }
yangyihe(������) 2012-02-24 14:23:17
0 5 9������ʧ�ģ�ֱ�Ӳ���

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
	std::string iItem_Unique_Id;    //0��δ������Ҫ�����Զ�����1��������Ʒ��Ҫ�˹�����
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
	std::string iItem_Unique_Id;	// 0��δ������Ҫ�����Զ�����1��������Ʒ��Ҫ�˹�����
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
*	ApplyAnaylseStep �������н׶α�ǣ���Ӧ�����ݿ����tbICSComplaint�е�iICSAnaylseStep
*/
enum ApplyAnaylseStep
{
	STEP_WAITING_RUN_ICS = 0,		// ���ݹ���ϵͳǰ,δ������
	STEP_FLITER_RUN_ICS = 1,		// ���ݹ���ϵͳ���н׶�
	STEP_ROBSLOT_RUN_ICS = 2,		// ���ݵ���ǳ�ϵͳ���н׶�
	STEP_ITEMFLOW_RUN_ICS = 3,		// ������Ʒ��ˮϵͳ���н׶�
	STEP_ITEMTRACE_RUN_ICS = 4,		// ������Ʒ׷��ϵͳ���н׶�
	STEP_ITEMSEND_RUN_ICS = 5,		// ������Ʒ����ϵͳ���н׶�
	STEP_PUNNISH_RUN_ICS = 6,		// ���ݳͷ�ϵͳ���н׶�
	STEP_OFFLINEMODULE_RUN_ICS = 7,	// ����ģ��
	STEP_REPLAY_RUN_ICS = 10,		// ���ݻظ�ϵͳ���н׶�
	//.......code add here...
};

/*
*	ApplyAnaylseResult �������н׶α�ǣ���Ӧ�����ݿ����tbICSComplaint�е�iICSAnaylseResult
*/
enum ApplyAnaylseResult
{
	STEP_WAITING_RUN_RESULT = 0,			// ���ݹ���ϵͳǰ,δ������

	STEP_FLITER_RUNNING_RESULT = 11,		// ���ݹ���ϵͳ������,������
	STEP_FLITER_RUN_RESULT_FALSE = 12,		// ���ݹ���ϵͳ���н׶δ���
	STEP_FLITER_RUN_RESULT_TURE = 13,		// ���ݹ���ϵͳ���н׶���ȷ

	STEP_ROBSLOT_RUNNING_RESULT = 20,		// ���ݵ���ǳ�ϵͳ���н׶�����
	STEP_ROBSLOT_RUN_RESULT_FALSE = 21,		// ���ݵ���ǳ�ϵͳ���н׶δ���
	STEP_ROBSLOT_RUN_RESULT_TURE = 22,		// ���ݵ���ǳ�ϵͳ���н׶���ȷ

	STEP_ITEMFLOW_RUNNING_RESULT = 30,		// ������Ʒ��ˮϵͳ���н׶�������
	STEP_ITEMFLOW_RUN_RESULT_FALSE = 31,	// ������Ʒ��ˮϵͳ���н׶δ���
	STEP_ITEMFLOW_RUN_RESULT_TURE = 32,		// ������Ʒ��ˮϵͳ���н׶���ȷ

	STEP_ITEMFTRACE_RUNNING_RESULT = 40,	// ������Ʒ׷��ϵͳ���н׶�������
	STEP_ITEMFTRACE_RUN_RESULT_FALSE = 41,	// ������Ʒ׷��ϵͳ���н׶δ���
	STEP_ITEMFTRACE_RUN_RESULT_TURE = 42,	// ������Ʒ׷��ϵͳ���н׶���ȷ

	STEP_ITEMFSEND_RUNNING_RESULT = 50,		// ������Ʒ����ϵͳ���н׶�������
	STEP_ITEMFSEND_RUN_RESULT_FALSE = 51,	// ������Ʒ����ϵͳ���н׶δ���
	STEP_ITEMFSEND_RUN_RESULT_TURE = 52,	// ������Ʒ����ϵͳ���н׶���ȷ
	//.......code add here...
};

/*
*	ApplyAnaylseStep �������н׶α�ǣ���Ӧ�����ݿ����tbICSComplaint�е�iICSAnaylseResult
*/
enum ApplyAnaylseStat
{
	STEP_WAITING_RUN_STAT = 0,							// ��ʼ״̬

	STEP_FLITER_RUNNING_STAT = 10,						// ���ݹ���,������
	STEP_FLITER_RUN_STAT_SUCCESS = 11,					// ���ݹ���,����ɹ�
	STEP_FLITER_RUN_STAT_ERROR_ROLE_INFO = 12,			// ���ݹ���,��ɫ��Ϣ��ѯ����
	STEP_FLITER_RUN_STAT_ERROR_LEVEL = 13,				// ���ݹ���,�ȼ����Ϸ�
	STEP_FLITER_RUN_STAT_ERROR_BANNED = 14,				// ���ݹ���,��Ҵ��ڷ����
	STEP_FLITER_RUN_STAT_ERROR_APPLY_TIME = 15,			// ���ݹ���,����ʱ�䳬������Ҫ��
	STEP_FLITER_RUN_STAT_ERROR_INPUT_TIME = 16,			// ���ݹ���,���ֱ���ʱ��������һ�ε�½ʱ��
	STEP_FLITER_RUN_STAT_ERROR_APPLY_AGAIN = 17,		// ���ݹ���,�ظ�����
	STEP_FLITER_RUN_STAT_ERROR_INTERFACE =18,			// ���ݹ���,�ӿڴ���
	STEP_FLITER_RUN_STAT_ERROR_APPLY_LOT = 19,			// ���ݹ���,ˢ��

	STEP_ROBSLOT_RUNNING_STAT = 20,						// �ǳ�����,������
	STEP_ROBSLOT_RUN_STAT_SUCCESS = 21,					// �ǳ�����,����ɹ�
	STEP_ROBSLOT_RUN_STAT_ERROR_TIMESLOT = 22,			// �ǳ�����,��ʼʱ����ڽ���ʱ��
	STEP_ROBSLOT_RUN_STAT_ERROR_INTERFACE = 23,			// �ǳ�����,��¼�ǳ���־�ӿڴ���
	STEP_ROBSLOT_RUN_STAT_ERROR_COUNT_SMALL = 24,		// �ǳ�����,��¼�ǳ���־����������
	STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_EXCEED = 25,		// �ǳ�����,̫������¼�޷�����
	STEP_ROBSLOT_RUN_STAT_ERROR_AREAS_ANALYSIS = 26,	// �ǳ�����,IPת�������޷�����
	STEP_ROBSLOT_RUN_STAT_ERROR_FIND_ROB_SLOT = 27,		// �ǳ�����,û���쳣��¼ʱ���
	
	STEP_ITEMFLOW_RUNNING_STAT = 30,					// ��Ʒ��ˮ,������
	STEP_ITEMFLOW_RUN_STAT_SUCCESS = 31,				// ��Ʒ��ˮ,����ɹ�
	STEP_ITEMFLOW_RUN_STAT_ERROR_NO_FLOW = 32,			// ��Ʒ��ˮ,û����Ʒ����
	
	STEP_ITEMTRACE_RUNNING_STAT = 40,					// ��Ʒ׷��,������
	STEP_ITEMTRACE_RUN_STAT_SUCCESS = 41,				// ��Ʒ׷��,����ɹ�
	STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ROUTE = 42,		// ��Ʒ׷��,�޷�׷��
	STEP_ITEMTRACE_RUN_STAT_ERROR_NO_ICS = 43,			// ��Ʒ׷��,û����Ʒ��Ҫ׷��
	
	STEP_ITEMSEND_RUNNING_STAT = 50,					// ��Ʒ����,������
	STEP_ITEMSEND_RUN_STAT_SUCCESS = 51,				// ��Ʒ����,����ɹ�
	STEP_ITEMSEND_RUN_STAT_ERROR_NO_LIST = 52,			// ��Ʒ����,û����Ʒ��Ҫ����	
	
	STEP_ITEMSEND_RUN_STAT_SEND_AGAIN = 54,				// ����ģ���ظ�������
	
	STEP_WAITING_RUN_LATER_ICS_STATE = 200,				// ���൥�ݺ�������
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
	STEP_Reply_WAITING_RESULT = 0,			// δ�ظ�
	STEP_Reply_WAITING_RESULT_SUCCESS = 1,	// �Ѿ��ظ����ɹ�
	STEP_Reply_WAITING_RESULT_ERROR = 2,	// �ظ�ʧ��
};

/************************************V1.00******************************************/

//add by asherzhou 2012-01-12-----------------------------end----------------------

//�궨�壬��Ҫ�Ǵ����׼��־��ȫ�ֱ���
struct ics_uin_info
{
	std::string ICSID;		// ICS����
	std::string UserID;		// �û��ʺţ�QQ
	std::string WorldID;	// ����
	std::string RoleID;		// ��ɫID
	std::string VIPLevel;	// �û�VIP�ȼ�
	
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
	std::string UserID;			// �û��ʺţ�QQ
	std::string WorldID;		// ����
	std::string RoleID;			// ��ɫID
	std::string RoleLevel;		// ��ɫ�ȼ�
	std::string RoleName;		// ��ɫ�ǳ�
	std::string VIPLevel;		// �û�VIP�ȼ�

	std::string ICSID;			// ICS����
	std::string ICSCreateTime;	// ICS�ʱ��
	std::string RobedTime;		// ���ֱ���ʱ�䣬��Ϊ��ѯlogin��־�Ŀ�ʼʱ��
	std::string LastLoginTime;	// ���һ�ε�½ʱ�䣬��Ϊ��ѯlogin��־�Ľ���ʱ��
		
	std::string AnaylseRes;		// ���ݷ������
								// 0-��ʼ״̬��׼��ϵͳ����
								// 1-��������15��
								// 2-60�����ظ�����
								// 3-��ɫ����С��5��
								// 4-û�е�½��־
								// 5-����½ʱ��С�ڱ���ʱ��
								// 7-��ɫ�������5��
								// 8-�ȼ�С��25
								// 20-����������������������������������

	std::string HandRes;		// �����Ե��ݴ�����
								// 0-��ʼ̬��������
								// 1-����״̬�������������������׼
								// 2-����״̬�����������������׼
								// 3-��־�������
								// 3-���������ɣ��ѷ������
								// 4-�Ѿ����͸�������
								// 5-�Ѿ��ظ��û�
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
	std::string Status;    //0��δ������Ҫ�����Զ�����1��������Ʒ��Ҫ�˹�����
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

typedef  	bool			MAUL_FLAG;		//< �Ƿ��ֶ����

typedef struct ExistedItems {
	std::string ItemGuid;
};


#endif	//__MSG_H__
