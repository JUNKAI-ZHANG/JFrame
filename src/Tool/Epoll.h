#ifndef __EPOLL_H_
#define __EPOLL_H_

#include <iostream>
#include <sys/socket.h> //create socket
#include <netinet/in.h> //socket addr
#include <sys/epoll.h>  //epoll
#include <unistd.h>     //func close()
#include <fcntl.h>      // 非阻塞
#include <stdint.h>
#include <arpa/inet.h>
    

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

#endif // __EPOLL_H_