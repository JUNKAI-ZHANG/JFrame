#ifndef SERVER_COMMONSERVER_IBASESERVER_H_
#define SERVER_COMMONSERVER_IBASESERVER_H_

#include "CreateServerContext.h"
#include "lib/log/log.h"
#include "lib/net/NetService.h"
#include "lib/net/define/err.h"

class IBaseServer {
   public:
    explicit IBaseServer(CreateServerContext* kCreateServerContext)
        : m_kCreateServerContext(kCreateServerContext) {
        m_kNetService = new NetService();
    }
    virtual ~IBaseServer() {
        delete m_kNetService;
    }

    virtual void LaunchServer() = 0;

   protected:
    CreateServerContext* GetCreateServerContext() { return m_kCreateServerContext; }
    NetService* GetNetService() { return m_kNetService; }

    virtual void ProcessNetMessage() = 0;

   private:
    CreateServerContext* m_kCreateServerContext;

    NetService* m_kNetService;
};

#endif  // SERVER_COMMONSERVER_IBASESERVER_H_