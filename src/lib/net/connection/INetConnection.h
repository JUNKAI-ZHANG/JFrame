#ifndef NET_CONNECTION_INETCONNECTION_H
#define NET_CONNECTION_INETCONNECTION_H

#include <stdint.h>
#include <stdio.h>

#include <string>

#include "IConnectionContext.h"
#include "google/protobuf/message.h"
#include "lib/net/NetService.h"
#include "lib/net/message/MessageHead.h"
#include "lib/util/RingBuffer.h"
#include "lib/util/Time.h"
#include "proto/msg_id.pb.h"

class INetConnection {
   public:
    explicit INetConnection(std::unique_ptr<IConnectionContext>&& pConnectionContext);
    virtual ~INetConnection();

    virtual void SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage);
    virtual void SendMsg(const char* pMsg, uint32_t iLen);
    virtual void Close();

    uint64_t GetConnGuid();
    uint32_t GetIp();
    uint16_t GetPort();
    int32_t GetSocketFd();
    uint64_t GetConnMs();
    std::shared_ptr<RingBuffer> GetRecvBuffer();
    std::string GetIpStr();
    virtual std::string ToString();

   protected:
    std::unique_ptr<IConnectionContext> m_pConnectionContext;
    std::shared_ptr<RingBuffer> m_pRecvBuffer;
};

class CSConnection : public INetConnection {
   public:
    CSConnection(std::unique_ptr<CSConnectionContext>&& kCSConnectionContext);
    virtual ~CSConnection();

    virtual void SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage) override;
    virtual void SendMsg(const char* pMsg, uint32_t iLen) override;
    virtual void Close() override;
};

class SSConnection : public INetConnection {
   public:
    SSConnection(std::unique_ptr<SSConnectionContext>&& kSSConnectionContext);
    virtual ~SSConnection();

    virtual void SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage) override;
    virtual void SendMsg(const char* pMsg, uint32_t iLen);
    virtual void Close();
};

#endif  // NET_CONNECTION_INETCONNECTION_H