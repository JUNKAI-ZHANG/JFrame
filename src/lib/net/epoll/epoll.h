#ifndef __EPOLL_H_
#define __EPOLL_H_

#include <arpa/inet.h>
#include <fcntl.h>       // 非阻塞
#include <netinet/in.h>  //socket addr
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>  //create socket
#include <unistd.h>      //func close()

#include <functional>
#include <iostream>

#include "lib/log/log.h"
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
        if ((m_iEpollFd = epoll_create(1)) == -1) {
            LogError("{module:NetEpoll}", "Failed to create epoll instance");
            return NetError::NET_EPOLL_CREATE_ERR;
        }

        return NetError::NET_OK;
    }
    NetError EpollAdd(int iEvnetFd) {
        struct epoll_event event;
        event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
        event.data.fd = iEvnetFd;

        if (epoll_ctl(m_iEpollFd, EPOLL_CTL_ADD, iEvnetFd, &event) == -1) {
            LogError("{module:NetEpoll}", "Failed to add connection_fd to epoll instance");
            return NetError::NET_EPOLL_ADD_FD_ERR;
        }

        return NetError::NET_OK;
    }

    NetError EpollDel(int iEvnetFd) {
        if (epoll_ctl(m_iEpollFd, EPOLL_CTL_DEL, iEvnetFd, nullptr) == -1) {
            LogError("{module:NetEpoll}", "Failed to delete connection_fd from epoll instance");
            return NetError::NET_EPOLL_DEL_FD_ERR;
        }

        return NetError::NET_OK;
    }

    NetError EpollWait() {
        if ((m_iEpollWaitEventNum = epoll_wait(m_iEpollFd, m_kEpollWaitEvents, ms_iMaxConn, 1)) == -1) {
            LogError("{module:NetEpoll}", "Failed to wait for events");
            close(m_iEpollFd);
            return NET_EPOLL_WAIT_ERR;
        }
        return NetError::NET_OK;
    }

    NetError EpollHandleEvent(std::function<NetError(int32_t, epoll_event, void*)> kEpollEventAction) {
        NetError eErr = NetError::NET_OK;
        for (int32_t id = 0; id < m_iEpollWaitEventNum; ++id) {
            int32_t iConnFd = m_kEpollWaitEvents[id].data.fd;
            if ((eErr = kEpollEventAction(iConnFd, m_kEpollWaitEvents[id], nullptr)) != NetError::NET_OK) {
                LogError("{module:NetEpoll}", "Handle event failed");
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