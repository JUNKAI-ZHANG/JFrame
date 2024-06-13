#ifndef NET_CONNECTION_ICONNECTIONCONTEXT_H
#define NET_CONNECTION_ICONNECTIONCONTEXT_H

#include <stdint.h>

#include "lib/net/define/NetDefine.h"

class IConnectionContext {
   public:
    explicit IConnectionContext() {
    }
    virtual ~IConnectionContext() {}

    uint32_t GetIp() { return m_iIp; }
    uint16_t GetPort() { return m_iPort; }
    int32_t GetSocketFd() { return m_iSocketFd; }
    uint64_t GetConnMs() { return m_lConnMs; }

   public:
    uint32_t m_iIp;
    uint16_t m_iPort;
    int32_t m_iSocketFd;
    uint64_t m_lConnMs;

    ServiceType m_eLocalType;
    ServiceType m_ePeerType;
};

class CSConnectionContext : public IConnectionContext {
   public:
    uint64_t m_lPlayerUid;
    uint32_t m_iAreaId;
};

class SSConnectionContext : public IConnectionContext {
};

#endif  // NET_CONNECTION_ICONNECTIONCONTEXT_H