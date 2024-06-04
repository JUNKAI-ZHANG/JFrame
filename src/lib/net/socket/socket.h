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
            std::cout << "Failed to create socket" << std::endl;
            return NET_SOCKET_CREATE_ERR;
        }

        int32_t ok = 1;
        if (setsockopt(GetSocketFd(), SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int32_t)) == -1) {
            perror("setsockopt");
            return NET_SOCKET_SET_OPT_ERR;
        }
        return NET_OK;
    }

    NetError Bind(uint32_t iIp, uint16_t iPort) {
        struct sockaddr_in kAddr;
        memset(&kAddr, 0, sizeof(kAddr));
        kAddr.sin_family = AF_INET;
        kAddr.sin_port = htons(iPort);
        kAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(GetSocketFd(), reinterpret_cast<struct sockaddr*>(&kAddr), sizeof(kAddr)) == -1) {
            std::cout << "Failed to bind address" << std::endl;
            this->Close();
            return NET_SOCKET_BIND_ERR;
        }
        return NET_OK;
    }

    NetError Listen() {
        if (listen(GetSocketFd(), SOMAXCONN) == -1) {
            std::cout << "Failed to listen on socket" << std::endl;
            this->Close();
            return NET_SOCKET_LISTEN_ERR;
        }
        return NET_OK;
    }

    NetError Accept(int32_t& iAcceptFd) {
        struct sockaddr_in kAddr;
        socklen_t addrlen = sizeof(kAddr);
        iAcceptFd = accept(m_iSocketFd, reinterpret_cast<struct sockaddr*>(&kAddr), &addrlen);

        if (iAcceptFd == -1) {
            std::cout << "Failed to accept connection" << std::endl;
            return NetError::NET_SOCKET_ACCEPT_ERR;
        }
        std::cout << "Accepted connection from " << inet_ntoa(kAddr.sin_addr) << ":" << ntohs(kAddr.sin_port) << std::endl;
        return NetError::NET_OK;
    }

    /*
    virtual int32_t Connect(uint32_t iIp, uint16_t iPort) = 0;

    virtual int32_t SendMsg(const char* pMsg, uint32_t iLen) = 0;

    virtual int32_t RecvMsg(char* pMsg, uint32_t iLen) = 0;
    */

    void Close() {
        close(GetSocketFd());
    }

   private:
    int32_t m_iSocketFd;
};

#endif  // NET_SOCKET_SOCKET_H