#include "RouteServer.h"

void RouteServer::InitializeRegisterMsg() {
    // this->GetMessageDispatcher()->RegisterMsg(MsgId::MsgId::MSG_ID_SERVER_FIRST_PACKAGE, &RouteServer::OnRecvServerFirstPackage);
    this->GetMessageDispatcher()->RegisterMsg(MsgId::MsgId::MSG_ID_SERVER_FIRST_PACKAGE,
                                              std::bind([](std::unique_ptr<MessageHead> pMsgHead, std::string kMsgBodyBytes) -> void {}, std::placeholders::_1, std::placeholders::_2));
    return;
}

void RouteServer::BeforeLaunchServer() {
}

void RouteServer::LaunchServer() {
    LogInfo("RouteServer", "is running...");
    CommonServer::LaunchServer();
    return;
}

void RouteServer::OnRecvServerFirstPackage(std::unique_ptr<MessageHead> pMsgHead, std::string kMsgBodyBytes) {
    LogInfo("OnRecvServerFirstPackage");
}

int main(int argc, char** argv) {
    CreateServerContext* kCreateServerContext = new CreateServerContext();
    kCreateServerContext->SetPort(10913);
    RouteServer* kRouteServer = new RouteServer(kCreateServerContext);
    kRouteServer->LaunchServer();
    return 0;
}
