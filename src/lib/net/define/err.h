#ifndef NET_ERR_H
#define NET_ERR_H

enum NetError {
    NET_OK = 0,                  // ok
    NET_ERR = 1,                 // error
    NET_SOCKET_CREATE_ERR = 2,   // socket create error
    NET_SOCKET_SET_OPT_ERR = 3,  // socket set opt error
    NET_SOCKET_BIND_ERR = 4,     // socket bind error
    NET_SOCKET_LISTEN_ERR = 5,   // socket listen error

    NET_EPOLL_CREATE_ERR = 21,  // epoll create error
    NET_EPOLL_ADD_FD_ERR = 22,  // epoll add fd error
    NET_EPOLL_WAIT_ERR = 23,    // epoll wait error
};

#endif