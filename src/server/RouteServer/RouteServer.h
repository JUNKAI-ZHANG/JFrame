#ifndef __ROUTESERVER_H_
#define __ROUTESERVER_H_

#include "libserver/CommonServer/CommonServer.h"

class RouteServer : public CommonServer {
   public:
    explicit RouteServer(CreateServerContext* kCreateServerContext)
        : CommonServer(kCreateServerContext) {
    }
    virtual ~RouteServer() {
    }

    virtual void LaunchServer() override;
};

#endif  // __ROUTESERVER_H_