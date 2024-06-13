#ifndef __GATESERVER_H_
#define __GATESERVER_H_

#include "libserver/CommonServer/CommonServer.h"
#include "libserver/CommonServer/CreateServerContext.h"

#define GATE_MODULE "{module:GateServer}"

class GateServer : public CommonServer {
   public:
    explicit GateServer(CreateServerContext* kCreateServerContext)
        : CommonServer(kCreateServerContext) {
    }
    virtual ~GateServer() {
    }
    virtual std::string GetServerModule() override;
    virtual ServiceType GetServiceType() override;

    virtual void LaunchServer() override;
    virtual void OnAfterLaunchServer() override;

   protected:
   private:
};

#endif  // __GATESERVER_H_