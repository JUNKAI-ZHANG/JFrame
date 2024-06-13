#ifndef NET_CONNECTION_CONNECTIONPOOL_H
#define NET_CONNECTION_CONNECTIONPOOL_H

#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "lib/net/connection/INetConnection.h"

class INetConnection;

class ConnectionPool {
   public:
    explicit ConnectionPool() {
    }
    virtual ~ConnectionPool() {
    }

    void AddConnection(std::shared_ptr<INetConnection> pConn);

    void RemoveConnection(std::shared_ptr<INetConnection> pConn);

    std::shared_ptr<INetConnection> GetConnection(uint32_t iSocketFd);

   private:
    std::unordered_map<uint32_t, std::shared_ptr<INetConnection>> m_hmapConn;
};

#endif  // NET_CONNECTION_CONNECTIONPOOL_H