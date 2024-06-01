#ifndef __EPOLL_H_
#define __EPOLL_H_

#include <arpa/inet.h>
#include <fcntl.h>       // 非阻塞
#include <netinet/in.h>  //socket addr
#include <stdint.h>
#include <sys/socket.h>  //create socket
#include <unistd.h>      //func close()

#include <iostream>

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

#ifdef OS_MAC
#include <sys/event.h>
#endif

class Epoll {
   public:
    explicit Epoll();
    virtual ~Epoll();

   public:
    int32_t GetEpollFd();
    int32_t CreateEpoll();
    int32_t AddEventToEpoll(int fd);

   private:
    int32_t _m_epoll_fd = -1;
};

#endif  // __EPOLL_H_