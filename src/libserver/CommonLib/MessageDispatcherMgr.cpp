#include "libserver/CommonLib/MessageDispatcherMgr.h"

template <typename T>
void MessageDispatcherMgr::Register(MsgId::MsgId eMsgId, std::shared_ptr<MessageDispatcher<T>> pMessageDispatcher) {
    m_hmapMsgId2MessageDispatcher[(int32_t)eMsgId] = pMessageDispatcher;
    return;
}

void MessageDispatcherMgr::Invoke(std::unique_ptr<NetMessage> pNetMessage) {
    int32_t iMsgId = pNetMessage->GetMessageHead()->GetMsgId();
    typename std::unordered_map<int32_t, std::shared_ptr<IMessageDispatcher>>::iterator kIter = m_hmapMsgId2MessageDispatcher.find(iMsgId);
    if (kIter == m_hmapMsgId2MessageDispatcher.end()) {
        return;
    }
    (kIter->second)->Invoke(std::move(pNetMessage));
    return;
}