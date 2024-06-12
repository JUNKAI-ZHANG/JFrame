#include "NetMessageMgr.h"

NetMessageMgr::NetMessageMgr() {
    m_kNetMessageQueue = new BlockQueue<NetMessage>();
}

NetMessageMgr::~NetMessageMgr() {
    delete m_kNetMessageQueue;
}

void NetMessageMgr::Add(NetMessage* kNetMessage) {
    m_kNetMessageQueue->PushBack(*kNetMessage);
}

void NetMessageMgr::Add(MessageHead* kMessageHead, char* kMessageBodyBytes, INetConnection* kConn) {
    m_kNetMessageQueue->PushBack(*(new NetMessage(kMessageHead, kMessageBodyBytes, kConn)));
}

std::deque<std::shared_ptr<NetMessage>> NetMessageMgr::Swap() {
    std::deque<std::shared_ptr<NetMessage>> kDeque;
    kDeque.clear();
    m_kNetMessageQueue->Swap(kDeque);
    return kDeque;
}

std::deque<std::shared_ptr<NetMessage>> NetMessageMgr::SwapTimeout(uint32_t iTimeoutMs) {
    std::deque<std::shared_ptr<NetMessage>> kDeque;
    kDeque.clear();
    m_kNetMessageQueue->SwapTimeout(iTimeoutMs, kDeque);
    return kDeque;
}