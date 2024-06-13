#ifndef NET_MESSAGE_MESSAGEHEAD_H
#define NET_MESSAGE_MESSAGEHEAD_H

#define MESSAGE_HEAD_SIZE 32

#include <string.h>

/*
msg head total : 32 bytes
-------------------------------------
| player_guid : 8 bytes             | // 玩家UID
-------------------------------------
| area_id : 4 bytes                 | // 区服ID
-------------------------------------
| msg_id : 4 bytes                  | // 消息ID
-------------------------------------
| msg_len : 4 bytes                 | // 消息长度, 包括消息头
-------------------------------------
| msg_src : 2 bytes                 | // 消息来源, 0:错误, 1:客户端 2~n:服务端
-------------------------------------
| msg_dst : 2 bytes                 | // 消息目的, 0:错误, 1:客户端 2~n:服务端
-------------------------------------
| msg_seq : 4 bytes                 | // 消息序列号
-------------------------------------
| msg_time : 4 bytes                | // 消息时间戳
-------------------------------------
|                                   |
|            IMessage               |
|                                   |
-------------------------------------
*/

class MessageHead {
   public:
    MessageHead() {
        m_iPlayerUid = 0;
        m_iAreaId = 0;
        m_iMsgId = 0;
        m_iMsgLen = 0;
        m_iMsgSrc = 0;
        m_iMsgDst = 0;
        m_iMsgSeq = 0;
        m_iMsgTime = 0;
    }

    virtual ~MessageHead() {
    }

    std::string EncodeMessageHeadBytes() {
        char pHeadBytes[MESSAGE_HEAD_SIZE];
        memset(pHeadBytes, 0, sizeof(char));

        uint64_t* pPlayerGuid = reinterpret_cast<uint64_t*>(pHeadBytes);
        *pPlayerGuid = m_iPlayerUid;

        uint32_t* pAreaId = reinterpret_cast<uint32_t*>(pHeadBytes + 8);
        *pAreaId = m_iAreaId;

        uint32_t* pMsgId = reinterpret_cast<uint32_t*>(pHeadBytes + 12);
        *pMsgId = m_iMsgId;

        uint32_t* pMsgLen = reinterpret_cast<uint32_t*>(pHeadBytes + 16);
        *pMsgLen = m_iMsgLen;

        uint16_t* pMsgSrc = reinterpret_cast<uint16_t*>(pHeadBytes + 20);
        *pMsgSrc = m_iMsgSrc;

        uint16_t* pMsgDst = reinterpret_cast<uint16_t*>(pHeadBytes + 22);
        *pMsgDst = m_iMsgDst;

        uint32_t* pMsgSeq = reinterpret_cast<uint32_t*>(pHeadBytes + 24);
        *pMsgSeq = m_iMsgSeq;

        uint32_t* pMsgTime = reinterpret_cast<uint32_t*>(pHeadBytes + 28);
        *pMsgTime = m_iMsgTime;

        return std::string(pHeadBytes, MESSAGE_HEAD_SIZE);
    }

    void DecodeMessageHeadBytes(char* pHeadBytes) {
        uint64_t* pPlayerUid = reinterpret_cast<uint64_t*>(pHeadBytes);
        m_iPlayerUid = *pPlayerUid;

        uint32_t* pAreaId = reinterpret_cast<uint32_t*>(pHeadBytes + 8);
        m_iAreaId = *pAreaId;

        uint32_t* pMsgId = reinterpret_cast<uint32_t*>(pHeadBytes + 12);
        m_iMsgId = *pMsgId;

        uint32_t* pMsgLen = reinterpret_cast<uint32_t*>(pHeadBytes + 16);
        m_iMsgLen = *pMsgLen;

        uint16_t* pMsgSrc = reinterpret_cast<uint16_t*>(pHeadBytes + 20);
        m_iMsgSrc = *pMsgSrc;

        uint16_t* pMsgDst = reinterpret_cast<uint16_t*>(pHeadBytes + 22);
        m_iMsgDst = *pMsgDst;

        uint32_t* pMsgSeq = reinterpret_cast<uint32_t*>(pHeadBytes + 24);
        m_iMsgSeq = *pMsgSeq;

        uint32_t* pMsgTime = reinterpret_cast<uint32_t*>(pHeadBytes + 28);
        m_iMsgTime = *pMsgTime;
    }

    // 打印
    void PrintMessageHead() {
        printf("player_uid : %lu\n", m_iPlayerUid);
        printf("area_id : %u\n", m_iAreaId);
        printf("msg_id : %u\n", m_iMsgId);
        printf("msg_len : %u\n", m_iMsgLen);
        printf("msg_src : %u\n", m_iMsgSrc);
        printf("msg_dst : %u\n", m_iMsgDst);
        printf("msg_seq : %u\n", m_iMsgSeq);
        printf("msg_time : %u\n", m_iMsgTime);
    }

    void SetPlayerUid(uint64_t iPlayerUid) { m_iPlayerUid = iPlayerUid; }
    uint64_t GetPlayerUid() { return m_iPlayerUid; }

    void SetAreaId(uint32_t iAreaId) { m_iAreaId = iAreaId; }
    uint32_t GetAreaId() { return m_iAreaId; }

    void SetMsgId(uint32_t iMsgId) { m_iMsgId = iMsgId; }
    uint32_t GetMsgId() { return m_iMsgId; }

    void SetMsgLen(uint32_t iMsgLen) { m_iMsgLen = iMsgLen; }
    uint32_t GetMsgLen() { return m_iMsgLen; }

    void SetMsgSrc(uint16_t iMsgSrc) { m_iMsgSrc = iMsgSrc; }
    uint16_t GetMsgSrc() { return m_iMsgSrc; }

    void SetMsgDst(uint16_t iMsgDst) { m_iMsgDst = iMsgDst; }
    uint16_t GetMsgDst() { return m_iMsgDst; }

    void SetMsgSeq(uint32_t iMsgSeq) { m_iMsgSeq = iMsgSeq; }
    uint32_t GetMsgSeq() { return m_iMsgSeq; }

    void SetMsgTime(uint32_t iMsgTime) { m_iMsgTime = iMsgTime; }
    uint32_t GetMsgTime() { return m_iMsgTime; }

   private:
    uint64_t m_iPlayerUid;
    uint32_t m_iAreaId;
    uint32_t m_iMsgId;
    uint32_t m_iMsgLen;
    uint16_t m_iMsgSrc;
    uint16_t m_iMsgDst;
    uint32_t m_iMsgSeq;
    uint32_t m_iMsgTime;
};

#endif  // NET_MESSAGE_MESSAGEHEAD_H