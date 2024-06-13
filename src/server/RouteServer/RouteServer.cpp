#include "RouteServer.h"

void RouteServer::LaunchServer() {
    LogInfo("RouteServer", "is running...");
    CommonServer::LaunchServer();
    return;
}

int main(int argc, char** argv) {
    CreateServerContext* kCreateServerContext = new CreateServerContext();
    kCreateServerContext->SetPort(10913);
    RouteServer* kRouteServer = new RouteServer(kCreateServerContext);
    kRouteServer->LaunchServer();
    return 0;
}
