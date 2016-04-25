/******************************************************************************
 * �ļ����ƣ� safe_protocol.h
 * �ļ������� ����safe��Ŀ��Э��ͷ���ڲ�����ģ���sys_id
 * �������ڣ� 2008-11-24
 * ��    �ߣ� mallory
 * Copyright 1998 - 2008 TENCENT Inc. All Rights Reserved
 * �޸���ʷ�� 
 ******************************************************************************/

#ifndef SAFE_PROTOCOL_JIANLING_ZONE_H
#define SAFE_PROTOCOL_JIANLING_ZONE_H

#include <stdint.h>
#include <netinet/in.h>

namespace SEC_JIANLING_ZONE
{

/* �������ĵĿ�ʼ, ������־ */
const char  SEC_SAFE_STX   =   0x02;
const char  SEC_SAFE_ETX   =   0x03;

/* ��������ͷǩ������ */
const uint32_t SEC_SAFE_ECHO_LEN = 64;

#pragma pack(1)

/* ����Э��ͷ�ṹ���� */
typedef struct
{
    uint16_t usLength;                  
    uint64_t ullUin;                     
    uint16_t usSysType;                 //ϵͳ����,�μ�emSafeSysID 
    uint16_t usMsgType;                 //��Ϣ����                 
    uint32_t ulAppID;                   //ҵ�����ID               
    uint32_t ulClientIP;                //�û�ip                   
    uint16_t usClientPort;              //�û�port                 
    uint32_t ulConnIP;                  //ҵ��server ip            
    uint16_t usConnPort;                //ҵ��server port          
    uint32_t ulSeq;                     //�ش�sequency id          
    uint32_t ulTraceID;                 //����ID                   
    uint32_t ulSendTime;                //����ʱ��                 
    uint16_t usLocaleID;                //֧�����Եط�             
    uint16_t usTimeZone;                //ʱ����Ԥ���ֶ�           
    uint16_t usVersion;                 //�汾                     
    uint8_t  ucEncrypt;                 //�����Ƿ����             
    uint8_t  ucReTry;                   //�ط�����                 
    char     acEcho[SEC_SAFE_ECHO_LEN]; //�����ֶ�, Ӧ�ò��ܸ���,���뱣֤һ�ֲ��ĵ�����Ӧ�лش���������                
    char     acMsgBody[0];              
} JianLingZoneMsgHead; 

#pragma pack()


inline uint64_t safe_ntohll(const uint64_t &ullIn)
{
    char*   pCur    = (char*)(&ullIn);
    if (1 != ntohs(1))  {
        //���x86
        uint64_t  ullOut  = (((uint64_t)(ntohl(*(uint32_t*)pCur))) 
                                       << (sizeof(uint32_t) * 8))
                                       + ntohl(*(uint32_t*)(pCur + sizeof(uint32_t)));
        return ullOut;
    }   else    {
        return ullIn;         
    }
}

#define safe_htonll(ullIn)  safe_ntohll(ullIn)


inline void Ntoh(JianLingZoneMsgHead* pMsg)
{
    pMsg->usLength     = ntohs(pMsg->usLength);     
    pMsg->ullUin       = safe_ntohll(pMsg->ullUin);        
    pMsg->usSysType    = ntohs(pMsg->usSysType);    
    pMsg->usMsgType    = ntohs(pMsg->usMsgType);    
    pMsg->ulAppID      = ntohl(pMsg->ulAppID);      
    pMsg->ulClientIP   = ntohl(pMsg->ulClientIP);   
    pMsg->usClientPort = ntohs(pMsg->usClientPort); 
    pMsg->ulConnIP     = ntohl(pMsg->ulConnIP);     
    pMsg->usConnPort   = ntohs(pMsg->usConnPort);   
    pMsg->ulSeq        = ntohl(pMsg->ulSeq);        
    pMsg->ulTraceID    = ntohl(pMsg->ulTraceID);    
    pMsg->ulSendTime   = ntohl(pMsg->ulSendTime);   
    pMsg->usLocaleID   = ntohs(pMsg->usLocaleID);   
    pMsg->usTimeZone   = ntohs(pMsg->usTimeZone);   
    pMsg->usVersion    = ntohs(pMsg->usVersion);    
}
inline void Hton(JianLingZoneMsgHead* pMsg)
{
    pMsg->usLength     = htons(pMsg->usLength);     
    pMsg->ullUin       = safe_htonll(pMsg->ullUin);        
    pMsg->usSysType    = htons(pMsg->usSysType);    
    pMsg->usMsgType    = htons(pMsg->usMsgType);    
    pMsg->ulAppID      = htonl(pMsg->ulAppID);      
    pMsg->ulClientIP   = htonl(pMsg->ulClientIP);   
    pMsg->usClientPort = htons(pMsg->usClientPort); 
    pMsg->ulConnIP     = htonl(pMsg->ulConnIP);     
    pMsg->usConnPort   = htons(pMsg->usConnPort);   
    pMsg->ulSeq        = htonl(pMsg->ulSeq);        
    pMsg->ulTraceID    = htonl(pMsg->ulTraceID); 
    pMsg->ulSendTime   = htonl(pMsg->ulSendTime);   
    pMsg->usLocaleID   = htons(pMsg->usLocaleID);   
    pMsg->usTimeZone   = htons(pMsg->usTimeZone);   
    pMsg->usVersion    = htons(pMsg->usVersion);    
}


};

#endif //~SAFE_PROTOCOL_H

