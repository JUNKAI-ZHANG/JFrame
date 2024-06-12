#ifndef NET_MANAGER_NETMESSAGEMGR_H
#define NET_MANAGER_NETMESSAGEMGR_H

#include <memory>

#include "lib/net/connection/INetConnection.h"
#include "lib/net/message/MessageHead.h"
#include "lib/util/BlockQueue.h"

class NetMessageMgr;

class NetMessage {
   public:
    friend NetMessageMgr;

   public:
    explicit NetMessage(MessageHead kMessageHead,
                        char* pMessageBodyBytes,
                        std::shared_ptr<INetConnection> pConn)
        : m_kMessageHead(kMessageHead),
          m_kMessageBodyBytes(pMessageBodyBytes),
          m_kConn(pConn) {
    }
    virtual ~NetMessage() {
    }

    MessageHead GetMessageHead() { return m_kMessageHead; }
    std::string GetMessageBodyBytes() { return m_kMessageBodyBytes; }
    std::shared_ptr<INetConnection> GetNetConnection() { return m_kConn; }

   private:
    MessageHead m_kMessageHead;
    std::string m_kMessageBodyBytes;
    std::shared_ptr<INetConnection> m_kConn;
};

class NetMessageMgr {
   public:
    friend NetMessage;
    explicit NetMessageMgr();
    virtual ~NetMessageMgr();

    void AddRecvMessage(std::unique_ptr<NetMessage>& pNetMessage);
    void AddRecvMessage(MessageHead kMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn);

    void AddSendMessage(std::unique_ptr<NetMessage>& pNetMessage);
    void AddSendMessage(MessageHead kMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn);

    std::deque<std::unique_ptr<NetMessage>> GetRecvMessageDeque();
    std::deque<std::unique_ptr<NetMessage>> GetRecvMessageDequeTimeout(uint32_t iTimeoutMs);

    std::deque<std::unique_ptr<NetMessage>> GetSendMessageDeque();
    std::deque<std::unique_ptr<NetMessage>> GetSendMessageDequeTimeout(uint32_t iTimeoutMs);

   private:
    BlockQueue<std::unique_ptr<NetMessage>> m_kNetRecvMessageDeque;
    BlockQueue<std::unique_ptr<NetMessage>> m_kNetSendMessageDeque;
};

#endif  // NET_MANAGER_NETMESSAGEMGR_H