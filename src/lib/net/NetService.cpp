#ifndef __COMMONSERVER_CPP_
#define __COMMONSERVER_CPP_

#include "./NetService.h"

#include <string.h>

#include <functional>
#include <iostream>

class NetService;

NetService::NetService() {
    m_kNetEpoll = new NetEpoll();
    m_kNetSocket = new NetSocket();
    m_kConnPool = new ConnectionPool();
}

NetService::~NetService() {
}

NetError NetService::CloseFd(int32_t iConnFd) {
    NetError eErr = NetError::NET_OK;
    if (iConnFd == m_kNetSocket->GetSocketFd()) {
    } else {
        INetConnection* pConn = m_kConnPool->GetConnection(iConnFd);
        if (pConn == nullptr) {
            return NetError::NET_CONN_NOT_EXIST_ERR;
        }
        pConn->Close();
    }
    close(iConnFd);
    return eErr;
}

NetError NetService::Working(int32_t iPort) {
    NetError eErr = NetError::NET_OK;
    eErr = m_kNetSocket->Init();
    if (eErr != NetError::NET_OK) {
        std::cout << "Init Failed" << std::endl;
        return eErr;
    }

    eErr = m_kNetSocket->Bind(0, iPort);
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

    eErr = m_kNetEpoll->EpollAdd(m_kNetSocket->GetSocketFd());
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

        eErr = m_kNetEpoll->EpollHandleEvent(std::bind(&NetService::HandleEpollEvent,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3));
        if (eErr != NetError::NET_OK) {
            std::cout << "EpollHandleEvent Failed" << std::endl;
            return eErr;
        }

        this->DoTick();
    }
    return eErr;
}

NetError NetService::HandleEpollEvent(int32_t iConnFd, epoll_event kEvent, void* pData) {
    NetError eErr = NetError::NET_OK;
    if (kEvent.events & EPOLLIN) {
        if (iConnFd == m_kNetSocket->GetSocketFd()) {
            eErr = HandleNewConnecionEvent();
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
    return eErr;
}

NetError NetService::DoTick() {
    // 控制频率
    std::cout << "DoTick" << std::endl;
    return NetError::NET_OK;
}

NetError NetService::HandleNewConnecionEvent() {
    NetError eErr = NetError::NET_OK;
    int32_t iConnFd = 0;

    if ((eErr = m_kNetSocket->Accept(iConnFd)) != NetError::NET_OK) {
        return eErr;
    }

    if ((eErr = m_kNetEpoll->EpollAdd(iConnFd)) != NetError::NET_OK) {
        CloseFd(iConnFd);
        return NetError::NET_ERR;
    }
    // Todo : 添加进连接池
    return eErr;
}

NetError NetService::HandleConnMsgEvent(int32_t iConnFd) {
    NetError eErr = NetError::NET_OK;
    if (m_kConnPool == nullptr) {
        return NetError::NET_CONN_POOL_NULLPTR_ERR;
    }

    INetConnection* pConn = m_kConnPool->GetConnection(iConnFd);
    if (pConn == nullptr) {
        return NetError::NET_CONN_NOT_EXIST_ERR;
    }

    if (pConn->GetRecvBuffer() == nullptr) {
        std::cout << "Conn buffer is nullptr : " << pConn->ToString() << std::endl;
        return NetError::NET_CONN_RECV_BUFFER_NULLPTR_ERR;
    }

    // Receive data
    uint8_t tmp[2048];
    ssize_t tmp_received = 0;
    while ((tmp_received = recv(iConnFd, tmp, 2048, MSG_DONTWAIT)) > 0) {
        if (!pConn->GetRecvBuffer()->AddBuffer(tmp, tmp_received)) {
            std::cout << "ServerBase : Client Read Buffer is Full" << std::endl;
            tmp_received = -1;
            break;
        }
    }

    if (tmp_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // 数据读取完毕
            HandleReceivedMsg(iConnFd);
        } else {
            std::cout << "ServerBase : Failed to read from connection" << std::endl;
            CloseFd(iConnFd);
            return NetError::NET_ERR;
        }
    } else if (tmp_received == 0) {
        std::cout << "Connection closed by remote host" << std::endl;
        CloseFd(iConnFd);
        return NetError::NET_ERR;
    }
    return eErr;
}

NetError NetService::HandleReceivedMsg(int32_t fd) {
    // Todo : 处理接收到的消息
    return NetError::NET_OK;
}

#endif  // __COMMONSERVER_CPP_