#include "./CommonServer.h"

#include <memory.h>

#include <iostream>
#include <thread>

CommonServer::CommonServer(CreateServerContext* kCreateServerContext)
    : IBaseServer(kCreateServerContext) {
}

CommonServer::~CommonServer() {
}

void CommonServer::ProcessNetMessage() {
    std::deque<std::shared_ptr<NetMessage>> kMessageDeque = this->GetNetService()->GetNetMessageMgr()->Swap();
    LogInfo("Recv package num :", kMessageDeque.size());
}

void CommonServer::LaunchServer() {
    // 创建网络线程
    std::thread kNetThread(std::bind(&NetService::Working, GetNetService(), GetCreateServerContext()->GetPort()));

    while (true) {
        usleep(1000000);
        this->ProcessNetMessage();
    }

    // 等待线程结束
    kNetThread.join();

    return;
}
