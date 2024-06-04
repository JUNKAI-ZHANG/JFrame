#ifndef __GATESERVER_H_
#define __GATESERVER_H_

#include "../../lib/logger/log.h"
#include "../CommonServer/CommonServer.h"
#include "../CommonServer/CreateServerContext.h"

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

#endif  // __GATESERVER_H_