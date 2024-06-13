#ifndef SERVER_COMMONSERVER_COMMONSERVER_H_
#define SERVER_COMMONSERVER_COMMONSERVER_H_

#include <unordered_map>

#include "IBaseServer.h"

class CommonServer : public IBaseServer {
   public:
    explicit CommonServer(CreateServerContext* kCreateServerContext);
    virtual ~CommonServer();
    virtual std::string GetServerModule() override;

   protected:
    virtual void LaunchServer() override;
    virtual void OnAfterLaunchServer() override;
    virtual void ProcessNetRecvMessage() override;
    virtual void SendMsgToConn(std::shared_ptr<INetConnection> pNetConnection, MsgId::MsgId eMsgId, ::google::protobuf::Message& kMessage) override;

   protected:
    int32_t TryConnectToRouteServer();
    bool ConnectToRouteServer();
    std::shared_ptr<SSConnection> GetRouteServerConn();

   private:
    std::shared_ptr<SSConnection> m_pRouteServerConn;
};

#endif  // SERVER_COMMONSERVER_COMMONSERVER_H_