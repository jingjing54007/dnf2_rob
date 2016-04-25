/**
@file rob_core_anaylse.cpp

@brief 

	---------------------------------------------------------
	����������������(IP�쳣�����������������ˮ����)
	
	�����ĵ��ݱ����ǵ��ݺϷ��ģ��ǵ��Ŵ�������У�ɾ��������Ʒ��ǰ��
	��������������������� ICS����
	����������£�1) ��ȡҪ�����ĵ��ݣ�
	1���ȼ��Ƿ���� 2)ʱ������ж�
	---------------------------------------------------------

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		darrenqiu
@version	1.0
@date		2010/10/29

history:
<PRE>
History ID	:	1
Author    	:	asherzhou
DateTime	:	2011/05/01
Description :	Create
</PRE>
*/
#include <iostream>
#include "rob_anlys_ip.h"
#include "rob_ics_oper.h"
#include "rob_anlys_valid.h"
#include "rob_rule_logic.h"
#include "log.h"
#include "msg.h"
#include "glog.h"
#include "conf.h"
#include "comm_strings.h"
int main(int argc, char** argv)
{
	

	//������־
	CLog* ICSLogOutLOG;
	
	//����ģ����
	RobICSOper icsOper;
	RobAnlysIP icsRobAnlysIP;
	RobRuleLogic icsRobLogic;
	int iRet = 0;

	//������־�ĳ�ʼ��
	std::string sICSLogOutLOG="./logs/"+GetCurDateDay()+"RobLogOutFliter.log";
	ICSLogOutLOG = new CLog((const char*)sICSLogOutLOG.c_str());
	ICSLogOutLOG->LogOn();
	
	ICSLogOutLOG->WriteLog("Start RobLogOutFliter------------------------------------\n");
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Start RobLogOutFliter------------------------------------\n");

#ifdef dnf2
	ICSLogOutLOG->WriteLog("======The [dnf2] Prcosess======\n");
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "======The [dnf2] Prcosess======\n");

#endif

	int SIZE_OF_ICS = StrToInt(CRobConf::getConf()->GetIcsValid()["size_of_ics_two"]);
	ICSLogOutLOG->WriteLog("Read ICS LogOut Count is %d\n",SIZE_OF_ICS);
	WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "Read ICS LogOut Count is %d\n",SIZE_OF_ICS);
	
	/*****************************************************************************************
	@  A:�ڴ��ȡ��Rob_Ics_Complaint ����Ҫ��һ�����е���ʱ��β�ѯ��
	@  1.��ȡSIZE_OF_ICS�ĵ�����Ϣ������Rob_Ics_Complaint������
	*/
	Rob_Ics_Complaint Ics_Info[SIZE_OF_ICS];
	map<string,string> theParamIn;
	vector < map<string,string> > icsResult;

	int size_of_ics_info = icsOper.ReadICSInfoList(STEP_ROBSLOT_RUN_ICS,Ics_Info,SIZE_OF_ICS);
	if(size_of_ics_info >= 0 && size_of_ics_info <= SIZE_OF_ICS){
		ICSLogOutLOG->WriteLog("1> ��ȡ���ݽ���� ICS size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "1> ��ȡ���ݽ���� ICS size:%d and less than SIZE_OF_ICS Successfully\n",size_of_ics_info);

		vector< Rob_SLOT > robSlotList;
		for(int k=0;k<size_of_ics_info;k++){
			//2.���µ���״̬Ϊ������
			iRet = icsOper.UpdateICSProc(Ics_Info[k].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUNNING_RESULT,STEP_ROBSLOT_RUNNING_STAT);
			ICSLogOutLOG->WriteLog("2> ���µ��ݳɹ��� UpdateICSProc  LogOut iRet:%d\n",iRet);
			WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2> ���µ��ݳɹ��� UpdateICSProc  LogOut iRet:%d\n",iRet);
			if(iRet!=0){
				//������Ҫ�ʼ����������������汾����---��ͣ���ܡ�
				ICSLogOutLOG->WriteLog("2.1> ���µ���ʧ�ܣ� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1> ���µ���ʧ�ܣ� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

			}else{
				ICSLogOutLOG->WriteLog("2.1> ���µ��ݳɹ��� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "2.1> ���µ��ݳɹ��� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);

			}
		}
		for(int j=0;j<size_of_ics_info;j++){
			robSlotList.clear();
			//3.����������ݵ��쳣��¼ʱ���
			ICSLogOutLOG->WriteLog("3> ��ҵĵ�¼�ǳ���־��sICSId:��%s��\n",Ics_Info[j].sICSId.c_str());
			WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "3> ��ҵĵ�¼�ǳ���־��sICSId:��%s��\n",Ics_Info[j].sICSId.c_str());

			iRet = icsRobAnlysIP.getRobLogInOutLog(Ics_Info[j],robSlotList);
			if(iRet != 0){
				//4.1������Ҳ����ϵ��ŷ���ʱ��ε�����
				iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUN_RESULT_FALSE,(ApplyAnaylseStat)iRet);
				ICSLogOutLOG->WriteLog("4> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�Σ� UpdateICSProc  LogOut iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�Σ� UpdateICSProc  LogOut iRet:%d\n",iRet);

				if(iRet!=0){
					//������Ҫ�ʼ����������������汾����---��ͣ���ܡ�
					ICSLogOutLOG->WriteLog("4.1> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�νӿڷ��ء�ʧ�ܡ��� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.1> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�νӿڷ��ء�ʧ�ܡ��� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

				}else{
					//������Ҫ�ʼ����������������汾����---��ͣ���ܡ�
					ICSLogOutLOG->WriteLog("4.1> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�νӿڷ��ء��ɹ����� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.1> ���µ���״̬Ϊ��¼�ǳ����޷����������쳣����ʱ�νӿڷ��ء��ɹ����� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
				}
				continue;
			}else{
				//4.2������ҷ��ϵ��ŷ���ʱ��ε����ݣ�ͬʱ�����ݲ���tbICSRoleLogout�쳣��¼����
				iRet = icsOper.UpdateICSProc(Ics_Info[j].sID,STEP_ROBSLOT_RUN_ICS,STEP_ROBSLOT_RUN_RESULT_TURE,STEP_ROBSLOT_RUN_STAT_SUCCESS);
				ICSLogOutLOG->WriteLog("4> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�Σ� UpdateICSProc  LogOut iRet:%d\n",iRet);
				WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�Σ� UpdateICSProc  LogOut iRet:%d\n",iRet);

				if(iRet!=0){
					//������Ҫ�ʼ����������������汾����---��ͣ���ܡ�
					ICSLogOutLOG->WriteLog("4.2> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�νӿڷ��ء�ʧ�ܡ��� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);
				   WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE ,Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.2> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�νӿڷ��ء�ʧ�ܡ��� UpdateICSProc LogOut Happend Error-iRet:%d\n",iRet);

				}else{
					//������Ҫ�ʼ����������������汾����---��ͣ���ܡ�
					ICSLogOutLOG->WriteLog("4.2> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�νӿڷ��ء��ɹ����� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "4.2> ���µ���״̬Ϊ��¼�ǳ����ѷ��������쳣����ʱ�νӿڷ��ء��ɹ����� UpdateICSProc LogOut Happend Successfully-iRet:%d\n",iRet);

					//5.������ҵ��쳣��¼����
					ICSLogOutLOG->WriteLog("5> ���뵥�ݵ�¼�ǳ��쳣����ʱ��\n");
					WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , Ics_Info[j].sICSId.c_str(), Ics_Info[j].sUin.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5> ���뵥�ݵ�¼�ǳ��쳣����ʱ��\n");

					for(int k=0 ;k<robSlotList.size();k++){
						if(robSlotList[k].iRobFlag=="1"){
							sleep(1);
							//5.1��ȡ����ڵ���ʱ����ڵĶ�ʧ��Ǯ����������iMoney_Sub��
							iRet = icsRobLogic.getRobSlotMoneySub(robSlotList[k]);
							if(iRet!=0){
								ICSLogOutLOG->WriteLog("5.1>��ȡ��%d����ҡ�%s����Ǯ��������ѯ��ʧ�ܡ�iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.1>��ȡ��%d����ҡ�%s����Ǯ��������ѯ��ʧ�ܡ�iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);

							}else{
								ICSLogOutLOG->WriteLog("5.1>��ȡ��%d����ҡ�%s����Ǯ��������ѯ���ɹ���iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.1>��ȡ��%d����ҡ�%s����Ǯ��������ѯ���ɹ���iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);

								iRet = icsOper.InsertICSRobSlotData(robSlotList[k]);
								if(iRet!=0){
									ICSLogOutLOG->WriteLog("5.2> ���뵥�ݡ�%d�����ݺš�%s����¼�ǳ��쳣����ʱ������ʧ�ܡ�iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
									WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.2> ���뵥�ݡ�%d�����ݺš�%s����¼�ǳ��쳣����ʱ������ʧ�ܡ�iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								}else{
									ICSLogOutLOG->WriteLog("5.2> ���뵥�ݡ�%d�����ݺš�%s����¼�ǳ��쳣����ʱ�������ɹ���iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
									WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , robSlotList[k].ICSID.c_str(), robSlotList[k].UserID.c_str(), TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "5.2> ���뵥�ݡ�%d�����ݺš�%s����¼�ǳ��쳣����ʱ�������ɹ���iRet��%d\n",k,robSlotList[k].ICSID.c_str(),iRet);
								}
							}
						}else{
							continue;
						}
					}
				}
				
			}
		}
	}else{
		ICSLogOutLOG->WriteLog("--->>>��ȡ������ReadICSInfoList size:��%d��������û�е��ݴ�����߼��ӿڴ���\n",size_of_ics_info);
		WriteLogsRUN(TLOG_TYPE_ROBLOGOUTANALYSE , "", "", TLOG_SERVICE_ID,TLOG_LEVEL_INFO ,__FUNCTION__, "--->>>��ȡ������ReadICSInfoList size:��%d��������û�е��ݴ�����߼��ӿڴ���\n",size_of_ics_info);

		return 0;
	}


	return 0;
}
