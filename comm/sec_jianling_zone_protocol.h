#ifndef __SEC_JIANLING_ZONE_PROTOCOL_H__
#define __SEC_JIANLING_ZONE_PROTOCOL_H__
#include    <stdint.h>

namespace SEC_JIANLING_ZONE
{




#pragma pack(1)

typedef    struct {
    uint64_t        ulReserve;        //保留字段
} CommReq;


typedef    struct {
    int32_t        iRetCode;        //处理结果
    int32_t        iReason;        //处理原因，参考emCheckXXXReasion
    uint16_t        usMsgLen;        //错误消息长度
    uint8_t        aszMsg[256];        //错误信息，可以直接展示给用户
} CommRsp;


typedef    struct {
    CommReq        stCommReq;
} AreaListReq;


typedef    struct {
    uint32_t        ulWorldId;        //服务器Id
    uint16_t        usWorldNameLen;        //服务器名字长度
    uint8_t        aszWorldName[128];        //服务器名字
} WorldInfo;


typedef    struct {
    uint32_t        ulAreaId;        //大区Id
    uint16_t        usAreaNameLen;        //大区名字长度
    uint8_t        aszAreaName[128];        //大区名字
    uint16_t        usWorldCnt;        //服务器个数
    WorldInfo        astWorldList[128];        //服务器列表
} AreaInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAreaListLen;        //大区列表长度
    AreaInfo        astAreaList[128];        //大区列表
} AreaListRsp;


typedef    struct {
    CommReq        stCommReq;
    uint32_t        ulWorldId;        //服务器Id
} RoleListReq;


typedef    struct {
    uint32_t        ulRoleId;        //角色Id
    uint16_t        usRoleNameLen;        //角色名字长度
    uint8_t        aszRoleName[128];        //角色名字
} RoleInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usRoleListLen;        //角色列表长度
    RoleInfo        astRoleList[128];        //角色列表
} RoleListRsp;


typedef    struct {
    CommReq        stCommReq;
} XinYueReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint8_t        ucXinYueFlag;        //是否心悦用户标识
} XinYueRsp;


typedef    struct {
    CommReq        stCommReq;
    uint32_t        ulAreaId;        //大区Id
    uint32_t        ulWorldId;        //服务器Id
    uint32_t        ulRoleId;        //角色Id
    uint32_t        ulLastLoginTime;        //被盗前最后一次登录时间
    uint32_t        ulBeiDaoTime;        //被盗时间
    uint32_t        ulAppealTime;        //申诉时间
    uint16_t        usDescLen;        //描述长度
    uint8_t        aszDesc[512];        //描述
} AppealReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usTipLen;        //提示长度
    uint8_t        aszTip[128];        //提示
} AppealRsp;


typedef    struct {
    CommReq        stCommReq;
} AppealListReq;


typedef    struct {
    uint16_t        usAppealNoLen;        //申诉单号长度
    uint8_t        aszAppealNo[30];        //申诉单号:rob-14033117133114300207
    uint16_t        usStatusLen;        //进度长度
    uint8_t        aszStatus[128];        //进度
    uint32_t        ulAppealTime;        //申诉时间
    uint16_t        usDescLen;        //摘要长度
    uint8_t        aszDesc[512];        //摘要
} AppealInfo;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAppealListCnt;        //申诉单条数
    AppealInfo        astAppealList[128];        //申诉单列表
} AppealListRsp;


typedef    struct {
    CommReq        stCommReq;
    uint16_t        usAppealNoLen;        //申诉单号长度
    uint8_t        aszAppealNo[30];        //申诉单号:rob-14033117133114300207
} AppealDetailReq;


typedef    struct {
    CommRsp        stCommRsp;
    uint16_t        usAppealNoLen;        //申诉单号长度
    uint8_t        aszAppealNo[30];        //申诉单号:rob-14033117133114300207
    WorldInfo        stWorldInfo;        //服务器名
    RoleInfo        stRoleInfo;        //角色名
    uint32_t        ulLastLoginTime;        //被盗前最后一次登录时间
    uint32_t        ulBeiDaoTime;        //被盗时间
    uint16_t        usDescLen;        //记录详情长度
    uint8_t        aszDesc[512];        //记录详情
} AppealDetailRsp;


typedef    struct {
    uint64_t        ullUin;        //用户uin
    uint32_t        ulWorldId;        //服务器Id
    uint16_t        usWorldNameLen;        //服务器名字长度
    uint8_t        aszWorldName[128];        //服务器名字
    uint32_t        ulRoleId;        //角色Id
    uint16_t        usRoleNameLen;        //角色名字长度
    uint8_t        aszRoleName[128];        //角色名字
    uint32_t        vipLevel;        //心悦用户等级
    uint32_t        ulAppealTime;        //申诉时间
} AppealNotifyBaseInfo;


typedef    struct {
    uint32_t        ulAppealRetCode;        //业务处理结果
    uint16_t        usAppealResMsgLen;        //申请单处理结果消息长度
    uint8_t        aszAppealResMsg[256];        //申请单处理结果信息，可以直接展示给用户
} AppealNotifyDetailInfo;


typedef    struct {
    CommReq        stCommReq;
    uint16_t        usAppealNoLen;        //申诉单号长度
    uint8_t        aszAppealNo[30];        //申诉单号:rob-14033117133114300207
    AppealNotifyBaseInfo        stBaseInfo;        //申诉结果基本信息
    AppealNotifyDetailInfo        stDetailInfo;        //申诉结果详细信息
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

