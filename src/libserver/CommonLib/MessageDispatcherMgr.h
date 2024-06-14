#ifndef COMMONLIB_MESSAGEDISPATCHERMGR_H
#define COMMONLIB_MESSAGEDISPATCHERMGR_H

#include "lib/util/Singleton.h"
#include "libserver/CommonLib/MessageDispatcher.h"

class IMessageDispatcher;
template <typename T>
class MessageDispatcher;

class MessageDispatcherMgr {
   public:
    template <typename T>
    void Register(MsgId::MsgId eMsgId, std::shared_ptr<MessageDispatcher<T>> pMessageDispatcher);

    void Invoke(std::unique_ptr<NetMessage> pNetMessage);

   private:
    std::unordered_map<int32_t, std::shared_ptr<IMessageDispatcher>> m_hmapMsgId2MessageDispatcher;
};

#define MESSAGE_DISPATCHER_MGR Singleton<MessageDispatcherMgr>::Instance()

#endif  // COMMONLIB_MESSAGEDISPATCHERMGR_H