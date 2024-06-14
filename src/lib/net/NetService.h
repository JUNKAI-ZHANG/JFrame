#ifndef NET_NETSERVICE_H
#define NET_NETSERVICE_H

#include <stdint.h>

#include <unordered_map>

#include "lib/log/log.h"
#include "lib/net/connection/ConnectionPool.h"
#include "lib/net/define/err.h"
#include "lib/net/epoll/epoll.h"
#include "lib/net/manager/NetMessageMgr.h"
#include "lib/net/socket/socket.h"
#include "lib/util/Singleton.h"

class NetMessage;
class NetMessageMgr;
class ConnectionPool;

class NetService {
   public:
    explicit NetService();
    virtual ~NetService();
    NetError Working(int32_t iPort);
    void SendMsg(std::unique_ptr<NetMessage> pNetMessage);

   public:
    std::shared_ptr<NetEpoll> GetNetEpoll() { return m_pNetEpoll; }
    std::shared_ptr<NetSocket> GetNetSocket() { return m_pNetSocket; }
    std::shared_ptr<ConnectionPool> GetConnectionPool() { return m_pConnPool; }
    std::shared_ptr<NetMessageMgr> GetNetMessageMgr() { return m_pNetMessageMgr; }

   protected:
    virtual NetError CloseFd(int32_t iConnFd);
    virtual NetError HandleNewConnecionEvent();
    virtual NetError HandleConnMsgEvent(int32_t iConnFd);
    virtual NetError HandleReceivedMsg(int32_t iConnFd);
    virtual NetError ProcessNetSendMessage();

   protected:
    NetError DoTick();
    NetError HandleEpollEvent(int32_t iConnFd, epoll_event kEvent, void* pData);

   protected:
    std::shared_ptr<NetEpoll> m_pNetEpoll;
    std::shared_ptr<NetSocket> m_pNetSocket;
    std::shared_ptr<ConnectionPool> m_pConnPool;
    std::shared_ptr<NetMessageMgr> m_pNetMessageMgr;
};

#define NET_SERVICE Singleton<NetService>::Instance()

#endif  // NET_NETSERVICE_H