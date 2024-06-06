#ifndef NET_MESSAGE_MESSAGEHEAD_H
#define NET_MESSAGE_MESSAGEHEAD_H

#define MESSAGE_HEAD_SIZE 32

/*
msg head total : 32 bytes
-------------------------------------
| player_guid : 8 bytes             | // 玩家GUID
-------------------------------------
| area_id : 4 bytes                 | // 区服ID
-------------------------------------
| msg_id : 4 bytes                  | // 消息ID
-------------------------------------
| msg_len : 4 bytes                 | // 消息长度, 包括消息头
-------------------------------------
| msg_type : 2 bytes                | // C2S / S2C / S2S
-------------------------------------
| msg_src : 2 bytes                 | // 消息来源, 0:客户端, 1~n:服务端
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
        m_iPlayerGuid = 0;
        m_iAreaId = 0;
        m_iMsgId = 0;
        m_iMsgLen = 0;
        m_iMsgType = 0;
        m_iMsgSrc = 0;
        m_iMsgSeq = 0;
        m_iMsgTime = 0;
    }

    virtual ~MessageHead() {}

    void SetPlayerGuid(uint64_t iPlayerGuid) { m_iPlayerGuid = iPlayerGuid; }
    uint64_t GetPlayerGuid() { return m_iPlayerGuid; }

    void SetAreaId(uint32_t iAreaId) { m_iAreaId = iAreaId; }
    uint32_t GetAreaId() { return m_iAreaId; }

    void SetMsgId(uint32_t iMsgId) { m_iMsgId = iMsgId; }
    uint32_t GetMsgId() { return m_iMsgId; }

    void SetMsgLen(uint32_t iMsgLen) { m_iMsgLen = iMsgLen; }
    uint32_t GetMsgLen() { return m_iMsgLen; }

    void SetMsgType(uint16_t iMsgType) { m_iMsgType = iMsgType; }
    uint16_t GetMsgType() { return m_iMsgType; }

    void SetMsgSrc(uint16_t iMsgSrc) { m_iMsgSrc = iMsgSrc; }
    uint16_t GetMsgSrc() { return m_iMsgSrc; }

    void SetMsgSeq(uint32_t iMsgSeq) { m_iMsgSeq = iMsgSeq; }
    uint32_t GetMsgSeq() { return m_iMsgSeq; }

    void SetMsgTime(uint32_t iMsgTime) { m_iMsgTime = iMsgTime; }
    uint32_t GetMsgTime() { return m_iMsgTime; }

   private:
    uint64_t m_iPlayerGuid;
    uint32_t m_iAreaId;
    uint32_t m_iMsgId;
    uint32_t m_iMsgLen;
    uint16_t m_iMsgType;
    uint16_t m_iMsgSrc;
    uint32_t m_iMsgSeq;
    uint32_t m_iMsgTime;
};

#endif  // NET_MESSAGE_MESSAGEHEAD_H