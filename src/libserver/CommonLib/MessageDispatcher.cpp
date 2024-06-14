#include "libserver/CommonLib/MessageDispatcher.h"

template <typename T>
MessageDispatcher<T>::MessageDispatcher() {
}

template <typename T>
MessageDispatcher<T>::~MessageDispatcher() {
}

template <typename T>
void MessageDispatcher<T>::RegisterMsg(MsgId::MsgId eMsgId, MsgCallback kCallback) {
    if (m_hmapMsgId2Callback.find((int32_t)eMsgId) != m_hmapMsgId2Callback.end()) {
        return;
    }
    m_hmapMsgId2Callback[(int32_t)eMsgId] = kCallback;
    MESSAGE_DISPATCHER_MGR.Register<T>(eMsgId, std::make_shared<MessageDispatcher<T>>(this));
}

template <typename T>
void MessageDispatcher<T>::Invoke(std::unique_ptr<NetMessage> pNetMessage) {
    int32_t iMsgId = pNetMessage->GetMessageHead()->GetMsgId();
    typename std::unordered_map<int32_t, MsgCallback>::iterator kIter = m_hmapMsgId2Callback.find(iMsgId);
    if (kIter == m_hmapMsgId2Callback.end()) {
        return;
    }
    (kIter->second)(std::move(pNetMessage->GetMessageHead()), pNetMessage->GetMessageBodyBytes());
}