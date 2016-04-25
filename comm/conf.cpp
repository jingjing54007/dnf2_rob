/**
@file conf.cpp

@brief 

@details
Copyright (c) 2010 Tencent. All rights reserved.

@author		GunLi
@version	1.0
@date		2010/10/09

history:
<PRE>
History ID	:	1
Author    	:	GunLi
DateTime	:	2010/10/09
Description :	Create
</PRE>

Alert by asherzhou
2011/04/19
*/

#include "conf.h"
#include "msg.h"
#include "comm.h"
#include "comm_datetime.h"
//��������Include��Ҫ�����ϵͳ��
#include "NDS_config.h"
#include "comm_strings.h"


CRobConf* CRobConf::RobConf = NULL;
CLog* CRobConf::ConfLOG=NULL;
/**
Description:	��ȡһ��ConfNode����
@return			
@exception		none
*/
CRobConf* CRobConf::getConf()
{
	if (RobConf == NULL)
	{
		RobConf = new CRobConf();
	}
	return RobConf;
}

/**
Description:	���캯������ȡ���е�������Ϣ
@return			
@exception		none
@alert  asherzhou �޸�һЩ��ȡ������Ϣ������dr,log���ÿ�����ȫȥ���ˡ��Ż�rob.conf�ṹ
@							modified by zhtang, xml config
*/
CRobConf::CRobConf()
{
	MAKECONFLOG="./logs/"+GetCurDateDay()+"RobConf.log";
	ConfLOG = new CLog((const char*)MAKECONFLOG.c_str());
	ConfLOG->WriteLog("<<<Again we new a Clog???>>>\n");
	WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID, TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "<<<Again we new a Clog???>>>\n" );

	ConfLOG->LogOn();
	ConfLOG->WriteLog("<<<Get rob.conf Begin>>>\n");
	WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "<<<Get rob.conf Begin>>>\n" );

	//���������Ϣ���л��棬���֮�����¶�ȡ����
	if(Ics_Oper.size()>0) Ics_Oper.clear();
	if(Ics_Valid.size()>0) Ics_Valid.clear();
	if(GroupInfo.size()>0) GroupInfo.clear();
	if(Log_Info.size()>0) Log_Info.clear();
	if(UserList.size()>0) UserList.clear();
	if(Common_Interf.size()>0) Common_Interf.clear();

	CONFPATH = "./cfg/rob.xml";
	xmlPtr = xmlReadFile(CONFPATH.c_str(),"UTF-8",XML_PARSE_NOBLANKS);
	xmlKeepBlanksDefault(1);
	ParseXmlConfig(xmlPtr);
}

CRobConf::~CRobConf()
{
  if(NULL != ConfLOG)
  	{
  		delete ConfLOG;
		ConfLOG = NULL;
  	}
  if(NULL != xmlPtr)
  {
	  xmlFreeDoc(xmlPtr);
	  xmlPtr = NULL;//��ֹ�����ͷ�
  }
}
int CRobConf::ParseXmlConfig(xmlDocPtr xmlPtr)
{
	xmlNodePtr ptrNode;
	xmlNodePtr ptrRoot;
	xmlNodePtr ptrCur;

	
	if(NULL != xmlPtr)
	{
		ConfLOG->WriteLog("[INFO]>>load xml success\n");
		WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "[INFO]>>load xml success\n" );
		// locate the root node:<rob>
		ptrRoot = xmlDocGetRootElement(xmlPtr);
		if (!xmlStrcmp(ptrRoot->name,BAD_CAST"rob"))
		{
			// get all the children
			ptrCur = ptrRoot->xmlChildrenNode;
			//0 parse [common] node
			ParseComponentByNodeName(ptrCur,BAD_CAST"common",this->GroupInfo);
			
			//1 parse [userlist] node
			ptrCur = ptrCur->next;
			ParseComponentByNodeName(ptrCur,BAD_CAST"userlist",this->UserList);
			
			//1 parse [log] node
			ptrCur = ptrCur->next;
			ParseComponentByNodeName(ptrCur,BAD_CAST"log",this->Log_Info);

			// enter [interface] node,and locate the first node[common_interface]
			ptrCur = ptrCur->next;
			ptrCur = ptrCur->xmlChildrenNode;

			//2 parse [common_interface] node
			ParseComponentByNodeName(ptrCur,BAD_CAST"common_interface",this->Common_Interf);

			//3 parse [ics_valid] node
			ptrCur = ptrCur->next;
			ParseComponentByNodeName(ptrCur,BAD_CAST"ics_valid",this->Ics_Valid);

			//4 parse [authority] node
			ptrCur = ptrCur->next;
			ParseComponentByNodeName(ptrCur,BAD_CAST"authority",this->Authority);

			//5 parse [ics_oper] node
			ptrCur = ptrCur->next;
			ParseComponentByNodeName(ptrCur,BAD_CAST"ics_oper",this->Ics_Oper);

			ConfLOG->WriteLog("[INFO]>>parse xml success,END!\n");
			WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "[INFO]>>parse xml success,END!\n" );
		}else
		{
			ConfLOG->WriteLog("[ERROR]>>parse error,root name is wrong! Please check it!\n");
			WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_ERROR, __FUNCTION__, "[ERROR]>>parse error,root name is wrong! Please check it!\n" );
		}
	}else
	{
		ConfLOG->WriteLog("[ERROR]>>Load %s fail! Please check it!\n",CONFPATH.c_str());
		WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_ERROR, __FUNCTION__, "[ERROR]>>Load %s fail! Please check it!\n",CONFPATH.c_str() );
	}
	return 0;
}
/*******************************************************************
Description:	�������µĽṹ��xml���νṹ
				<common>
					<add key="group_id" value="225" desc="ҵ��ID"/>
					<add key="group_name" value="c9" desc=""/>
				</common>

Params:			ptrCur :����xml���νṹ�ĸ��ڵ�
				inputName: ����ڵ������
				theResult:	�������и��ڵ��µ������ӽڵ�key->value���ϣ����
				
return:			��ȷ���
@exception		none
*/
int CRobConf::ParseComponentByNodeName(xmlNodePtr ptrCur,xmlChar* inputName,map< string, string >& theResult)
{
	ConfLOG->WriteLog("--------[INFO]>> begin to parse node[%s]---------\n",(char*)inputName);
	WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "--------[INFO]>> begin to parse node[%s]---------\n",(char*)inputName );

	xmlNodePtr ptrNode;
	int i = 0;

	if (!xmlStrcmp(ptrCur->name,inputName))
	{
		ptrNode = ptrCur->xmlChildrenNode;//��ʼadd�ڵ����
		if (!xmlStrcmp(ptrNode->name,BAD_CAST"add"))
		{
			xmlAttrPtr ptrAttr;
			while (ptrNode)
			{
				ptrAttr = ptrNode->properties;
				string strKey;
				string strValue;
				while(ptrAttr)
				{
					if (!xmlStrcmp(ptrAttr->name,BAD_CAST"key"))
					{
						xmlChar* ucKey = xmlGetProp(ptrNode,BAD_CAST"key");
						strKey = string((char*)ucKey);
// 						if (strKey=="group_id")
// 						{
// 							printf("strKey=%s\n",strKey.c_str());
// 						}
						xmlFree(ucKey);
						ucKey = NULL;
					}else if (!xmlStrcmp(ptrAttr->name,BAD_CAST"value"))
					{
						xmlChar* ucValue = xmlGetProp(ptrNode,BAD_CAST"value");
						strValue = string((char*)ucValue);
// 						if (strKey=="group_id")
// 						{
// 							printf("values=%s\n",strValue.c_str());
// 						}
						xmlFree(ucValue);
						ucValue = NULL;
					}
					ptrAttr = ptrAttr->next;				
				}
				ConfLOG->WriteLog("[%d][key: %s=%s]\n",i,strKey.c_str(),strValue.c_str());
				WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "[%d][key: %s=%s]\n", i, strKey.c_str(), strValue.c_str() );
				theResult[strKey] = strValue;
				ptrNode = ptrNode->next;
				i++;
			}
		}else
		{
			ConfLOG->WriteLog("[ERROR]:Parse error,node name[%s] is wrong,please check it!\n",(char*)ptrNode->name);
			WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_ERROR, __FUNCTION__, "[ERROR]:Parse error,node name[%s] is wrong,please check it!\n", (char*)ptrNode->name );
			return -1;
		}
	}else
	{
		ConfLOG->WriteLog("[ERROR]:Parse error,node name[%s] is wrong,please check it!\n",(char*)ptrCur->name);
		WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_ERROR, __FUNCTION__, "[ERROR]:Parse error,node name[%s] is wrong,please check it!\n", (char*)ptrCur->name );
		return -1;
	}
	ConfLOG->WriteLog("[PARSE RESULT]>> Node[%s]Children=[%d] \n",(char*)inputName,theResult.size());
	WriteLogsRUN(TLOG_TYPE_ROBCONF, "", "", TLOG_SERVICE_ID,  TLOG_LEVEL_CONF + TLOG_LEVEL_INFO, __FUNCTION__, "[PARSE RESULT]>> Node[%s]Children=[%d] \n", (char*)inputName, theResult.size() );

	return 0;
}
std::map< std::string, std::string > CRobConf::GetCommonInterface() const
{
	return this->Common_Interf;
}

std::map< std::string, std::string > CRobConf::GetIcsValid() const
{
	return this->Ics_Valid;
}

std::map< std::string, std::string > CRobConf::GetGroupInfo() const
{
	return this->GroupInfo;
}

std::map< std::string, std::string > CRobConf::GetUserList() const
{
	return this->UserList;
}

std::map< std::string, std::string > CRobConf::GetLogInfo() const
{
	return this->Log_Info;
}

std::map< std::string, std::string > CRobConf::GetAuthority() const
{
	return this->Authority;
}

std::map< std::string, std::string > CRobConf::GetIcsOper() const
{
	return this->Ics_Oper;
}