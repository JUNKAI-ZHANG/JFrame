#ifndef __GATESERVER_H_
#define __GATESERVER_H_

#include "libserver/CommonServer/CommonServer.h"
#include "libserver/CommonServer/CreateServerContext.h"

class GateServer : public CommonServer {
   public:
    explicit GateServer(CreateServerContext* kCreateServerContext)
        : CommonServer(kCreateServerContext) {
    }
    virtual ~GateServer() {
    }

    virtual void LaunchServer() override;

   protected:
   private:
};

#define GateServer

#endif  // __GATESERVER_H_