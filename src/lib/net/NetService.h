#ifndef NET_NETSERVICE_H
#define NET_NETSERVICE_H

#include <stdint.h>

#include <unordered_map>

#include "connection/ConnectionPool.h"
#include "define/err.h"
#include "epoll/epoll.h"
#include "socket/socket.h"

class NetService {
   public:
    int32_t Working(int32_t port);

   protected:
    virtual int32_t CloseFd(int32_t fd);
    virtual void DoTick() = 0;
    virtual NetError HandleNewConnecionEvent(int32_t fd);
    virtual NetError HandleConnMsgEvent(int32_t fd);
    virtual NetError HandleReceivedMsg(int32_t fd);

   protected:
    NetEpoll* m_kNetEpoll;
    NetSocket* m_kNetSocket;
    ConnectionPool* m_kConnPool;
};

#endif  // NET_NETSERVICE_H