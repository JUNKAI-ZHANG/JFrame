#ifndef SERVER_COMMONSERVER_CREATESERVERCONTEXT_H_
#define SERVER_COMMONSERVER_CREATESERVERCONTEXT_H_

#include <stdint.h>

class CreateServerContext {
   public:
    explicit CreateServerContext() {
    }

    virtual ~CreateServerContext() {
    }

    void SetPort(int32_t iPort) { m_iPort = iPort; }
    int32_t GetPort() { return m_iPort; }

   protected:
   private:
    int32_t m_iPort;
};

#endif  // SERVER_COMMONSERVER_CREATESERVERCONTEXT_H_