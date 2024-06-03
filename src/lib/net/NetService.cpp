#ifndef __COMMONSERVER_CPP_
#define __COMMONSERVER_CPP_

#include "./NetService.h"

#include <string.h>

#include <iostream>

class NetService;

NetService::NetService() {
    m_kNetEpoll = new NetEpoll();
    m_kNetSocket = new NetSocket();
    m_kConnPool = new ConnectionPool();
}

NetService::~NetService() {
}

int32_t NetService::CloseFd(int32_t fd) {
    if (fd == _m_socket_fd) {
    } else {
    }
    close(fd);
    return 1;
}

NetError NetService::Working(int32_t port) {
    NetError eErr = NetError::NET_OK;
    eErr = m_kNetSocket->Init();
    if (eErr != NetError::NET_OK) {
        std::cout << "Init Failed" << std::endl;
        return eErr;
    }

    eErr = m_kNetSocket->Bind(0, port);
    if (eErr != NetError::NET_OK) {
        std::cout << "Bind Failed" << std::endl;
        return eErr;
    }

    eErr = m_kNetSocket->Listen();
    if (eErr != NetError::NET_OK) {
        std::cout << "Listen Failed" << std::endl;
        return eErr;
    }

    eErr = m_kNetEpoll->CreateEpoll();
    if (eErr != NetError::NET_OK) {
        std::cout << "CreateEpoll Failed" << std::endl;
        m_kNetSocket->Close();
        return eErr;
    }

    eErr = m_kNetEpoll->AddEventToEpoll(m_kNetSocket->GetSocketFd());
    if (eErr != NetError::NET_OK) {
        std::cout << "AddEventToEpoll Failed" << std::endl;
        m_kNetSocket->Close();
        return eErr;
    }

    while (true) {
        eErr = m_kNetEpoll->EpollWait();
        if (eErr != NetError::NET_OK) {
            std::cout << "EpollWait Failed" << std::endl;
            return eErr;
        }

        eErr = m_kNetEpoll->EpollHandleEvent(std::bind(&NetService::HandleEpollEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        if (eErr != NetError::NET_OK) {
            std::cout << "EpollHandleEvent Failed" << std::endl;
            return eErr;
        }

        this->DoTick();
    }
}

NetError NetService::HandleEpollEvent(int32_t iConnFd, epoll_event kEvent, void *pData) {
    NetError eErr = NetError::NET_OK;
    if (kEvent.events & EPOLLIN) {
        if (iConnFd == m_kNetSocket->GetSocketFd()) {
            eErr = HandleNewConnecionEvent(iConnFd);
            if (eErr != NetError::NET_OK) {
                std::cout << "handleListenerEvent Failed" << std::endl;
                return eErr;
            }
        } else {
            eErr = HandleConnMsgEvent(iConnFd);
            if (eErr != NetError::NET_OK) {
                std::cout << "handleConnEvent" << std::endl;
                return eErr;
            }
        }
    }
    if (kEvent.events & EPOLLERR) {
        eErr = CloseFd(iConnFd);
        if (eErr != NetError::NET_OK) {
            std::cout << "CloseFd(EPOLLERR) Failed" << std::endl;
            return eErr;
        }
    } else if (kEvent.events & EPOLLHUP) {
        eErr = CloseFd(iConnFd);
        if (eErr != NetError::NET_OK) {
            std::cout << "CloseFd(EPOLLHUP) Failed" << std::endl;
            return eErr;
        }
    }
    return NetError::NET_OK;
}

void NetService::DoTick() {}

NetError NetService::HandleNewConnecionEvent(int32_t iConnFd) {
    NetError eErr = NetError::NET_OK;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int32_t accept_conn_fd = accept(_m_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);

    if (accept_conn_fd == -1) {
        std::cout << "Failed to accept connection" << std::endl;
        return NetError::NET_ERR;
    }

    std::cout << "Accepted connection from " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;

    if (m_kNetEpoll->EpollAdd(accept_conn_fd) == -1) {
        CloseFd(accept_conn_fd);
        return NetError::NET_ERR;
    }
    // Todo : 添加fd对应的buffer
    return NetError::NET_OK;
}

NetError NetService::HandleConnMsgEvent(int32_t fd) {
    ssize_t body_size = 0;
    ssize_t tmp_received;

    // check connection state
    if (m_umapConnsBuffer.find(fd) == m_umapConnsBuffer.end()) {
        std::cout << "Cannot find client buffer, Did the socket be closed" << std::endl;
        return NetError::NET_ERR;
    }

    // Receive data from client
    uint8_t tmp[2048];
    while ((tmp_received = recv(fd, tmp, 2048, MSG_DONTWAIT)) > 0) {
        if (!m_umapConnsBuffer[fd]->AddBuffer(tmp, tmp_received)) {
            std::cout << "ServerBase : Client Read Buffer is Full" << std::endl;
            tmp_received = -1;
            break;
        }
    }

    if (tmp_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // 数据读取完毕
            HandleReceivedMsg(fd);
        } else {
            std::cout << "ServerBase : Failed to read from connection" << std::endl;
            CloseFd(fd);
            return NetError::NET_ERR;
        }
    } else if (tmp_received == 0) {
        std::cout << "Connection closed by remote host" << std::endl;
        CloseFd(fd);
        return NetError::NET_ERR;
    }
    return NetError::NET_OK;
}

NetError NetService::HandleReceivedMsg(int32_t fd) {
    // Todo : 处理接收到的消息
    return NetError::NET_OK;
}

#endif  // __COMMONSERVER_CPP_