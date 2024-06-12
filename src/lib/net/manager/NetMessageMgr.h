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
    explicit NetMessage(MessageHead* kMessageHead, char* kMessageBodyBytes, INetConnection* kConn) : m_kMessageHead(kMessageHead),
                                                                                                     m_kMessageBodyBytes(kMessageBodyBytes),
                                                                                                     m_kConn(kConn) {
    }

   private:
    std::shared_ptr<MessageHead> m_kMessageHead;
    std::string m_kMessageBodyBytes;
    std::shared_ptr<INetConnection> m_kConn;
};

class NetMessageMgr {
   public:
    friend NetMessage;
    explicit NetMessageMgr();
    virtual ~NetMessageMgr();

    void Add(NetMessage* kNetMessage);
    void Add(MessageHead* kMessageHead, char* kMessageBodyBytes, INetConnection* kConn);

    std::deque<std::shared_ptr<NetMessage>> Swap();
    std::deque<std::shared_ptr<NetMessage>> SwapTimeout(uint32_t iTimeoutMs);

   private:
    BlockQueue<NetMessage>* m_kNetMessageQueue;
};

#endif  // NET_MANAGER_NETMESSAGEMGR_H