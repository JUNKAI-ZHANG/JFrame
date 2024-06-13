#include "ConnectionPool.h"

void ConnectionPool::AddConnection(std::shared_ptr<INetConnection> pConn) {
    m_hmapConn[pConn->GetSocketFd()] = pConn;
}

void ConnectionPool::RemoveConnection(std::shared_ptr<INetConnection> pConn) {
    if (pConn == nullptr) {
        return;
    }
    std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn.find(pConn->GetSocketFd());
    if (pConnIter == m_hmapConn.end()) {
        return;
    }
    m_hmapConn.erase(pConn->GetSocketFd());
}

std::shared_ptr<INetConnection> ConnectionPool::GetConnection(uint32_t iSocketFd) {
    std::unordered_map<uint32_t, std::shared_ptr<INetConnection>>::iterator pConnIter = m_hmapConn.find(iSocketFd);
    if (pConnIter == m_hmapConn.end()) {
        return nullptr;
    }
    return pConnIter->second;
}