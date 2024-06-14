#ifndef COMMONLIB_MESSAGEDISPATCHER_H
#define COMMONLIB_MESSAGEDISPATCHER_H

#include "functional"
#include "lib/net/manager/NetMessageMgr.h"
#include "lib/net/message/MessageHead.h"
#include "libserver/CommonLib/MessageDispatcherMgr.h"
#include "proto/msg_id.pb.h"
#include "string"

class MessageDispatcherMgr;

class IMessageDispatcher {
   public:
    virtual void Invoke(std::unique_ptr<NetMessage> pNetMessage) = 0;
};

template <typename T>
class MessageDispatcher : public IMessageDispatcher {
   public:
    typedef void (T::*MsgCallback)(std::unique_ptr<MessageHead>, std::string);
    typedef void (*tmp)();

   public:
    explicit MessageDispatcher();
    virtual ~MessageDispatcher();

    void RegisterMsg(MsgId::MsgId eMsgId, MsgCallback kCallback);
    void RegisterMsg(MsgId::MsgId eMsgId, std::function<void(std::unique_ptr<MessageHead>, std::string)> kCallback) {}
    void RegisterMsg(MsgId::MsgId eMsgId, tmp kCallback) {}
    virtual void Invoke(std::unique_ptr<NetMessage> pNetMessage) override;

   private:
    std::unordered_map<int32_t, MsgCallback> m_hmapMsgId2Callback;
};

#endif  // COMMONLIB_MESSAGEDISPATCHER_H