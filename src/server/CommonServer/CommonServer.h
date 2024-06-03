#ifndef __COMMONSERVER_H_
#define __COMMONSERVER_H_

#include <unordered_map>

#include "../../lib/net/epoll/epoll.h"
#include "Common_Define.h"
#include "lib/net/define/err.h"

class IBaseServer {
   public:
    virtual int32_t Working(int32_t port) = 0;

   protected:
    virtual int32_t StartListen(int32_t port) = 0;
    virtual int32_t CloseFd(int32_t fd) = 0;
    virtual void DoTick() = 0;
    virtual NetError HandleNewConnecionEvent(int32_t fd) = 0;
    virtual NetError HandleConnMsgEvent(int32_t fd) = 0;
    virtual NetError HandleReceivedMsg(int32_t fd) = 0;

   protected:
    std::unordered_map<int32_t, RingBuffer *> m_umapConnsBuffer;
};

class CommonServer : public IBaseServer {
   public:
    explicit CommonServer();
    virtual ~CommonServer();

    virtual int32_t Working(int32_t port) override;

   protected:
    virtual int32_t StartListen(int32_t port) override;
    virtual int32_t CloseFd(int32_t fd) override;
    virtual void DoTick() override;
    virtual NetError HandleNewConnecionEvent(int32_t fd) override;
    virtual NetError HandleConnMsgEvent(int32_t fd) override;
    virtual NetError HandleReceivedMsg(int32_t fd) override;

   private:
    Epoll *_m_epoll;
    int32_t _m_socket_fd;

    char *_m_buffer;
};

#endif  // __COMMONSERVER_H_