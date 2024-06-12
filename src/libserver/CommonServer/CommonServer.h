#ifndef SERVER_COMMONSERVER_COMMONSERVER_H_
#define SERVER_COMMONSERVER_COMMONSERVER_H_

#include <unordered_map>

#include "IBaseServer.h"

class CommonServer : public IBaseServer {
   public:
    explicit CommonServer(CreateServerContext* kCreateServerContext);
    virtual ~CommonServer();

   protected:
    virtual void LaunchServer() override;
    virtual void ProcessNetMessage() override;

   private:
};

#endif  // SERVER_COMMONSERVER_COMMONSERVER_H_