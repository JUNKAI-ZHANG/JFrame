#ifndef __EPOLL_H_
#define __EPOLL_H_

#include <arpa/inet.h>
#include <fcntl.h>       // 非阻塞
#include <netinet/in.h>  //socket addr
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>  //create socket
#include <unistd.h>      //func close()

#include <iostream>

#include "lib/net/define/err.h"

class NetEpoll {
   public:
    typedef NetError (*EpollEventAction)(int32_t, epoll_event, void*);

   public:
    explicit NetEpoll() {
    }
    virtual ~NetEpoll() {
        close(m_iEpollFd);
    }

   public:
    int32_t GetEpollFd() {
        return m_iEpollFd;
    }

    NetError CreateEpoll() {
        // Since Linux 2.6.8, the size argument is ignored, but must be greater than zero;
        m_iEpollFd = epoll_create(1);
        if (m_iEpollFd == -1) {
            std::cout << "Failed to create epoll instance" << std::endl;
            return NET_EPOLL_CREATE_ERR;
        }

        return NET_OK;
    }
    NetError EpollAdd(int fd) {
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
        event.data.fd = fd;

        if (epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
            std::cout << "Failed to add connection_fd to epoll instance" << std::endl;
            return NET_EPOLL_ADD_FD_ERR;
        }

        return NET_OK;
    }

    NetError EpollWait() {
        m_iEpollWaitEventNum = epoll_wait(m_iEpollFd, m_kEpollWaitEvents, ms_iMaxConn, 1);
        if (m_iEpollWaitEventNum == -1) {
            std::cout << "Epoll_wait Failed" << std::endl;
            close(m_iEpollFd);
            return NET_EPOLL_WAIT_ERR;
        }
    }

    NetError EpollHandleEvent(EpollEventAction kEpollEventAction) {
        NetError eErr = NetError::NET_OK;
        for (int32_t id = 0; id < m_iEpollWaitEventNum; ++id) {
            int32_t iConnFd = m_kEpollWaitEvents[id].data.fd;
            eErr = kEpollEventAction(iConnFd, m_kEpollWaitEvents[id], nullptr);
            if (eErr != NetError::NET_OK) {
                std::cout << "Handle event failed" << std::endl;
                return eErr;
            }
        }
        return eErr;
    }

   private:
    int32_t m_iEpollFd = -1;

    int32_t m_iEpollWaitEventNum = -1;
    static const int32_t ms_iMaxConn = 2048;
    struct epoll_event m_kEpollWaitEvents[2048];
};

#endif  // __EPOLL_H_