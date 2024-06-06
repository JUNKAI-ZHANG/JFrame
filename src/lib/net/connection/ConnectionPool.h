#ifndef NET_CONNECTION_CONNECTIONPOOL_H
#define NET_CONNECTION_CONNECTIONPOOL_H

#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "./INetConnection.h"

class ConnectionPool {
   public:
    explicit ConnectionPool() {
        m_hmapConn = new std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>();
    }
    virtual ~ConnectionPool() {
        if (m_hmapConn != nullptr) {
            delete m_hmapConn;
            m_hmapConn = nullptr;
        }
    }

    void AddConnection(INetConnection* pConn) {
        if (pConn == nullptr) {
            return;
        }
        m_hmapConn->insert(std::make_pair(pConn->GetSocketFd(), std::make_shared<INetConnection>(*pConn)));
    }

    void RemoveConnection(INetConnection* pConn) {
        if (pConn == nullptr) {
            return;
        }
        std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn->find(pConn->GetSocketFd());
        if (pConnIter == m_hmapConn->end()) {
            return;
        }
        m_hmapConn->erase(pConn->GetSocketFd());
    }

    INetConnection* GetConnection(uint32_t iSocketFd) {
        std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn->find(iSocketFd);
        if (pConnIter == m_hmapConn->end()) {
            return nullptr;
        }
        return pConnIter->second.get();
    }

   private:
    std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>* m_hmapConn = nullptr;
};

#endif  // NET_CONNECTION_CONNECTIONPOOL_H