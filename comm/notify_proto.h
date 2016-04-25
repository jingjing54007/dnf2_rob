#ifndef __NotifyProto_H__
#define __NotifyProto_H__

#include "sec_jianling_zone_protocol.h"
#include "safe_protocol_jianling_zone.h"

#include <iostream>
using namespace std;

namespace SEC_JIANLING_ZONE
{

// 申诉结果通知响应包
typedef struct AppealNotifyRspPkg_t
{
    char             cStx;
    JianLingZoneMsgHead   stHead;
    AppealNotifyRsp    stBody; 
    char             cEtx;
}*PAppealNotifyRspPkg;

// 申诉结果通知请求包
typedef struct AppealNotifyReqPkg_t
{
    char             cStx;
    JianLingZoneMsgHead   stHead;
    AppealNotifyReq    stBody; 
    char             cEtx;
} *PAppealNotifyReqPkg;

#pragma pack()


template<class Type>
static int PduEncode(char* szBuz, int& iBuzLen, const Type& stMsg)
{
    const uint32_t iHeadLen = sizeof(stMsg.stHead);
	cout<<"iBuzLen:"<<iBuzLen<<" iHeadLen:"<<iHeadLen<<endl;
    // 设置消息头
    szBuz[0] = SEC_SAFE_STX;
    memcpy(&szBuz[1], &stMsg.stHead, iHeadLen);

    // 设置 pdu 消息体
    int iBodyLen = iBuzLen - iHeadLen - 1;
    uint8_t *pPduBuz = (uint8_t*)&(szBuz[iHeadLen + 1]);
	
	cout<<"iBuzLen:"<<iBuzLen<<" iBodyLen:"<<iBodyLen<<endl;
	
    int iRet = 0;
    if( 0 != (iRet = pdusec_pack(&stMsg.stBody, pPduBuz, &iBodyLen)) )
    {
    	cout<<"pdusec_pack iRet:"<<iRet<<endl;
		cout.flush();
        return iRet;
    }

    // 设置消息尾
    szBuz[ iHeadLen + 1 + iBodyLen ] = SEC_SAFE_ETX;

    // 发送 buffer 的长度
    iBuzLen = iHeadLen + 2 + iBodyLen;

    JianLingZoneMsgHead *pMsgHead = (JianLingZoneMsgHead *)&szBuz[1];
    pMsgHead->usLength = iBuzLen;
    Hton( pMsgHead );

	cout<<"iBuzLen:"<<iBuzLen<<" iBodyLen:"<<iBodyLen<<endl;
	cout.flush();
    return 0;
};

template<class Type>
static int PduDecode(Type &stMsg, const char szBuz[], int iBuzLen)
{
    if( SEC_SAFE_STX != szBuz[0] || SEC_SAFE_ETX != szBuz[iBuzLen - 1] )
    {
        return -1;
    }

    stMsg.cStx = SEC_SAFE_STX;
    stMsg.cEtx = SEC_SAFE_ETX;

    // 解析包头
    JianLingZoneMsgHead* pMsgHead = (JianLingZoneMsgHead*)&szBuz[1];
    memcpy(&stMsg.stHead,pMsgHead, sizeof(stMsg.stHead));
    Ntoh(&stMsg.stHead);

    if( iBuzLen != stMsg.stHead.usLength )
    {
        return -2;
    }

    // 获取消息体内容
    uint8_t *pPduBody = (uint8_t*)pMsgHead->acMsgBody;
    int iBodyLen = stMsg.stHead.usLength - sizeof(stMsg.stHead) - 2;

    int iRet = 0;
    if(0 != (iRet = pdusec_unpack(pPduBody, &iBodyLen, &stMsg.stBody)) )
    {
        return iRet;
    }
    return 0;
}
}
#endif
