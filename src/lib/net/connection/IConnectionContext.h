#ifndef NET_CONNECTION_ICONNECTIONCONTEXT_H
#define NET_CONNECTION_ICONNECTIONCONTEXT_H

#include <stdint.h>

class IConnectionContext {
   public:
    explicit IConnectionContext(uint32_t iIp, uint16_t iPort, int32_t iSocketFd, uint64_t lConnMs) : m_iIp(iIp), m_iPort(iPort), m_iSocketFd(iSocketFd), m_lConnMs(lConnMs) {
    }
    virtual ~IConnectionContext() {}

    uint32_t GetIp() { return m_iIp; }
    uint16_t GetPort() { return m_iPort; }
    int32_t GetSocketFd() { return m_iSocketFd; }
    uint64_t GetConnMs() { return m_lConnMs; }

   private:
    uint32_t m_iIp;
    uint16_t m_iPort;
    int32_t m_iSocketFd;
    uint64_t m_lConnMs;
};

#endif  // NET_CONNECTION_ICONNECTIONCONTEXT_H