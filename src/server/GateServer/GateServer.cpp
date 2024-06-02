#ifndef __GATESERVER_CPP_
#define __GATESERVER_CPP_

#include "../CommonServer/CommonServer.h"

#include "../../lib/logger/log.h"

int main() {
    CommonServer* kGateserver = new CommonServer();

    Log* kLogger = new Log();

    kLogger->Info("GateServer", "is running...", 1 + 2);

    kGateserver->Working(6666);

    return 0;
}

#endif  // __GATESERVER_CPP_