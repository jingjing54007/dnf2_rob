#ifndef __SEC_JIANLING_ZONE_PROTOCOL_H__
#define __SEC_JIANLING_ZONE_PROTOCOL_H__
#include    <stdint.h>

namespace SEC_JIANLING_ZONE
{




#pragma pack(1)

typedef    struct {
    uint64_t        ulReserve;        //�����ֶ�
} CommReq;


typedef    struct {
    int32_t        iRetCode;        //������
    int32_t        iReason;        //����ԭ�򣬲ο�emCheckXXXReasion
    uint16_t        usMsgLen;        //������Ϣ����
    uint8_t        aszMsg[256];        //������Ϣ������ֱ��չʾ���û�
} CommRsp;


typedef    struct {
    CommReq        stCommReq;
} AreaListReq;


typedef    struct {
    uint32_t        ulWorldId;        //������Id
    uint16_t        usWorldNameLen;        //���������ֳ���
    uint8_t        aszWorldName[128];        //����������
} WorldInfo;


typedef    struct {
    uint32_t        ulAreaId;        //����Id
    uint16_t        usAreaNameLen;        //�������ֳ���
    uint8_t        aszAreaName[128];        //��������
    uint16_t        usWorldCnt;        //����������
    WorldInfo        astWorldList[128];        //�������б�
} AreaInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAreaListLen;        //�����б���
    AreaInfo        astAreaList[128];        //�����б�
} AreaListRsp;


typedef    struct {
    CommReq        stCommReq;
    uint32_t        ulWorldId;        //������Id
} RoleListReq;


typedef    struct {
    uint32_t        ulRoleId;        //��ɫId
    uint16_t        usRoleNameLen;        //��ɫ���ֳ���
    uint8_t        aszRoleName[128];        //��ɫ����
} RoleInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usRoleListLen;        //��ɫ�б���
    RoleInfo        astRoleList[128];        //��ɫ�б�
} RoleListRsp;


typedef    struct {
    CommReq        stCommReq;
} XinYueReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint8_t        ucXinYueFlag;        //�Ƿ������û���ʶ
} XinYueRsp;


typedef    struct {
    CommReq        stCommReq;
    uint32_t        ulAreaId;        //����Id
    uint32_t        ulWorldId;        //������Id
    uint32_t        ulRoleId;        //��ɫId
    uint32_t        ulLastLoginTime;        //����ǰ���һ�ε�¼ʱ��
    uint32_t        ulBeiDaoTime;        //����ʱ��
    uint32_t        ulAppealTime;        //����ʱ��
    uint16_t        usDescLen;        //��������
    uint8_t        aszDesc[512];        //����
} AppealReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usTipLen;        //��ʾ����
    uint8_t        aszTip[128];        //��ʾ
} AppealRsp;


typedef    struct {
    CommReq        stCommReq;
} AppealListReq;


typedef    struct {
    uint16_t        usAppealNoLen;        //���ߵ��ų���
    uint8_t        aszAppealNo[30];        //���ߵ���:rob-14033117133114300207
    uint16_t        usStatusLen;        //���ȳ���
    uint8_t        aszStatus[128];        //����
    uint32_t        ulAppealTime;        //����ʱ��
    uint16_t        usDescLen;        //ժҪ����
    uint8_t        aszDesc[512];        //ժҪ
} AppealInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAppealListCnt;        //���ߵ�����
    AppealInfo        astAppealList[128];        //���ߵ��б�
} AppealListRsp;


typedef    struct {
    CommReq        stCommReq;
    uint16_t        usAppealNoLen;        //���ߵ��ų���
    uint8_t        aszAppealNo[30];        //���ߵ���:rob-14033117133114300207
} AppealDetailReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAppealNoLen;        //���ߵ��ų���
    uint8_t        aszAppealNo[30];        //���ߵ���:rob-14033117133114300207
    WorldInfo        stWorldInfo;        //��������
    RoleInfo        stRoleInfo;        //��ɫ��
    uint32_t        ulLastLoginTime;        //����ǰ���һ�ε�¼ʱ��
    uint32_t        ulBeiDaoTime;        //����ʱ��
    uint16_t        usDescLen;        //��¼���鳤��
    uint8_t        aszDesc[512];        //��¼����
} AppealDetailRsp;


typedef    struct {
    uint64_t        ullUin;        //�û�uin
    uint32_t        ulWorldId;        //������Id
    uint16_t        usWorldNameLen;        //���������ֳ���
    uint8_t        aszWorldName[128];        //����������
    uint32_t        ulRoleId;        //��ɫId
    uint16_t        usRoleNameLen;        //��ɫ���ֳ���
    uint8_t        aszRoleName[128];        //��ɫ����
    uint32_t        vipLevel;        //�����û��ȼ�
    uint32_t        ulAppealTime;        //����ʱ��
} AppealNotifyBaseInfo;


typedef    struct {
    uint32_t        ulAppealRetCode;        //ҵ������
    uint16_t        usAppealResMsgLen;        //���뵥��������Ϣ����
    uint8_t        aszAppealResMsg[256];        //���뵥��������Ϣ������ֱ��չʾ���û�
} AppealNotifyDetailInfo;


typedef    struct {
    CommReq        stCommReq;
    uint16_t        usAppealNoLen;        //���ߵ��ų���
    uint8_t        aszAppealNo[30];        //���ߵ���:rob-14033117133114300207
    AppealNotifyBaseInfo        stBaseInfo;        //���߽��������Ϣ
    AppealNotifyDetailInfo        stDetailInfo;        //���߽����ϸ��Ϣ
} AppealNotifyReq;


typedef    struct {
    CommRsp        stCommRsp;
} AppealNotifyRsp;


#pragma pack()


int pdusec_pack(const CommReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, CommReq *struct_type_ptr);
int pdusec_pack(const CommRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, CommRsp *struct_type_ptr);
int pdusec_pack(const AreaListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaListReq *struct_type_ptr);
int pdusec_pack(const WorldInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, WorldInfo *struct_type_ptr);
int pdusec_pack(const AreaInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaInfo *struct_type_ptr);
int pdusec_pack(const AreaListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaListRsp *struct_type_ptr);
int pdusec_pack(const RoleListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleListReq *struct_type_ptr);
int pdusec_pack(const RoleInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleInfo *struct_type_ptr);
int pdusec_pack(const RoleListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleListRsp *struct_type_ptr);
int pdusec_pack(const XinYueReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, XinYueReq *struct_type_ptr);
int pdusec_pack(const XinYueRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, XinYueRsp *struct_type_ptr);
int pdusec_pack(const AppealReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealReq *struct_type_ptr);
int pdusec_pack(const AppealRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealRsp *struct_type_ptr);
int pdusec_pack(const AppealListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealListReq *struct_type_ptr);
int pdusec_pack(const AppealInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealInfo *struct_type_ptr);
int pdusec_pack(const AppealListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealListRsp *struct_type_ptr);
int pdusec_pack(const AppealDetailReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealDetailReq *struct_type_ptr);
int pdusec_pack(const AppealDetailRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealDetailRsp *struct_type_ptr);
int pdusec_pack(const AppealNotifyBaseInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyBaseInfo *struct_type_ptr);
int pdusec_pack(const AppealNotifyDetailInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyDetailInfo *struct_type_ptr);
int pdusec_pack(const AppealNotifyReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyReq *struct_type_ptr);
int pdusec_pack(const AppealNotifyRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp);
int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyRsp *struct_type_ptr);

}
#endif

