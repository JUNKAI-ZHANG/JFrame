#ifndef __GATESERVER_CPP_
#define __GATESERVER_CPP_

#include "GateServer.h"

void GateServer::LaunchServer() {
    LogInfo("GateServer", "is running...");
    CommonServer::LaunchServer();
    return;
}

int main() {
    CreateServerContext* kCreateServerContext = new CreateServerContext();
    kCreateServerContext->SetPort(10903);
    GateServer* kGateserver = new GateServer(kCreateServerContext);
    kGateserver->LaunchServer();
    return 0;
}

#endif  // __GATESERVER_CPP_