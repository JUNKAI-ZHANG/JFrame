#ifndef __GATESERVER_CPP_
#define __GATESERVER_CPP_

#include "GateServer.h"

void GateServer::LaunchServer() {
    LogInfo("GateServer", "is running...");
    CommonServer::LaunchServer();
    return;
}

std::string GateServer::GetServerModule() {
    return "{module:GateServer}";
}

ServiceType GateServer::GetServiceType() {
    return ServiceType::GateServer_Type;
}

void GateServer::OnAfterLaunchServer() {
    ConnectToRouteServer();
}

int main(int argc, char** argv) {
    CreateServerContext* kCreateServerContext = new CreateServerContext();
    kCreateServerContext->SetPort(10903);
    GateServer* kGateserver = new GateServer(kCreateServerContext);
    kGateserver->LaunchServer();
    return 0;
}

#endif  // __GATESERVER_CPP_