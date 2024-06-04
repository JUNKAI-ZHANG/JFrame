#ifndef __GATESERVER_H_
#define __GATESERVER_H_

#include "../CommonServer/CommonServer.h"
#include "../CommonServer/CreateServerContext.h"

#include "../../lib/logger/log.h"

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

#endif // __GATESERVER_H_