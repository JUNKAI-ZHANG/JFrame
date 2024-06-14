#ifndef __ROUTESERVER_H_
#define __ROUTESERVER_H_

#include "libserver/CommonServer/CommonServer.h"

class RouteServer : public CommonServer {
   public:
    explicit RouteServer(CreateServerContext* kCreateServerContext) : CommonServer(kCreateServerContext) {
        m_pMessageDispatcher = std::make_shared<MessageDispatcher<RouteServer>>();
    }
    virtual ~RouteServer() {
    }

    virtual void InitializeRegisterMsg() override;
    virtual void BeforeLaunchServer() override;
    virtual void LaunchServer() override;

   private:
    void OnRecvServerFirstPackage(std::unique_ptr<MessageHead> pMsgHead, std::string kMsgBodyBytes);

   public:
    std::shared_ptr<MessageDispatcher<RouteServer>> GetMessageDispatcher() {
        return m_pMessageDispatcher;
    }

   private:
    std::shared_ptr<MessageDispatcher<RouteServer>> m_pMessageDispatcher;
};

#endif  // __ROUTESERVER_H_