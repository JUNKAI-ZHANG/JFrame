#ifndef NET_SOCKET_SOCKET_H
#define NET_SOCKET_SOCKET_H

#include <arpa/inet.h>
#include <fcntl.h>       // 非阻塞
#include <netinet/in.h>  //socket addr
#include <stdint.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>  //create socket
#include <unistd.h>      //func close()

#include <iostream>

#include "lib/log/log.h"
#include "lib/net/connection/IConnectionContext.h"
#include "lib/net/define/err.h"

class NetSocket {
   public:
    explicit NetSocket() {
        Init();
    }

    virtual ~NetSocket() {}

    int32_t GetSocketFd() const { return m_iSocketFd; }

    NetError Init() {
        m_iSocketFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
        if (GetSocketFd() == -1) {
            LogError("{module:NetSocket}", "Failed to create socket");

            return NetError::NET_SOCKET_CREATE_ERR;
        }

        int32_t ok = 1;
        if (setsockopt(GetSocketFd(), SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int32_t)) == -1) {
            LogError("{module:NetSocket}", "Failed to set socket options");
            return NetError::NET_SOCKET_SET_OPT_ERR;
        }
        return NetError::NET_OK;
    }

    NetError Bind(uint32_t iIp, uint16_t iPort) {
        struct sockaddr_in kAddr;
        memset(&kAddr, 0, sizeof(kAddr));
        kAddr.sin_family = AF_INET;
        kAddr.sin_port = htons(iPort);
        kAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(GetSocketFd(), reinterpret_cast<struct sockaddr*>(&kAddr), sizeof(kAddr)) == -1) {
            LogError("{module:NetSocket}", "Failed to bind address");
            this->Close();
            return NetError::NET_SOCKET_BIND_ERR;
        }
        return NetError::NET_OK;
    }

    NetError Listen() {
        if (listen(GetSocketFd(), SOMAXCONN) == -1) {
            LogError("{module:NetSocket}", "Failed to listen on socket");
            this->Close();
            return NetError::NET_SOCKET_LISTEN_ERR;
        }
        return NetError::NET_OK;
    }

    NetError Accept(int32_t& iAcceptFd, IConnectionContext& pConnCtx) {
        struct sockaddr_in kAddr;
        socklen_t addrlen = sizeof(kAddr);

        if ((iAcceptFd = accept(m_iSocketFd, reinterpret_cast<struct sockaddr*>(&kAddr), &addrlen)) == -1) {
            LogError("{module:NetSocket}", "Failed to accept connection");
            return NetError::NET_SOCKET_ACCEPT_ERR;
        }
        LogInfo("{module:NetSocket}", "Accepted connection", this->toString(kAddr));

        // 构造临时连接上下文，接收到首包的时候建立真正的连接
        pConnCtx.m_iIp = ntohl(kAddr.sin_addr.s_addr);
        pConnCtx.m_iPort = ntohs(kAddr.sin_port);
        pConnCtx.m_iSocketFd = iAcceptFd;
        pConnCtx.m_lConnMs = TimeUtil.GetNowMs();

        return NetError::NET_OK;
    }

    NetError Connect(const std::string& ipAddress, uint16_t port, int32_t& iSocketConnFd) {
        // 创建 socket
        iSocketConnFd = socket(AF_INET, SOCK_STREAM, 0);
        if (iSocketConnFd == -1) {
            LogError("{module:NetSocket}", "Error creating socket");
            return NetError::NET_SOCKET_CREATE_ERR;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);

        // 将 IP 地址从字符串转换为网络字节序
        if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
            LogError("{module:NetSocket}", "Invalid IP address");
            return NetError::NET_SOCKET_INVALID_IP_ERR;
        }

        // 连接服务器
        if (connect(iSocketConnFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            LogError("{module:NetSocket}", "Connection failed");
            return NetError::NET_SOCKET_CONNECT_ERR;
        }

        // LogInfo("{module:NetSocket}", "Connected to server");
        return NetError::NET_OK;
    }

    NetError SendMsg(int32_t iConnFd, const char* pMsg, uint32_t iLen) {
        // 发送数据
        int32_t bytesSent = send(iConnFd, pMsg, iLen, 0);
        if (bytesSent == -1) {
            LogError("{module:NetSocket}", "Message send failed");
            return NetError::NET_SOCKET_SEND_ERR;
        }
        return NetError::NET_OK;
    }

    /*
    virtual int32_t RecvMsg(char* pMsg, uint32_t iLen) = 0;
    */

    void Close() {
        LogDebug("{module:NetSocket}", "Close socket", GetSocketFd());
        close(GetSocketFd());
    }

   private:
    std::string toString(sockaddr_in kAddr) {
        char pStr[24] = {0};
        sprintf(pStr, "ip:%s port:%d", inet_ntoa(kAddr.sin_addr), ntohs(kAddr.sin_port));
        return std::string(pStr);
    }

   private:
    int32_t m_iSocketFd;
};

#endif  // NET_SOCKET_SOCKET_H