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
                LogError("{module:NetService}", "HandleConnMsgEvent Failed");
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
sudo systemctl start firewalld
sudo systemctl enable firewalld

sudo firewall-cmd --zone=public --add-port=6666/tcp --permanent

sudo yum install net-tools -y
sudo netstat -tulnp | grep 6666

sudo systemctl stop firewalld
sudo systemctl disable firewalld

sudo systemctl start iptables
sudo systemctl enable iptables

# 开放单个端口
sudo iptables -A INPUT -p tcp --dport 6666 -j ACCEPT

# 开放端口区间
sudo iptables -A INPUT -m state --state NEW -m tcp -p tcp --dport 40000:40010 -j ACCEPT

# 保存iptables规则
sudo service iptables save


NetError NetService::DoTick() {
    // 控制频率
    // LogInfo("{module:NetService}", "Now time is :", TimeUtil.GetNowS());
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
        LogError("{module:NetService}", "Conn buffer is nullptr :", pConn->ToString());
        return NetError::NET_CONN_RECV_BUFFER_NULLPTR_ERR;
    }

    // Receive data
    uint8_t tmp[2048];
    ssize_t tmp_received = 0;
    while ((tmp_received = recv(iConnFd, tmp, 2048, MSG_DONTWAIT)) > 0) {
        if (!pConn->GetRecvBuffer()->AddBuffer(tmp, tmp_received)) {
            LogError("{module:NetService}", "Client Read Buffer is Full", pConn->ToString());
            tmp_received = -1;
            break;
        }
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
        LogError("{module:NetService}", "Connection closed by remote host", pConn->ToString());
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