#ifndef __COMMONSERVER_CPP_
#define __COMMONSERVER_CPP_

#include "CommonServer.h"

#include <memory.h>

class CommonServer;

CommonServer::CommonServer() {
    _m_epoll = new Epoll();
    _m_buffer = new char[MAX_BUFFER_SIZE];
    _m_events = new epoll_event[MAX_CONN];
}

CommonServer::~CommonServer() {
    delete _m_epoll;
    delete[] _m_buffer;
    delete[] _m_events;
}

int32_t CommonServer::StartListen(int32_t port) {
    _m_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (_m_socket_fd == -1) {
        std::cerr << "Failed to create socket" << std::endl;
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
        std::cerr << "Failed to bind address" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }

    if (listen(_m_socket_fd, SOMAXCONN) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
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
        std::cerr << "StartListen Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    if (_m_epoll->CreateEpoll() == -1) {
        std::cerr << "CreateEpoll Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    if (_m_epoll->AddEventToEpoll(_m_socket_fd) == -1) {
        std::cerr << "AddEventToEpoll Failed" << std::endl;
        CloseFd(_m_socket_fd);
        return -1;
    }
    while (true) {
        int32_t _conns = epoll_wait(_m_epoll->GetEpollFd(), _m_events, MAX_CONN, EPOLL_WAIT);
        if (_conns == -1) {
            std::cerr << "Epoll_wait Failed" << std::endl;
            CloseFd(_m_epoll->GetEpollFd());
            return -1;
        }
        for (int32_t id = 0; id < _conns; ++id) {
            int32_t conn_fd = _m_events[id].data.fd;
            if (_m_events[id].events & EPOLLIN) {
                if (conn_fd == _m_socket_fd) {
                    if (handleNewConnecionEvent(conn_fd) == -1) {
                        std::cerr << "handleListenerEvent Failed" << std::endl;
                        return -1;
                    }
                } else {
                    if (handleConnMsgEvent(conn_fd) == -1) {
                        std::cerr << "handleConnEvent" << std::endl;
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

int32_t CommonServer::handleNewConnecionEvent(int32_t fd) {
    if (_m_socket_fd != fd) {
        std::cerr << "Fd Error" << std::endl;
        return -1;
    }
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int32_t accept_conn_fd = accept(_m_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), &addrlen);

    if (accept_conn_fd == -1) {
        std::cerr << "Failed to accept connection" << std::endl;
        return -1;
    }

    std::cout << "Accepted connection from " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << std::endl;

    if (_m_epoll->AddEventToEpoll(accept_conn_fd) == -1) {
        CloseFd(accept_conn_fd);
        return -1;
    }
    // Todo : 添加fd对应的buffer
    return 1;
}

int32_t CommonServer::handleConnMsgEvent(int32_t fd) {
    ssize_t body_size = 0;
    ssize_t tmp_received;

    // check connection state
    if (conn.find(conn_fd) == conn.end()) {
        std::cerr << "Cannot find client buffer, Did the socket be closed" << std::endl;
        return;
    }

    // Receive data from client
    uint8_t tmp[TMP_BUFFER_SIZE];
    while ((tmp_received = recv(conn_fd, tmp, JSON.TMP_BUFFER_SIZE, MSG_DONTWAIT)) > 0) {
        if (!conn[conn_fd]->AddBuffer(tmp, tmp_received)) {
            std::cerr << "ServerBase : Client Read Buffer is Full" << std::endl;
            tmp_received = -1;
            break;
        }
    }

    if (tmp_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // 数据读取完毕
            HandleReceivedMsg(conn[conn_fd], conn_fd);
        } else {
            std::cerr << "ServerBase : Failed to read from connection" << std::endl;
            CloseClientSocket(conn_fd);
            return;
        }
    } else if (tmp_received == 0) {
        std::cout << "Connection closed by remote host" << std::endl;
        CloseClientSocket(conn_fd);
        return;
    }
}

#endif  // __COMMONSERVER_CPP_