#ifndef __EPOLL_CPP_
#define __EPOLL_CPP_

#include <Epoll.h>

class Epoll;

Epoll::Epoll() {
}

Epoll::~Epoll() {
    close(_m_epoll_fd);
}

int32_t Epoll::GetEpollFd() {
    return _m_epoll_fd;
}

int32_t Epoll::CreateEpoll() {
    // Since Linux 2.6.8, the size argument is ignored, but must be greater than zero;
    _m_epoll_fd = epoll_create(1);
    if (_m_epoll_fd == -1) {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return -1;
    }

    return 1;
}

/*
 * @return 1 success -1 failed
 */
int32_t Epoll::AddEventToEpoll(int fd) {
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    event.data.fd = fd;

    if (epoll_ctl(_m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        std::cerr << "Failed to add connection_fd to epoll instance" << std::endl;
        return -1;
    }

    return 1;
}

#endif // __EPOLL_CPP_
