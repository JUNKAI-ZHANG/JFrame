#ifndef NET_CONNECTION_CONNECTIONPOOL_H
#define NET_CONNECTION_CONNECTIONPOOL_H

#include <stdint.h>

#include <unordered_map>

#include "./INetConnection.h"

class ConnectionPool {
   public:
    void AddConnection(INetConnection* pConn) {
        if (pConn == nullptr) {
            return;
        }
        m_mapConn[pConn->GetSocketFd()] = pConn;
    }

    void RemoveConnection(INetConnection* pConn) {
        if (pConn == nullptr) {
            return;
        }
        m_mapConn.erase(pConn->GetSocketFd());
    }

    INetConnection* GetConnection(uint32_t iSocketFd) {
        auto pConnIter = m_mapConn.find(iSocketFd);
        if (pConnIter == m_mapConn.end()) {
            return nullptr;
        }
        return pConnIter->second;
    }

   private:
    std::unordered_map<uint32_t, INetConnection*> m_mapConn;
};

#endif  // NET_CONNECTION_CONNECTIONPOOL_H