#ifndef __COMMONSERVER_H_
#define __COMMONSERVER_H_

#include <unordered_map>

#include "../../Tool/Epoll.h"
#include "../CommonLib/RingBuffer.h"
#include "../Common_Define.h"

class IServer {
   protected:
    virtual int32_t StartListen(int32_t port) = 0;
    virtual int32_t CloseFd(int32_t fd) = 0;
    virtual int32_t Working(int32_t port) = 0;
    virtual void DoTick() = 0;
    virtual int32_t HandleNewConnecionEvent(int32_t fd) = 0;
    virtual int32_t HandleConnMsgEvent(int32_t fd) = 0;

   protected:
    std::unordered_map<int32_t, RingBuffer *> _m_conns_buffer;
};

class CommonServer : public IServer {
   public:
    explicit CommonServer();
    virtual ~CommonServer();

   public:
    virtual int32_t StartListen(int32_t port) override {};
    virtual int32_t CloseFd(int32_t fd) override {};
    virtual int32_t Working(int32_t port) override {};
    virtual void DoTick() override {};

   private:
    virtual int32_t HandleNewConnecionEvent(int32_t fd) override;
    virtual int32_t HandleConnMsgEvent(int32_t fd) override;

   private:
    Epoll *_m_epoll;
    int32_t _m_socket_fd;

    char *_m_buffer;
    struct epoll_event *_m_events;
};

#endif  // __COMMONSERVER_H_