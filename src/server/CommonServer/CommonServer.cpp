#include "./CommonServer.h"

#include <memory.h>

#include <iostream>

class CommonServer;

CommonServer::CommonServer(CreateServerContext* kCreateServerContext)
    : IBaseServer(kCreateServerContext) {
}

CommonServer::~CommonServer() {
}

void CommonServer::LaunchServer() {
    this->GetNetService()->Working(GetCreateServerContext()->GetPort());
    return;
}
