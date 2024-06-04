#ifndef NET_ERR_H
#define NET_ERR_H

enum NetError {
    NET_OK = 0,                  // ok
    NET_ERR = 1,                 // error

    NET_SOCKET_NULLPTR_ERR = 12,     // socket is nullptr
    NET_EPOLL_NULLPTR_ERR = 13,      // epoll is nullptr
    NET_CONN_POOL_NULLPTR_ERR = 14,  // connection pool is nullptr

    NET_SOCKET_CREATE_ERR = 42,   // socket create error
    NET_SOCKET_SET_OPT_ERR = 43,  // socket set opt error
    NET_SOCKET_BIND_ERR = 44,     // socket bind error
    NET_SOCKET_LISTEN_ERR = 45,   // socket listen error
    NET_SOCKET_ACCEPT_ERR = 46,   // socket accept error

    NET_EPOLL_CREATE_ERR = 61,  // epoll create error
    NET_EPOLL_ADD_FD_ERR = 62,  // epoll add fd error
    NET_EPOLL_WAIT_ERR = 63,    // epoll wait error

    NET_CONN_NOT_EXIST_ERR = 81,  // connection not exist error

    NET_CONN_RECV_BUFFER_NULLPTR_ERR = 101,  // connection recv buffer is nullptr
};

#endif