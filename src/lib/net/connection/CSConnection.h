#ifndef NET_CONNECTION_CSCONNECTION_H
#define NET_CONNECTION_CSCONNECTION_H

#include "./INetConnection.h"

class CSConnection : public INetConnection {
   public:
    CSConnection(uint32_t iIp, uint16_t iPort, uint32_t iSocketFd, uint64_t lConnMs)
        : INetConnection(iIp, iPort, iSocketFd, lConnMs) {
    }

    virtual ~CSConnection() {}

    virtual void SendMsg(const char* pMsg, uint32_t iLen) {
        // TODO
    }

    virtual void Close() {
        // TODO
    }
};

#endif  // NET_CONNECTION_CSCONNECTION_H