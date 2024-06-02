#ifndef __EPOLL_H_
#define __EPOLL_H_

#include <arpa/inet.h>
#include <fcntl.h>       // 非阻塞
#include <netinet/in.h>  //socket addr
#include <stdint.h>
#include <sys/socket.h>  //create socket
#include <unistd.h>      //func close()
#include <sys/epoll.h>
#include <iostream>

class Epoll {
   public:
    explicit Epoll() {

    }
    virtual ~Epoll() {
        close(_m_epoll_fd);
    }

   public:
    int32_t GetEpollFd() {
        return _m_epoll_fd;
    }
    int32_t CreateEpoll() {
        // Since Linux 2.6.8, the size argument is ignored, but must be greater than zero;
        _m_epoll_fd = epoll_create(1);
        if (_m_epoll_fd == -1) {
            std::cout << "Failed to create epoll instance" << std::endl;
            return -1;
        }

        return 1;
    }
    int32_t AddEventToEpoll(int fd) {
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
        event.data.fd = fd;

        if (epoll_ctl(_m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
            std::cout << "Failed to add connection_fd to epoll instance" << std::endl;
            return -1;
        }

        return 1;
    }

   private:
    int32_t _m_epoll_fd = -1;
};

#endif  // __EPOLL_H_