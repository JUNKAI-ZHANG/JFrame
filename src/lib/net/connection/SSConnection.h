#ifndef NET_CONNECTION_SSCONNECTION_H
#define NET_CONNECTION_SSCONNECTION_H

#include "./INetConnection.h"

class SSConnection : public INetConnection {
   public:
    SSConnection(uint32_t iIp, uint16_t iPort, uint32_t iSocketFd, uint64_t lConnMs)
        : INetConnection(iIp, iPort, iSocketFd, lConnMs) {
    }

    virtual ~SSConnection() {}

    virtual void SendMsg(const char* pMsg, uint32_t iLen) {
        // TODO
    }

    virtual void Close() {
        // TODO
    }
};

#endif  // NET_CONNECTION_SSCONNECTION_H