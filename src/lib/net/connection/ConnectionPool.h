#ifndef NET_CONNECTION_CONNECTIONPOOL_H
#define NET_CONNECTION_CONNECTIONPOOL_H

#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "./INetConnection.h"

class ConnectionPool {
   public:
    explicit ConnectionPool() {
    }
    virtual ~ConnectionPool() {
    }

    void AddConnection(std::shared_ptr<INetConnection>& pConn) {
        m_hmapConn[pConn->GetSocketFd()] = pConn;
    }

    void RemoveConnection(std::shared_ptr<INetConnection>& pConn) {
        if (pConn == nullptr) {
            return;
        }
        std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn.find(pConn->GetSocketFd());
        if (pConnIter == m_hmapConn.end()) {
            return;
        }
        m_hmapConn.erase(pConn->GetSocketFd());
    }

    std::shared_ptr<INetConnection> GetConnection(uint32_t iSocketFd) {
        std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn.find(iSocketFd);
        if (pConnIter == m_hmapConn.end()) {
            return nullptr;
        }
        return pConnIter->second;
    }

   private:
    std::unordered_map<uint32_t, std::shared_ptr<INetConnection>> m_hmapConn;
};

#endif  // NET_CONNECTION_CONNECTIONPOOL_H