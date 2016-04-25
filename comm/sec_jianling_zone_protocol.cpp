#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <stdint.h>
#include    <stdarg.h>
#include    <string.h>
#include    <errno.h>
#include    <ctype.h>
#include    <time.h>
#include    <fcntl.h>
#include    <sys/types.h>
#include    <sys/socket.h>
#include    <sys/time.h>
#include    <arpa/inet.h>
#include    <netinet/in.h>
#include    <netinet/ip.h>
#include    <netinet/ip_icmp.h>
#include    <netinet/tcp.h>
#include    <netinet/udp.h>
#include    <linux/stddef.h>

#include    "sec_jianling_zone_protocol.h"


#define PDU_ERR_FIELD_DISMATCH -101
#define PDU_ERR_TAG_INVALID -102
#define PDU_ERR_FIELD_NOTEXISTS -103
#define PDU_ERR_WRONG_PKG -104
#define QzoneProtocolSOH 0x04
#define QzoneProtocolEOT 0x05

#ifdef    __cplusplus
extern "C" {
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
#define ntohll64(x)       (x)
#define htonll64(x)       (x)
#else
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define ntohll64(x)     __bswap_64 (x)
#define htonll64(x)     __bswap_64 (x)
#endif
#endif

#define    NETWORK2HOST_64(v)        (uint64_t)(ntohll64(v))
#define    NETWORK2HOST_32(v)        (uint32_t)(ntohl(v))
#define    NETWORK2HOST_16(v)        (uint16_t)(ntohs(v))
#define    HOST2NETWORK_64(v)        (uint64_t)(htonll64(v))
#define    HOST2NETWORK_32(v)        (uint32_t)(htonl(v))
#define    HOST2NETWORK_16(v)        (uint16_t)(htons(v))

#define    GET_STRING_NOVAL_8(s, l)    do {(l) = *((uint8_t *)(s)); (s) += sizeof(uint8_t); (s) += (l);} while (0)
#define    GET_STRING_VAL_8(s, l, p)    do {(l) = *((uint8_t *)(s)); (s) += sizeof(uint8_t); (p) = (s); (s) += (l);} while (0)
#define    GET_STRING_NOVAL_16(s, l)    do {(l) = *((uint16_t *)(s)); (s) += sizeof(uint16_t); (s) += (l);} while (0)
#define    GET_STRING_VAL_16(s, l, p)    do {(l) = *((uint16_t *)(s)); (s) += sizeof(uint16_t); (p) = (s); (s) += (l);} while (0)
#define    GET_STRING_NOVAL_32(s, l)    do {(l) = *((uint32_t *)(s)); (s) += sizeof(uint32_t); (s) += (l);} while (0)
#define    GET_STRING_VAL_32(s, l, p)    do {(l) = *((uint32_t *)(s)); (s) += sizeof(uint32_t); (p) = (s); (s) += (l);} while (0)

#define    GET_STRING_NOVAL_16_CONV(s, l)    do {(l) = NETWORK2HOST_16(*((uint16_t *)(s))); (s) += sizeof(uint16_t); (s) += (l);} while (0)
#define    GET_STRING_VAL_16_CONV(s, l, p)    do {(l) = NETWORK2HOST_16(*((uint16_t *)(s))); (s) += sizeof(uint16_t); (p) = (s); (s) += (l);} while (0)
#define    GET_STRING_NOVAL_32_CONV(s, l)    do {(l) = NETWORK2HOST_32(*((uint32_t *)(s))); (s) += sizeof(uint32_t); (s) += (l);} while (0)
#define    GET_STRING_VAL_32_CONV(s, l, p)    do {(l) = NETWORK2HOST_32(*((uint32_t *)(s))); (s) += sizeof(uint32_t); (p) = (s); (s) += (l);} while (0)

#define    SET_STRING_VAL_8(s, l, p)        do {*((uint8_t *)(s)) = (l); (s) += sizeof(uint8_t); memcpy((s), (p), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_8_CONV(s, l, p)        do {*((uint8_t *)(s)) = (l); (s) += sizeof(uint8_t); memcpy((s), (p), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_16(s, l, p)    do {*((uint16_t *)(s)) = (l); (s) += sizeof(uint16_t); memcpy((s), (p), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_16_CONV(s, l, p)    do {*((uint16_t *)(s)) = HOST2NETWORK_16((l)); (s) += sizeof(uint16_t); memcpy((s), (p), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_32(s, l, p)    do {*((uint32_t *)(s)) = (l); (s) += sizeof(uint32_t); memcpy((s), (p), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_32_CONV(s, l, p)    do {*((uint32_t *)(s)) = HOST2NETWORK_32((l)); (s) += sizeof(uint32_t); memcpy((s), (p), (l)); (s) += (l);} while (0)

#define    GET_STRING_VAL_NO_LEN(s, l, p)    do {memcpy((p), (s), (l)); (s) += (l);} while (0)
#define    SET_STRING_VAL_NO_LEN(s, l, p)    do {memcpy((s), (p), (l)); (s) += (l);} while (0)


#define    STRING_OCCUPY_LEN_8(l)        (sizeof(uint8_t) + (l))
#define    STRING_OCCUPY_LEN_16(l)        (sizeof(uint26_t) + (l))
#define    STRING_OCCUPY_LEN_32(l)        (sizeof(uint32_t) + (l))
#define    STRING_OCCUPY_LEN_64(l)        (sizeof(uint64_t) + (l))

#define    GET_8(s, l)    do {(l) = *((uint8_t *)(s)); (s) += sizeof(uint8_t); } while (0)
#define    GET_8_CONV(s, l)    do {(l) = *((uint8_t *)(s)); (s) += sizeof(uint8_t); } while (0)
#define    GET_16(s, l)    do {(l) = *((uint16_t *)(s)); (s) += sizeof(uint16_t); } while (0)
#define    GET_16_CONV(s, l)    do {(l) = NETWORK2HOST_16(*((uint16_t *)(s))); (s) += sizeof(uint16_t); } while (0)
#define    GET_32(s, l)    do {(l) = *((uint32_t *)(s)); (s) += sizeof(uint32_t); } while (0)
#define    GET_32_CONV(s, l)    do {(l) = NETWORK2HOST_32(*((uint32_t *)(s))); (s) += sizeof(uint32_t); } while (0)
#define    GET_64(s, l)    do {(l) = *((uint64_t *)(s)); (s) += 8; } while (0)
#define    GET_64_CONV(s, l)    do {(l) = NETWORK2HOST_64(*((uint64_t *)(s))); (s) += sizeof(uint64_t); } while (0)
#define    GET_64_HISTORY(s, l)    do {\
                                char _tmp_buf[8]; \
                                _tmp_buf[0] = (s)[3]; _tmp_buf[1] = (s)[2]; _tmp_buf[2] = (s)[1]; _tmp_buf[3] = (s)[0]; _tmp_buf[4] = (s)[7]; _tmp_buf[5] = (s)[6]; _tmp_buf[6] = (s)[5]; _tmp_buf[7] = (s)[4]; \
                                (l) = *((uint64_t *)_tmp_buf); \
                                (s) += 8; \
                            } while (0)

#define    SET_8(s, l)    do {*((uint8_t *)(s)) = (l); (s) += sizeof(uint8_t); } while (0)
#define    SET_8_CONV(s, l)    do {*((uint8_t *)(s)) = (l); (s) += sizeof(uint8_t); } while (0)
#define    SET_16(s, l)    do {*((uint16_t *)(s)) = (l); (s) += sizeof(uint16_t); } while (0)
#define    SET_16_CONV(s, l)    do {*((uint16_t *)(s)) = HOST2NETWORK_16((l)); (s) += sizeof(uint16_t); } while (0)
#define    SET_32(s, l)    do {*((uint32_t *)(s)) = (l); (s) += sizeof(uint32_t); } while (0)
#define    SET_32_CONV(s, l)    do {*((uint32_t *)(s)) = HOST2NETWORK_32((l)); (s) += sizeof(uint32_t); } while (0)
#define    SET_64(s, l)    do {*((uint64_t *)(s)) = (l); (s) += 8; } while (0)
#define    SET_64_CONV(s, l)    do {*((uint64_t *)(s)) = HOST2NETWORK_64((l)); (s) += sizeof(uint64_t); } while (0)
#define    SET_64_HISTORY(s, l)    do {\
                                char *_tmp_buf = (char *)(&(l)); \
                                (s)[0] = _tmp_buf[3]; (s)[1] = _tmp_buf[2]; (s)[2] = _tmp_buf[1]; (s)[3] = _tmp_buf[0]; (s)[4] = _tmp_buf[7]; (s)[5] = _tmp_buf[6]; (s)[6] = _tmp_buf[5]; (s)[7] = _tmp_buf[4]; \
                                (s) += 8; \
                            } while (0)

#define    malloc_dbg(p, l, t)    do {(p) = (t)malloc((l)); if (!(p)) {(p) = NULL;} else {}} while (0)
#define    free_dbg(p)    do {if ((p)) {free((p)); (p) = NULL;}} while (0)
#ifdef    __cplusplus
}
#endif


namespace SEC_JIANLING_ZONE
{
int pdusec_pack(const CommReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ulReserve) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint64_t))
        return -100;
    SET_64_CONV(ptr, struct_type_ptr->ulReserve);
    len -= (int32_t)sizeof(uint64_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const CommRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : iRetCode) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(int32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->iRetCode);
    len -= (int32_t)sizeof(int32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : iReason) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(int32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->iReason);
    len -= (int32_t)sizeof(int32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : usMsgLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usMsgLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : aszMsg) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszMsg)/sizeof(struct_type_ptr->aszMsg[0]));
    if (struct_type_ptr->usMsgLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usMsgLen) * sizeof((struct_type_ptr->aszMsg[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszMsg, struct_type_ptr->usMsgLen);
    ptr += struct_type_ptr->usMsgLen;
    len -= (int32_t)((struct_type_ptr->usMsgLen) * sizeof(struct_type_ptr->aszMsg[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AreaListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const WorldInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ulWorldId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulWorldId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usWorldNameLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usWorldNameLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszWorldName) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszWorldName)/sizeof(struct_type_ptr->aszWorldName[0]));
    if (struct_type_ptr->usWorldNameLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usWorldNameLen) * sizeof((struct_type_ptr->aszWorldName[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszWorldName, struct_type_ptr->usWorldNameLen);
    ptr += struct_type_ptr->usWorldNameLen;
    len -= (int32_t)((struct_type_ptr->usWorldNameLen) * sizeof(struct_type_ptr->aszWorldName[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AreaInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ulAreaId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAreaId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAreaNameLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAreaNameLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszAreaName) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAreaName)/sizeof(struct_type_ptr->aszAreaName[0]));
    if (struct_type_ptr->usAreaNameLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAreaNameLen) * sizeof((struct_type_ptr->aszAreaName[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAreaName, struct_type_ptr->usAreaNameLen);
    ptr += struct_type_ptr->usAreaNameLen;
    len -= (int32_t)((struct_type_ptr->usAreaNameLen) * sizeof(struct_type_ptr->aszAreaName[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : usWorldCnt) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usWorldCnt);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : astWorldList) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->astWorldList)/sizeof(struct_type_ptr->astWorldList[0]));
    if (struct_type_ptr->usWorldCnt > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    
    for (i = 0; i < (uint32_t)(struct_type_ptr->usWorldCnt); i++) {
        reserve_len = len;
        res = pdusec_pack(&(struct_type_ptr->astWorldList[i]), ptr, &len);
        if (res < 0)
            return res;
        ptr += len;
        len = reserve_len - len;
    }
    
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AreaListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAreaListLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAreaListLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : astAreaList) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->astAreaList)/sizeof(struct_type_ptr->astAreaList[0]));
    if (struct_type_ptr->usAreaListLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    
    for (i = 0; i < (uint32_t)(struct_type_ptr->usAreaListLen); i++) {
        reserve_len = len;
        res = pdusec_pack(&(struct_type_ptr->astAreaList[i]), ptr, &len);
        if (res < 0)
            return res;
        ptr += len;
        len = reserve_len - len;
    }
    
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const RoleListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : ulWorldId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulWorldId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const RoleInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ulRoleId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulRoleId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usRoleNameLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usRoleNameLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszRoleName) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszRoleName)/sizeof(struct_type_ptr->aszRoleName[0]));
    if (struct_type_ptr->usRoleNameLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usRoleNameLen) * sizeof((struct_type_ptr->aszRoleName[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszRoleName, struct_type_ptr->usRoleNameLen);
    ptr += struct_type_ptr->usRoleNameLen;
    len -= (int32_t)((struct_type_ptr->usRoleNameLen) * sizeof(struct_type_ptr->aszRoleName[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const RoleListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usRoleListLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usRoleListLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : astRoleList) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->astRoleList)/sizeof(struct_type_ptr->astRoleList[0]));
    if (struct_type_ptr->usRoleListLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    
    for (i = 0; i < (uint32_t)(struct_type_ptr->usRoleListLen); i++) {
        reserve_len = len;
        res = pdusec_pack(&(struct_type_ptr->astRoleList[i]), ptr, &len);
        if (res < 0)
            return res;
        ptr += len;
        len = reserve_len - len;
    }
    
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const XinYueReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const XinYueRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : ucXinYueFlag) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint8_t))
        return -100;
    SET_8(ptr, struct_type_ptr->ucXinYueFlag);
    len -= (int32_t)sizeof(uint8_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : ulAreaId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAreaId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : ulWorldId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulWorldId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : ulRoleId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulRoleId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : ulLastLoginTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulLastLoginTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (6 : ulBeiDaoTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 6);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulBeiDaoTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (7 : ulAppealTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 7);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (8 : usDescLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 8);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usDescLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (9 : aszDesc) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 9);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszDesc)/sizeof(struct_type_ptr->aszDesc[0]));
    if (struct_type_ptr->usDescLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usDescLen) * sizeof((struct_type_ptr->aszDesc[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszDesc, struct_type_ptr->usDescLen);
    ptr += struct_type_ptr->usDescLen;
    len -= (int32_t)((struct_type_ptr->usDescLen) * sizeof(struct_type_ptr->aszDesc[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usTipLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usTipLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszTip) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszTip)/sizeof(struct_type_ptr->aszTip[0]));
    if (struct_type_ptr->usTipLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usTipLen) * sizeof((struct_type_ptr->aszTip[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszTip, struct_type_ptr->usTipLen);
    ptr += struct_type_ptr->usTipLen;
    len -= (int32_t)((struct_type_ptr->usTipLen) * sizeof(struct_type_ptr->aszTip[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealListReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : usAppealNoLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : aszAppealNo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAppealNo)/sizeof(struct_type_ptr->aszAppealNo[0]));
    if (struct_type_ptr->usAppealNoLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof((struct_type_ptr->aszAppealNo[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAppealNo, struct_type_ptr->usAppealNoLen);
    ptr += struct_type_ptr->usAppealNoLen;
    len -= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof(struct_type_ptr->aszAppealNo[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : usStatusLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usStatusLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : aszStatus) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszStatus)/sizeof(struct_type_ptr->aszStatus[0]));
    if (struct_type_ptr->usStatusLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usStatusLen) * sizeof((struct_type_ptr->aszStatus[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszStatus, struct_type_ptr->usStatusLen);
    ptr += struct_type_ptr->usStatusLen;
    len -= (int32_t)((struct_type_ptr->usStatusLen) * sizeof(struct_type_ptr->aszStatus[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : ulAppealTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (6 : usDescLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 6);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usDescLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (7 : aszDesc) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 7);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszDesc)/sizeof(struct_type_ptr->aszDesc[0]));
    if (struct_type_ptr->usDescLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usDescLen) * sizeof((struct_type_ptr->aszDesc[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszDesc, struct_type_ptr->usDescLen);
    ptr += struct_type_ptr->usDescLen;
    len -= (int32_t)((struct_type_ptr->usDescLen) * sizeof(struct_type_ptr->aszDesc[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealListRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAppealListCnt) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealListCnt);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : astAppealList) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->astAppealList)/sizeof(struct_type_ptr->astAppealList[0]));
    if (struct_type_ptr->usAppealListCnt > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    
    for (i = 0; i < (uint32_t)(struct_type_ptr->usAppealListCnt); i++) {
        reserve_len = len;
        res = pdusec_pack(&(struct_type_ptr->astAppealList[i]), ptr, &len);
        if (res < 0)
            return res;
        ptr += len;
        len = reserve_len - len;
    }
    
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealDetailReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAppealNo)/sizeof(struct_type_ptr->aszAppealNo[0]));
    if (struct_type_ptr->usAppealNoLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof((struct_type_ptr->aszAppealNo[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAppealNo, struct_type_ptr->usAppealNoLen);
    ptr += struct_type_ptr->usAppealNoLen;
    len -= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof(struct_type_ptr->aszAppealNo[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealDetailRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAppealNo)/sizeof(struct_type_ptr->aszAppealNo[0]));
    if (struct_type_ptr->usAppealNoLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof((struct_type_ptr->aszAppealNo[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAppealNo, struct_type_ptr->usAppealNoLen);
    ptr += struct_type_ptr->usAppealNoLen;
    len -= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof(struct_type_ptr->aszAppealNo[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : stWorldInfo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stWorldInfo), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : stRoleInfo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stRoleInfo), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (6 : ulLastLoginTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 6);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulLastLoginTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (7 : ulBeiDaoTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 7);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulBeiDaoTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (8 : usDescLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 8);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usDescLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (9 : aszDesc) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 9);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszDesc)/sizeof(struct_type_ptr->aszDesc[0]));
    if (struct_type_ptr->usDescLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usDescLen) * sizeof((struct_type_ptr->aszDesc[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszDesc, struct_type_ptr->usDescLen);
    ptr += struct_type_ptr->usDescLen;
    len -= (int32_t)((struct_type_ptr->usDescLen) * sizeof(struct_type_ptr->aszDesc[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealNotifyBaseInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ullUin) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint64_t))
        return -100;
    SET_64_CONV(ptr, struct_type_ptr->ullUin);
    len -= (int32_t)sizeof(uint64_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : ulWorldId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulWorldId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : usWorldNameLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usWorldNameLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : aszWorldName) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszWorldName)/sizeof(struct_type_ptr->aszWorldName[0]));
    if (struct_type_ptr->usWorldNameLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usWorldNameLen) * sizeof((struct_type_ptr->aszWorldName[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszWorldName, struct_type_ptr->usWorldNameLen);
    ptr += struct_type_ptr->usWorldNameLen;
    len -= (int32_t)((struct_type_ptr->usWorldNameLen) * sizeof(struct_type_ptr->aszWorldName[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : ulRoleId) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulRoleId);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (6 : usRoleNameLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 6);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usRoleNameLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (7 : aszRoleName) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 7);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszRoleName)/sizeof(struct_type_ptr->aszRoleName[0]));
    if (struct_type_ptr->usRoleNameLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usRoleNameLen) * sizeof((struct_type_ptr->aszRoleName[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszRoleName, struct_type_ptr->usRoleNameLen);
    ptr += struct_type_ptr->usRoleNameLen;
    len -= (int32_t)((struct_type_ptr->usRoleNameLen) * sizeof(struct_type_ptr->aszRoleName[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (8 : vipLevel) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 8);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->vipLevel);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (9 : ulAppealTime) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 9);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealNotifyDetailInfo *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : ulAppealRetCode) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint32_t))
        return -100;
    SET_32_CONV(ptr, struct_type_ptr->ulAppealRetCode);
    len -= (int32_t)sizeof(uint32_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAppealResMsgLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealResMsgLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszAppealResMsg) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAppealResMsg)/sizeof(struct_type_ptr->aszAppealResMsg[0]));
    if (struct_type_ptr->usAppealResMsgLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAppealResMsgLen) * sizeof((struct_type_ptr->aszAppealResMsg[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAppealResMsg, struct_type_ptr->usAppealResMsgLen);
    ptr += struct_type_ptr->usAppealResMsgLen;
    len -= (int32_t)((struct_type_ptr->usAppealResMsgLen) * sizeof(struct_type_ptr->aszAppealResMsg[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealNotifyReq *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommReq) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommReq), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 2);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
    len -= (int32_t)sizeof(uint16_t);
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 3);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_size = (sizeof(struct_type_ptr->aszAppealNo)/sizeof(struct_type_ptr->aszAppealNo[0]));
    if (struct_type_ptr->usAppealNoLen > tmp_size)
        return -1000;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof((struct_type_ptr->aszAppealNo[0]))))
        return -100;
    
    memcpy(ptr, struct_type_ptr->aszAppealNo, struct_type_ptr->usAppealNoLen);
    ptr += struct_type_ptr->usAppealNoLen;
    len -= (int32_t)((struct_type_ptr->usAppealNoLen) * sizeof(struct_type_ptr->aszAppealNo[0]));
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (4 : stBaseInfo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 4);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stBaseInfo), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
/* ----------------- FIELD (5 : stDetailInfo) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 5);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stDetailInfo), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_pack(const AppealNotifyRsp *struct_type_ptr, uint8_t *pkg_bufp, int32_t *pkg_buf_lenp)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    tmp_ptr;
    tmp_ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint32_t    tmp_size;
    tmp_size=0;
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    
/* ----------------- FIELD (1 : stCommRsp) ------------------ */    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 1);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    tmp_ptr = ptr;
    ptr += 2;
    len -= (int32_t)sizeof(uint16_t);
    
    reserve_len = len;
    res = pdusec_pack(&(struct_type_ptr->stCommRsp), ptr, &len);
    if (res < 0)
        return res;
    ptr += len;
    len = reserve_len - len;
    
    SET_16_CONV(tmp_ptr, (ptr - tmp_ptr - 2));
    
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, 65535);
    len -= (int32_t)sizeof(uint16_t);
    
    if (len <= (int32_t)sizeof(uint16_t))
        return -100;
    SET_16_CONV(ptr, (uint16_t)-1);
    len -= (int32_t)sizeof(uint16_t);
    
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, CommReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[1];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ulReserve) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 8)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint64_t))
                    return -100;
                GET_64_CONV(ptr, struct_type_ptr->ulReserve);
                len -= (int32_t)sizeof(uint64_t);
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, CommRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[4];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : iRetCode) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(int32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->iRetCode);
                len -= (int32_t)sizeof(int32_t);
                
                break;
            
/* ----------------- FIELD (2 : iReason) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(int32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->iReason);
                len -= (int32_t)sizeof(int32_t);
                
                break;
            
/* ----------------- FIELD (3 : usMsgLen) ------------------ */            
            case 3:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[2] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usMsgLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (4 : aszMsg) ------------------ */            
            case 4:
                
                all_field_array[3] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszMsg)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszMsg, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaListReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[1];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, WorldInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ulWorldId) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulWorldId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (2 : usWorldNameLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usWorldNameLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszWorldName) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszWorldName)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszWorldName, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usWorldNameLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usWorldNameLen = 0;
        all_field_array[2] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[5];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ulAreaId) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAreaId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (2 : usAreaNameLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAreaNameLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszAreaName) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAreaName)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAreaName, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (4 : usWorldCnt) ------------------ */            
            case 4:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[3] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usWorldCnt);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (5 : astWorldList) ------------------ */            
            case 5:
                
                all_field_array[4] = 1;
                if (fnum == 0)
                    continue;
                
                
                
                for (i = 0; i < (uint32_t)(struct_type_ptr->usWorldCnt); i++) {
                    reserve_len = len;
                    res = pdusec_unpack(ptr, &len, &(struct_type_ptr->astWorldList[i]));
                    if (res < 0)
                        return res;
                    ptr += len;
                    len = reserve_len - len;
                }
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAreaNameLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usAreaNameLen = 0;
        all_field_array[2] = 1;
    }
    
    if (all_field_array[3] == 0) {
        struct_type_ptr->usWorldCnt = 0;
        all_field_array[3] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AreaListRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usAreaListLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAreaListLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : astAreaList) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                
                
                for (i = 0; i < (uint32_t)(struct_type_ptr->usAreaListLen); i++) {
                    reserve_len = len;
                    res = pdusec_unpack(ptr, &len, &(struct_type_ptr->astAreaList[i]));
                    if (res < 0)
                        return res;
                    ptr += len;
                    len = reserve_len - len;
                }
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAreaListLen = 0;
        all_field_array[1] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleListReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[2];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : ulWorldId) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulWorldId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ulRoleId) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulRoleId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (2 : usRoleNameLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usRoleNameLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszRoleName) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszRoleName)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszRoleName, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usRoleNameLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usRoleNameLen = 0;
        all_field_array[2] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, RoleListRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usRoleListLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usRoleListLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : astRoleList) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                
                
                for (i = 0; i < (uint32_t)(struct_type_ptr->usRoleListLen); i++) {
                    reserve_len = len;
                    res = pdusec_unpack(ptr, &len, &(struct_type_ptr->astRoleList[i]));
                    if (res < 0)
                        return res;
                    ptr += len;
                    len = reserve_len - len;
                }
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usRoleListLen = 0;
        all_field_array[1] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, XinYueReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[1];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, XinYueRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[2];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : ucXinYueFlag) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 1)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint8_t))
                    return -100;
                GET_8(ptr, struct_type_ptr->ucXinYueFlag);
                len -= (int32_t)sizeof(uint8_t);
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->ucXinYueFlag = 0;
        all_field_array[1] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[9];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : ulAreaId) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAreaId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (3 : ulWorldId) ------------------ */            
            case 3:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[2] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulWorldId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (4 : ulRoleId) ------------------ */            
            case 4:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[3] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulRoleId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (5 : ulLastLoginTime) ------------------ */            
            case 5:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[4] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulLastLoginTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (6 : ulBeiDaoTime) ------------------ */            
            case 6:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[5] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulBeiDaoTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (7 : ulAppealTime) ------------------ */            
            case 7:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[6] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (8 : usDescLen) ------------------ */            
            case 8:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[7] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usDescLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (9 : aszDesc) ------------------ */            
            case 9:
                
                all_field_array[8] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszDesc)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszDesc, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->ulAreaId = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->ulWorldId = 0;
        all_field_array[2] = 1;
    }
    
    if (all_field_array[3] == 0) {
        struct_type_ptr->ulRoleId = 0;
        all_field_array[3] = 1;
    }
    
    if (all_field_array[4] == 0) {
        struct_type_ptr->ulLastLoginTime = 0;
        all_field_array[4] = 1;
    }
    
    if (all_field_array[5] == 0) {
        struct_type_ptr->ulBeiDaoTime = 0;
        all_field_array[5] = 1;
    }
    
    if (all_field_array[6] == 0) {
        struct_type_ptr->ulAppealTime = 0;
        all_field_array[6] = 1;
    }
    
    if (all_field_array[7] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[7] = 1;
    }
    
    if (all_field_array[8] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[8] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usTipLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usTipLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszTip) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszTip)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszTip, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usTipLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usTipLen = 0;
        all_field_array[2] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealListReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[1];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[7];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : usAppealNoLen) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (2 : aszAppealNo) ------------------ */            
            case 2:
                
                all_field_array[1] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAppealNo)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAppealNo, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (3 : usStatusLen) ------------------ */            
            case 3:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[2] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usStatusLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (4 : aszStatus) ------------------ */            
            case 4:
                
                all_field_array[3] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszStatus)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszStatus, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (5 : ulAppealTime) ------------------ */            
            case 5:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[4] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (6 : usDescLen) ------------------ */            
            case 6:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[5] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usDescLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (7 : aszDesc) ------------------ */            
            case 7:
                
                all_field_array[6] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszDesc)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszDesc, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[0] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[0] = 1;
    }
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usStatusLen = 0;
        all_field_array[2] = 1;
    }
    
    if (all_field_array[3] == 0) {
        struct_type_ptr->usStatusLen = 0;
        all_field_array[3] = 1;
    }
    
    if (all_field_array[4] == 0) {
        struct_type_ptr->ulAppealTime = 0;
        all_field_array[4] = 1;
    }
    
    if (all_field_array[5] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[5] = 1;
    }
    
    if (all_field_array[6] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[6] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealListRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usAppealListCnt) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealListCnt);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : astAppealList) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                
                
                for (i = 0; i < (uint32_t)(struct_type_ptr->usAppealListCnt); i++) {
                    reserve_len = len;
                    res = pdusec_unpack(ptr, &len, &(struct_type_ptr->astAppealList[i]));
                    if (res < 0)
                        return res;
                    ptr += len;
                    len = reserve_len - len;
                }
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAppealListCnt = 0;
        all_field_array[1] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealDetailReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAppealNo)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAppealNo, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[2] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealDetailRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[9];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAppealNo)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAppealNo, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (4 : stWorldInfo) ------------------ */            
            case 4:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[3] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stWorldInfo));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (5 : stRoleInfo) ------------------ */            
            case 5:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[4] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stRoleInfo));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (6 : ulLastLoginTime) ------------------ */            
            case 6:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[5] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulLastLoginTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (7 : ulBeiDaoTime) ------------------ */            
            case 7:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[6] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulBeiDaoTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (8 : usDescLen) ------------------ */            
            case 8:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[7] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usDescLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (9 : aszDesc) ------------------ */            
            case 9:
                
                all_field_array[8] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszDesc)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszDesc, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[2] = 1;
    }
    
    if (all_field_array[5] == 0) {
        struct_type_ptr->ulLastLoginTime = 0;
        all_field_array[5] = 1;
    }
    
    if (all_field_array[6] == 0) {
        struct_type_ptr->ulBeiDaoTime = 0;
        all_field_array[6] = 1;
    }
    
    if (all_field_array[7] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[7] = 1;
    }
    
    if (all_field_array[8] == 0) {
        struct_type_ptr->usDescLen = 0;
        all_field_array[8] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyBaseInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[9];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ullUin) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 8)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint64_t))
                    return -100;
                GET_64_CONV(ptr, struct_type_ptr->ullUin);
                len -= (int32_t)sizeof(uint64_t);
                
                break;
            
/* ----------------- FIELD (2 : ulWorldId) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulWorldId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (3 : usWorldNameLen) ------------------ */            
            case 3:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[2] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usWorldNameLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (4 : aszWorldName) ------------------ */            
            case 4:
                
                all_field_array[3] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszWorldName)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszWorldName, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (5 : ulRoleId) ------------------ */            
            case 5:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[4] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulRoleId);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (6 : usRoleNameLen) ------------------ */            
            case 6:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[5] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usRoleNameLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (7 : aszRoleName) ------------------ */            
            case 7:
                
                all_field_array[6] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszRoleName)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszRoleName, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (8 : vipLevel) ------------------ */            
            case 8:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[7] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->vipLevel);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (9 : ulAppealTime) ------------------ */            
            case 9:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[8] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAppealTime);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usWorldNameLen = 0;
        all_field_array[2] = 1;
    }
    
    if (all_field_array[3] == 0) {
        struct_type_ptr->usWorldNameLen = 0;
        all_field_array[3] = 1;
    }
    
    if (all_field_array[5] == 0) {
        struct_type_ptr->usRoleNameLen = 0;
        all_field_array[5] = 1;
    }
    
    if (all_field_array[6] == 0) {
        struct_type_ptr->usRoleNameLen = 0;
        all_field_array[6] = 1;
    }
    
    if (all_field_array[8] == 0) {
        struct_type_ptr->ulAppealTime = 0;
        all_field_array[8] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyDetailInfo *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[3];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : ulAppealRetCode) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                if (fnum != 4)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint32_t))
                    return -100;
                GET_32_CONV(ptr, struct_type_ptr->ulAppealRetCode);
                len -= (int32_t)sizeof(uint32_t);
                
                break;
            
/* ----------------- FIELD (2 : usAppealResMsgLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealResMsgLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszAppealResMsg) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAppealResMsg)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAppealResMsg, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyReq *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[5];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommReq) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommReq));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (2 : usAppealNoLen) ------------------ */            
            case 2:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[1] = 1;
                
                if (fnum != 2)
                    return PDU_ERR_FIELD_DISMATCH;
                
                if (len <= (int32_t)sizeof(uint16_t))
                    return -100;
                GET_16_CONV(ptr, struct_type_ptr->usAppealNoLen);
                len -= (int32_t)sizeof(uint16_t);
                
                break;
            
/* ----------------- FIELD (3 : aszAppealNo) ------------------ */            
            case 3:
                
                all_field_array[2] = 1;
                if (fnum == 0)
                    continue;
                
                if (fnum > sizeof(struct_type_ptr->aszAppealNo)) 
                    return -10001;
                
                
                if (len < (int32_t)fnum)
                    return -100;
                memcpy(struct_type_ptr->aszAppealNo, ptr, fnum);
                ptr+=fnum;
                
                len -= fnum;
                break;
            
/* ----------------- FIELD (4 : stBaseInfo) ------------------ */            
            case 4:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[3] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stBaseInfo));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
/* ----------------- FIELD (5 : stDetailInfo) ------------------ */            
            case 5:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[4] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stDetailInfo));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    if (all_field_array[1] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[1] = 1;
    }
    
    if (all_field_array[2] == 0) {
        struct_type_ptr->usAppealNoLen = 0;
        all_field_array[2] = 1;
    }
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}

int pdusec_unpack(const uint8_t *pkg_bufp, int32_t *pkg_buf_lenp, AppealNotifyRsp *struct_type_ptr)
{
    int    res;
    res=-255;
    int32_t    len;
    len=0;
    int32_t    reserve_len;
    reserve_len=0;
    const uint8_t*    ptr;
    ptr=NULL;
    uint8_t*    pos;
    pos=NULL;
    uint32_t    tag;
    tag=0;
    uint32_t    fnum;
    fnum=0;
    uint32_t    i;
    i=0;
    uint32_t    j;
    j=0;
    uint32_t    k;
    k=0;
    uint8_t    token;
    token=0;
    
    uint8_t all_field_array[1];
    
    if (struct_type_ptr==NULL)
        return -1;
    
    if (pkg_bufp==NULL)
        return -2;
    
    if (pkg_buf_lenp==NULL)
        return -3;
    
    ptr = pkg_bufp;
    len = (int32_t)(*pkg_buf_lenp);
    reserve_len = 0;
    if (sizeof(all_field_array) > 0)
        memset(all_field_array, 0, sizeof(all_field_array));
    
    while (len > 0) {
        if (len <= (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, tag);
        len -= (int32_t)sizeof(uint16_t);
        
        if (len < (int32_t)sizeof(uint16_t))
            return -100;
        GET_16_CONV(ptr, fnum);
        len -= (int32_t)sizeof(uint16_t);
        
        if (tag == (uint16_t)-1 && fnum == (uint16_t)-1)
            break;
        
        if (tag > 1000)
            return PDU_ERR_TAG_INVALID;
        
        switch (tag) {
            
/* ----------------- FIELD (1 : stCommRsp) ------------------ */            
            case 1:
                
                if (fnum == 0)
                    continue;
                
                all_field_array[0] = 1;
                
                reserve_len = len;
                res = pdusec_unpack(ptr, &len, &(struct_type_ptr->stCommRsp));
                if (res < 0)
                    return res;
                ptr += len;
                len = reserve_len - len;
                break;
            
            default:
                ptr += fnum;
                len -= fnum;
                break;
        }
    }
    
    
    for (i = 0; i < sizeof(all_field_array); i++) {
        if (all_field_array[i] == 0) {
            return PDU_ERR_FIELD_NOTEXISTS;
        }
    }
    
    *pkg_buf_lenp = (int32_t)(ptr - pkg_bufp);
    return 0;
}


}
