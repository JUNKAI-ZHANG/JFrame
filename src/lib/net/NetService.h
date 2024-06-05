#ifndef NET_NETSERVICE_H
#define NET_NETSERVICE_H

#include <stdint.h>

#include <unordered_map>

#include "connection/ConnectionPool.h"
#include "define/err.h"
#include "epoll/epoll.h"
#include "lib/log/log.h"
#include "socket/socket.h"

class NetService {
   public:
    explicit NetService();
    virtual ~NetService();
    NetError Working(int32_t iPort);

   protected:
    virtual NetError CloseFd(int32_t iConnFd);
    virtual NetError HandleNewConnecionEvent();
    virtual NetError HandleConnMsgEvent(int32_t iConnFd);
    virtual NetError HandleReceivedMsg(int32_t iConnFd);

   protected:
    NetError DoTick();
    NetError HandleEpollEvent(int32_t iConnFd, epoll_event kEvent, void* pData);

   protected:
    NetEpoll* m_kNetEpoll;
    NetSocket* m_kNetSocket;
    ConnectionPool* m_kConnPool;
};

#endif  // NET_NETSERVICE_H