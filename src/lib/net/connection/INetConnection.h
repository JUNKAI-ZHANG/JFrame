#ifndef NET_CONNECTION_INETCONNECTION_H
#define NET_CONNECTION_INETCONNECTION_H

#include <stdint.h>
#include <stdio.h>

#include <string>

#include "IConnectionContext.h"
#include "lib/util/RingBuffer.h"

class INetConnection {
   public:
    explicit INetConnection(IConnectionContext* kConnectionContext) {
        m_kConnectionContext = kConnectionContext;
        m_kRecvBuffer = new RingBuffer();
    }

    virtual ~INetConnection() {
    }

    // 根据连接类型发送消息
    virtual void SendMsg(const char* pMsg, uint32_t iLen) {}
    // 做释放逻辑
    virtual void Close() {}

    uint64_t GetConnGuid() { return (((uint64_t)GetIp()) << 16) | GetPort(); }

    uint32_t GetIp() { return m_kConnectionContext->GetIp(); }
    uint16_t GetPort() { return m_kConnectionContext->GetPort(); }
    int32_t GetSocketFd() { return m_kConnectionContext->GetSocketFd(); }
    uint64_t GetConnMs() { return m_kConnectionContext->GetConnMs(); }

    RingBuffer* GetRecvBuffer() { return m_kRecvBuffer; }

    std::string GetIpStr() {
        char pIpStr[16] = {0};
        snprintf(pIpStr, sizeof(pIpStr), "%d.%d.%d.%d",
                 (GetIp() & 0xFF000000) >> 24,
                 (GetIp() & 0x00FF0000) >> 16,
                 (GetIp() & 0x0000FF00) >> 8,
                 (GetIp() & 0x000000FF));
        return std::string(pIpStr);
    }

    virtual std::string ToString() {
        char pStr[128] = {0};
        snprintf(pStr, sizeof(pStr), "ip:%s port:%d socket_fd:%d conn_ms:%lu", GetIpStr().c_str(), GetPort(), GetSocketFd(), GetConnMs());
        return std::string(pStr);
    }

   private:
    IConnectionContext* m_kConnectionContext = nullptr;

    RingBuffer* m_kRecvBuffer = nullptr;
};

#endif  // NET_CONNECTION_INETCONNECTION_H