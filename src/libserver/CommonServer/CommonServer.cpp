#include "./CommonServer.h"

#include <memory.h>

#include <iostream>
#include <thread>

#include "lib/net/connection/INetConnection.h"
#include "proto/msg_id.pb.h"
#include "proto/msg_ss_common.pb.h"

CommonServer::CommonServer(CreateServerContext* kCreateServerContext)
    : IBaseServer(kCreateServerContext) {
}

CommonServer::~CommonServer() {
}

std::string CommonServer::GetServerModule() {
    return "{module:CommonServer}";
}

void CommonServer::InitializeRegisterMsg() {
}

void CommonServer::BeforeLaunchServer() {
    this->InitializeRegisterMsg();
}

void CommonServer::LaunchServer() {
    this->BeforeLaunchServer();
    // 创建网络线程
    std::thread kNetThread(std::bind(&NetService::Working, &NET_SERVICE, GetCreateServerContext()->GetPort()));
    this->OnAfterLaunchServer();

    while (true) {
        usleep(300000);
        this->ProcessNetRecvMessage();
    }

    // 等待线程结束
    kNetThread.join();

    return;
}

void CommonServer::OnAfterLaunchServer() {
}

void CommonServer::ProcessNetRecvMessage() {
    std::deque<std::unique_ptr<NetMessage>> pRecvMsgDeque = NET_SERVICE.GetNetMessageMgr()->GetRecvMessageDeque();
    LogInfo("Recv package num :", pRecvMsgDeque.size(), ",buffer :", pRecvMsgDeque.front()->GetNetConnection()->GetRecvBuffer()->GetCapacity());
    // todo : 做分帧处理包

    for (std::unique_ptr<NetMessage>& pNetMessage : pRecvMsgDeque) {
        MESSAGE_DISPATCHER_MGR.Invoke(std::move(pNetMessage));
    }
    return;
}

int32_t CommonServer::TryConnectToRouteServer() {
    int32_t iConnFd = -1;
    NetError eErr = NetError::NET_OK;
    if ((eErr = NET_SERVICE.GetNetSocket()->Connect("127.0.0.1", 10913, iConnFd)) != NetError::NET_OK) {
        LogError("{module:CommonServer}", "Failed to connect to RouteServer, errcode :", eErr);
        return -1;
    }

    if ((eErr = NET_SERVICE.GetNetEpoll()->EpollAdd(iConnFd)) != NetError::NET_OK) {
        LogError("{module:CommonServer}", "Failed to connect to RouteServer, errcode :", eErr);
        return -1;
    }
    return iConnFd;
}

bool CommonServer::ConnectToRouteServer() {
    int32_t iRetryCnt = 60;
    bool bIsConnToRouteServer = false;
    int32_t iRouteServerSocketFd = -1;
    while (iRetryCnt-- && !bIsConnToRouteServer) {
        usleep(500000);
        iRouteServerSocketFd = this->TryConnectToRouteServer();
        if (iRouteServerSocketFd != -1) {
            bIsConnToRouteServer = true;
        }
    }
    if (!bIsConnToRouteServer) {
        return false;
    }

    // 添加连接进连接池
    std::unique_ptr<SSConnectionContext> kSSConnectionContext = std::make_unique<SSConnectionContext>();
    kSSConnectionContext->m_iIp = 127 * 256 * 256 * 256 + 1;
    kSSConnectionContext->m_iPort = 10913;
    kSSConnectionContext->m_iSocketFd = iRouteServerSocketFd;
    kSSConnectionContext->m_lConnMs = TimeUtil.GetNowMs();
    kSSConnectionContext->m_eLocalType = ServiceType::GateServer_Type;
    kSSConnectionContext->m_ePeerType = ServiceType::RouteServer_Type;
    m_pRouteServerConn = std::make_shared<SSConnection>(std::move(kSSConnectionContext));
    NET_SERVICE.GetConnectionPool()->AddConnection(m_pRouteServerConn);

    // 发首包给Route
    SSMessage::ServerFirstPackage kServerFirstPackage;
    this->SendMsgToConn(m_pRouteServerConn, MsgId::MsgId::MSG_ID_SERVER_FIRST_PACKAGE, kServerFirstPackage);

    LogInfo(GetServerModule(), "Success to connect RouteServer");
    return true;
}

std::shared_ptr<SSConnection> CommonServer::GetRouteServerConn() {
    return m_pRouteServerConn;
}

void CommonServer::SendMsgToConn(std::shared_ptr<INetConnection> pNetConnection, MsgId::MsgId eMsgId, ::google::protobuf::Message& kMessage) {
    if (pNetConnection == nullptr) {
        return;
    }
    pNetConnection->SendMsg(eMsgId, kMessage);
}
