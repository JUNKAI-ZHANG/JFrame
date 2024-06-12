#include "./CommonServer.h"

#include <memory.h>

#include <iostream>
#include <thread>

CommonServer::CommonServer(CreateServerContext* kCreateServerContext)
    : IBaseServer(kCreateServerContext) {
}

CommonServer::~CommonServer() {
}

void CommonServer::ProcessNetRecvMessage() {
    std::deque<std::unique_ptr<NetMessage>> pRecvMsgDeque = this->GetNetService()->GetNetMessageMgr()->GetRecvMessageDeque();
    LogInfo("Recv package num :", pRecvMsgDeque.size(), ",buffer :", pRecvMsgDeque.front()->GetNetConnection()->GetRecvBuffer()->GetCapacity());
    for (std::unique_ptr<NetMessage>& pNetMessage : pRecvMsgDeque) {
        GetNetService()->SendMsg(pNetMessage);
    }
}

void CommonServer::LaunchServer() {
    // 创建网络线程
    std::thread kNetThread(std::bind(&NetService::Working, GetNetService(), GetCreateServerContext()->GetPort()));

    while (true) {
        usleep(300000);
        this->ProcessNetRecvMessage();
    }

    // 等待线程结束
    kNetThread.join();

    return;
}
