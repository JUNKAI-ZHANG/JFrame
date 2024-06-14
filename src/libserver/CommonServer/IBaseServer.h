#ifndef SERVER_COMMONSERVER_IBASESERVER_H_
#define SERVER_COMMONSERVER_IBASESERVER_H_

#include "CreateServerContext.h"
#include "lib/log/log.h"
#include "lib/net/NetService.h"
#include "lib/net/define/NetDefine.h"
#include "lib/net/define/err.h"
#include "libserver/CommonLib/MessageDispatcher.h"

class IBaseServer {
   public:
    explicit IBaseServer(CreateServerContext* kCreateServerContext)
        : m_kCreateServerContext(kCreateServerContext) {
    }
    virtual ~IBaseServer() {
    }
    virtual std::string GetServerModule() {
        return "{module:IBaseServer}";
    }
    virtual ServiceType GetServiceType() {
        return ServiceType::None_Type;
    }

    virtual void InitializeRegisterMsg() = 0;
    virtual void BeforeLaunchServer() = 0;
    virtual void LaunchServer() = 0;
    virtual void OnAfterLaunchServer() = 0;
    virtual void ProcessNetRecvMessage() = 0;
    virtual void SendMsgToConn(std::shared_ptr<INetConnection> pNetConnection, MsgId::MsgId eMsgId, ::google::protobuf::Message& kMessage) = 0;

   protected:
    CreateServerContext* GetCreateServerContext() { return m_kCreateServerContext; }

   private:
    CreateServerContext* m_kCreateServerContext;
};

#endif  // SERVER_COMMONSERVER_IBASESERVER_H_