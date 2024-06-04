#ifndef NET_CONNECTION_INETCONNECTION_H
#define NET_CONNECTION_INETCONNECTION_H

#include <stdint.h>
#include <stdio.h>

#include <string>

#include "util/RingBuffer.h"

class INetConnection {
   public:
    INetConnection(uint32_t iIp, uint16_t iPort, uint32_t iSocketFd, uint64_t lConnMs)
        : m_iIp(iIp), m_iPort(iPort), m_iSocketFd(iSocketFd), m_lConnMs(lConnMs) {
    }

    virtual ~INetConnection() {}

    // 根据连接类型发送消息
    virtual void SendMsg(const char* pMsg, uint32_t iLen) = 0;
    // 做释放逻辑
    virtual void Close() = 0;

    uint32_t GetIp() { return m_iIp; }
    uint16_t GetPort() { return m_iPort; }
    uint32_t GetSocketFd() { return m_iSocketFd; }
    uint64_t GetConnMs() { return m_lConnMs; }

    RingBuffer* GetRecvBuffer() { return m_kRecvBuffer; }

    std::string GetIpStr() {
        char pIpStr[16] = {0};
        snprintf(pIpStr, sizeof(pIpStr), "%d.%d.%d.%d",
                 (m_iIp & 0xFF000000) >> 24,
                 (m_iIp & 0x00FF0000) >> 16,
                 (m_iIp & 0x0000FF00) >> 8,
                 (m_iIp & 0x000000FF));
        return std::string(pIpStr);
    }

    virtual std::string ToString() {
        char pStr[128] = {0};
        snprintf(pStr, sizeof(pStr), "ip:%s port:%d socket:%d connMs:%lu",
                 GetIpStr().c_str(), m_iPort, m_iSocketFd, m_lConnMs);
        return std::string(pStr);
    }

   private:
    uint32_t m_iIp;
    uint16_t m_iPort;
    uint32_t m_iSocketFd;
    uint64_t m_lConnMs;

    RingBuffer* m_kRecvBuffer;
};

#endif  // NET_CONNECTION_INETCONNECTION_H