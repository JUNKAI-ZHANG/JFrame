#include "./CommonServer.h"

#include <memory.h>

#include <iostream>
#include <thread>

class CommonServer;

CommonServer::CommonServer(CreateServerContext* kCreateServerContext)
    : IBaseServer(kCreateServerContext) {
}

CommonServer::~CommonServer() {
}

void CommonServer::LaunchServer() {
    // 创建网络线程
    std::thread kNetThread(std::bind(&NetService::Working, GetNetService(), GetCreateServerContext()->GetPort()));

    // 等待线程结束
    kNetThread.join();

    return;
}
