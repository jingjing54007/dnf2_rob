/******************************************************************************
 * 文件名称： safe_protocol.h
 * 文件描述： 描述safe项目的协议头和内部功能模块的sys_id
 * 创建日期： 2008-11-24
 * 作    者： mallory
 * Copyright 1998 - 2008 TENCENT Inc. All Rights Reserved
 * 修改历史： 
 ******************************************************************************/

#ifndef SAFE_PROTOCOL_JIANLING_ZONE_H
#define SAFE_PROTOCOL_JIANLING_ZONE_H

#include <stdint.h>
#include <netinet/in.h>

namespace SEC_JIANLING_ZONE
{

/* 公共报文的开始, 结束标志 */
const char  SEC_SAFE_STX   =   0x02;
const char  SEC_SAFE_ETX   =   0x03;

/* 公共报文头签名长度 */
const uint32_t SEC_SAFE_ECHO_LEN = 64;

#pragma pack(1)

/* 公共协议头结构定义 */
typedef struct
{
    uint16_t usLength;                  
    uint64_t ullUin;                     
    uint16_t usSysType;                 //系统类型,参见emSafeSysID 
    uint16_t usMsgType;                 //消息类型                 
    uint32_t ulAppID;                   //业务分配ID               
    uint32_t ulClientIP;                //用户ip                   
    uint16_t usClientPort;              //用户port                 
    uint32_t ulConnIP;                  //业务server ip            
    uint16_t usConnPort;                //业务server port          
    uint32_t ulSeq;                     //回带sequency id          
    uint32_t ulTraceID;                 //流程ID                   
    uint32_t ulSendTime;                //发送时间                 
    uint16_t usLocaleID;                //支持语言地方             
    uint16_t usTimeZone;                //时区，预留字段           
    uint16_t usVersion;                 //版本                     
    uint8_t  ucEncrypt;                 //报文是否加密             
    uint8_t  ucReTry;                   //重发报文                 
    char     acEcho[SEC_SAFE_ECHO_LEN]; //回送字段, 应用不能更改,必须保证一字不改地在响应中回带给调用者                
    char     acMsgBody[0];              
} JianLingZoneMsgHead; 

#pragma pack()


inline uint64_t safe_ntohll(const uint64_t &ullIn)
{
    char*   pCur    = (char*)(&ullIn);
    if (1 != ntohs(1))  {
        //针对x86
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

