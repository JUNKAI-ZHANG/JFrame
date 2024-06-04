#ifndef __GATESERVER_CPP_
#define __GATESERVER_CPP_

#include "GateServer.h"

void GateServer::LaunchServer() {
    Log* kLogger = new Log();
    kLogger->Info("GateServer", "is running...", 1 + 2);

    CommonServer::LaunchServer();
    return;
}

int main() {
    CreateServerContext *kCreateServerContext = new CreateServerContext();
    kCreateServerContext->SetPort(6666);
    GateServer* kGateserver = new GateServer(kCreateServerContext);
    kGateserver->LaunchServer();
    return 0;
}

#endif  // __GATESERVER_CPP_