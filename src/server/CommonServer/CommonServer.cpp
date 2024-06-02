#ifndef __COMMONSERVER_CPP_
#define __COMMONSERVER_CPP_

#include "./CommonServer.h"

#include <memory.h>
#include <iostream>

class CommonServer;

CommonServer::CommonServer() {
    _m_epoll = new Epoll();
    _m_buffer = new char[1042876];
    _m_events = new epoll_event();
}

CommonServer::~CommonServer() {
    delete _m_epoll;
    delete[] _m_buffer;
    delete[] _m_events;
}

int32_t CommonServer::StartListen(int32_t port) {
    _m_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (_m_socket_fd == -1) {
        std::cout << "Failed to create socket" << std::endl;
        return -1;
    }

    int32_t yes = 1;
    if (setsockopt(_m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int32_t)) == -1) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(_m_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) == -1) {
        std::cout << "Failed to bind address" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }

    if (listen(_m_socket_fd, SOMAXCONN) == -1) {
        std::cout << "Failed to listen on socket" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }

    return 1;
}

int32_t CommonServer::CloseFd(int32_t fd) {
    if (fd == _m_socket_fd) {
    } else {
    }
    close(fd);
    return 1;
}

int32_t CommonServer::Working(int32_t port) {
    if (StartListen(port) == -1) {
        std::cout << "StartListen Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    if (_m_epoll->CreateEpoll() == -1) {
        std::cout << "CreateEpoll Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    if (_m_epoll->AddEventToEpoll(_m_socket_fd) == -1) {
        std::cout << "AddEventToEpoll Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    while (true) {
        int32_t _conns = epoll_wait(_m_epoll->GetEpollFd(), _m_events, MAX_CONN, EPOLL_WAIT);
        if (_conns == -1) {
            std::cout << "Epoll_wait Failed" << std::endl;
            CloseFd(_m_epoll->GetEpollFd());
            return -1;
        }
        for (int32_t id = 0; id < _conns; ++id) {
            int32_t conn_fd = _m_events[id].data.fd;
            if (_m_events[id].events & EPOLLIN) {
                if (conn_fd == _m_socket_fd) {
                    if (HandleNewConnecionEvent(conn_fd) != NetError::NET_OK) {
                        std::cout << "handleListenerEvent Failed" << std::endl;
                        return -1;
                    }
                } else {
                    if (HandleConnMsgEvent(conn_fd) != NetError::NET_OK) {
                        std::cout << "handleConnEvent" << std::endl;
                        return -1;
                    }
                }
            }
            if (_m_events[id].events & EPOLLERR) {
                CloseFd(conn_fd);
            }
            if (_m_events[id].events & EPOLLHUP) {
                CloseFd(conn_fd);
            }
        }
    }
}

void CommonServer::DoTick() {
}

NetError CommonServer::HandleNewConnecionEvent(int32_t fd) {
    if (_m_socket_fd != fd) {
        std::cout << "Fd Error" << std::endl;
        return NetError::NET_ERR;
    }
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int32_t accept_conn_fd = accept(_m_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);

    if (accept_conn_fd == -1) {
        std::cout << "Failed to accept connection" << std::endl;
        return NetError::NET_ERR;
    }

    std::cout << "Accepted connection from " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;

    if (_m_epoll->AddEventToEpoll(accept_conn_fd) == -1) {
        CloseFd(accept_conn_fd);
        return NetError::NET_ERR;
    }
    // Todo : 添加fd对应的buffer
    return NetError::NET_OK;
}

NetError CommonServer::HandleConnMsgEvent(int32_t fd) {
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

NetError CommonServer::HandleReceivedMsg(int32_t fd) {
    // Todo : 处理接收到的消息
    return NetError::NET_OK;
}

#endif  // __COMMONSERVER_CPP_