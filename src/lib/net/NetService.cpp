#ifndef __COMMONSERVER_CPP_
#define __COMMONSERVER_CPP_

#include "./NetService.h"

#include <string.h>

#include <functional>
#include <iostream>

#include "./message/MessageHead.h"
#include "proto/msg_cs_common.pb.h"

class NetService;

NetService::NetService() {
    m_kNetEpoll = new NetEpoll();
    m_kNetSocket = new NetSocket();
    m_kConnPool = new ConnectionPool();
    m_kNetMessageMgr = new NetMessageMgr();
}

NetService::~NetService() {
    delete m_kNetEpoll;
    delete m_kNetSocket;
    delete m_kConnPool;
    delete m_kNetMessageMgr;
}

NetError NetService::CloseFd(int32_t iConnFd) {
    NetError eErr = NetError::NET_OK;
    if (iConnFd == m_kNetSocket->GetSocketFd()) {
    } else {
        std::shared_ptr<INetConnection> pConn = m_kConnPool->GetConnection(iConnFd);
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

    if ((eErr = m_kNetSocket->Init()) != NetError::NET_OK) {
        LogError("{module:NetService}", "Init Failed");
        return eErr;
    }

    if ((eErr = m_kNetSocket->Bind(0, iPort)) != NetError::NET_OK) {
        LogError("{module:NetService}", "Bind Failed");
        return eErr;
    }

    if ((eErr = m_kNetSocket->Listen()) != NetError::NET_OK) {
        LogError("{module:NetService}", "Listen Failed");
        return eErr;
    }

    if ((eErr = m_kNetEpoll->CreateEpoll()) != NetError::NET_OK) {
        LogError("{module:NetService}", "CreateEpoll Failed");
        m_kNetSocket->Close();
        return eErr;
    }

    if ((eErr = m_kNetEpoll->EpollAdd(m_kNetSocket->GetSocketFd())) != NetError::NET_OK) {
        LogError("{module:NetService}", "AddEventToEpoll Failed");
        m_kNetSocket->Close();
        return eErr;
    }

    while (true) {
        if ((eErr = m_kNetEpoll->EpollWait()) != NetError::NET_OK) {
            LogError("{module:NetService}", "EpollWait Failed");
            return eErr;
        }

        eErr = m_kNetEpoll->EpollHandleEvent(std::bind(&NetService::HandleEpollEvent,
                                                       this,
                                                       std::placeholders::_1,
                                                       std::placeholders::_2,
                                                       std::placeholders::_3));
        if (eErr != NetError::NET_OK) {
            LogError("{module:NetService}", "EpollHandleEvent Failed");
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
            if ((eErr = HandleNewConnecionEvent()) != NetError::NET_OK) {
                LogError("{module:NetService}", "HandleNewConnecionEvent Failed");
                return eErr;
            }
        } else {
            if ((eErr = HandleConnMsgEvent(iConnFd)) != NetError::NET_OK) {
                LogError("{module:NetService}", "HandleConnMsgEvent Failed", eErr);
                return eErr;
            }
        }
    }
    if (kEvent.events & EPOLLERR) {
        if ((eErr = CloseFd(iConnFd)) != NetError::NET_OK) {
            LogError("{module:NetService}", "CloseFd(EPOLLERR) Failed");
            return eErr;
        }
    } else if (kEvent.events & EPOLLHUP) {
        if ((eErr = CloseFd(iConnFd)) != NetError::NET_OK) {
            LogError("{module:NetService}", "CloseFd(EPOLLHUP) Failed");
            return eErr;
        }
    }
    return eErr;
}

NetError NetService::DoTick() {
    // 控制频率
    // LogInfo("{module:NetService}", "Now time is :", TimeUtil.GetNowS());
    this->ProcessNetSendMessage();
    return NetError::NET_OK;
}

NetError NetService::HandleNewConnecionEvent() {
    NetError eErr = NetError::NET_OK;
    int32_t iConnFd = 0;
    std::unique_ptr<IConnectionContext> pConnCtx = std::make_unique<IConnectionContext>();

    if ((eErr = m_kNetSocket->Accept(iConnFd, *pConnCtx)) != NetError::NET_OK) {
        return eErr;
    }

    if ((eErr = m_kNetEpoll->EpollAdd(iConnFd)) != NetError::NET_OK) {
        CloseFd(iConnFd);
        return NetError::NET_ERR;
    }
    if (m_kConnPool == nullptr) {
        LogError("{module:NetService}", "ConnPool is nullptr");
        return NetError::NET_CONN_POOL_NULLPTR_ERR;
    }
    // Todo : 添加进连接池
    std::shared_ptr<INetConnection> pConn = std::make_shared<INetConnection>(std::move(pConnCtx));
    m_kConnPool->AddConnection(pConn);

    LogInfo("{module:NetService}", "Accepted connection");

    return eErr;
}

NetError NetService::HandleConnMsgEvent(int32_t iConnFd) {
    LogInfo("recv message");
    NetError eErr = NetError::NET_OK;
    if (m_kConnPool == nullptr) {
        LogError("{module:NetService}", "ConnPool is nullptr");
        return NetError::NET_CONN_POOL_NULLPTR_ERR;
    }

    std::shared_ptr<INetConnection> pConn = m_kConnPool->GetConnection(iConnFd);
    if (pConn == nullptr) {
        LogError("{module:NetService}", "Conn is nullptr");
        return NetError::NET_CONN_NOT_EXIST_ERR;
    }

    if (pConn->GetRecvBuffer() == nullptr) {
        LogError("{module:NetService}", "Conn buffer is nullptr :", pConn->ToString());
        return NetError::NET_CONN_RECV_BUFFER_NULLPTR_ERR;
    }

    // Receive data
    char tmp[2048];
    ssize_t tmp_received = 0;
    while ((tmp_received = recv(iConnFd, tmp, 2048, MSG_DONTWAIT)) > 0) {
        if (!pConn->GetRecvBuffer()->AddBuffer(tmp, tmp_received)) {
            LogError("{module:NetService}", "Client Read Buffer is Full", pConn->ToString());
            tmp_received = -1;
            break;
        }
        // LogInfo("{module:NetService}", "Received data from connection, size:", tmp_received);
        // jk's todo : 统计接收到的数据量
    }

    if (tmp_received < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // 数据读取完毕
            HandleReceivedMsg(iConnFd);
        } else {
            LogError("{module:NetService}", "Failed to read from connection", pConn->ToString());
            CloseFd(iConnFd);
            return NetError::NET_ERR;
        }
    } else if (tmp_received == 0) {
        LogInfo("{module:NetService}", "Closed connection,", pConn->ToString());
        // 先将fd从epoll删除，再关闭fd
        m_kNetEpoll->EpollDel(iConnFd);
        m_kConnPool->RemoveConnection(std::move(pConn));
        CloseFd(iConnFd);
    }
    return eErr;
}

NetError NetService::HandleReceivedMsg(int32_t iConnFd) {
    std::shared_ptr<INetConnection> pConn = m_kConnPool->GetConnection(iConnFd);
    if (pConn == nullptr) {
        LogError("{module:NetService}", "Conn is nullptr");
        return NetError::NET_CONN_NOT_EXIST_ERR;
    }

    if (pConn->GetRecvBuffer() == nullptr) {
        LogError("{module:NetService}", "Conn buffer is nullptr :", pConn->ToString());
        return NetError::NET_CONN_RECV_BUFFER_NULLPTR_ERR;
    }

    bool bSuccDecodeOnePackage = false;

    do {
        // Todo : 解析消息头
        MessageHead pMsgHead;
        char* pMsgHeadBytes = nullptr;
        if (pConn->GetRecvBuffer()->GetCapacity() >= MESSAGE_HEAD_SIZE) {
            pMsgHeadBytes = pConn->GetRecvBuffer()->GetBuffer(MESSAGE_HEAD_SIZE);
            pMsgHead.DecodeMessageHeadBytes(pMsgHeadBytes);
        } else {
            break;
        }

        // 消息头合法性校验

        // Todo : 解析消息体
        char* pMsgBodyBytes = nullptr;
        if (pConn->GetRecvBuffer()->GetCapacity() >= pMsgHead.GetMsgLen()) {
            pConn->GetRecvBuffer()->PopBuffer(MESSAGE_HEAD_SIZE);
            pMsgBodyBytes = pConn->GetRecvBuffer()->GetBuffer(pMsgHead.GetMsgLen() - MESSAGE_HEAD_SIZE);
            pConn->GetRecvBuffer()->PopBuffer(pMsgHead.GetMsgLen() - MESSAGE_HEAD_SIZE);

            bSuccDecodeOnePackage = true;
        } else {
            break;
        }
        // Todo : 处理消息
        /*
        CSMessage::PlayerLoginReq kReq;
        kReq.ParseFromArray(pMsgBodyBytes, pMsgHead->GetMsgLen() - MESSAGE_HEAD_SIZE);
        LogInfo("Message : ", kReq.ShortDebugString());
        */
        m_kNetMessageMgr->AddRecvMessage(pMsgHead, pMsgBodyBytes, pConn);

        // Todo : 释放消息头字节流
        if (pMsgHeadBytes != nullptr) {
            delete pMsgHeadBytes;
        }

        // Todo : 释放消息体字节流
        if (pMsgBodyBytes != nullptr) {
            delete pMsgBodyBytes;
        }
    } while (bSuccDecodeOnePackage);

    return NetError::NET_OK;
}

void NetService::SendMsg(std::unique_ptr<NetMessage> pNetMessage) {
    GetNetMessageMgr()->AddSendMessage(pNetMessage);
    return;
}

NetError NetService::ProcessNetSendMessage() {
    /*
    std::deque<std::unique_ptr<NetMessage>> pSendMsgDeque = GetNetMessageMgr()->GetSendMessageDeque();
    for (std::unique_ptr<NetMessage>& pNetMessage : pSendMsgDeque) {
        pNetMessage->GetNetConnection()->SendMsg(pNetMessage->GetMessageHead(), 45);
    }
    */
    return NetError::NET_OK;
}

#endif  // __COMMONSERVER_CPP_