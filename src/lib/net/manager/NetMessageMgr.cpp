#include "NetMessageMgr.h"

NetMessageMgr::NetMessageMgr() {
}

NetMessageMgr::~NetMessageMgr() {
}

void NetMessageMgr::AddRecvMessage(std::unique_ptr<NetMessage> pNetMessage) {
    m_kNetRecvMessageDeque.PushBack(std::move(pNetMessage));
}

void NetMessageMgr::AddRecvMessage(std::unique_ptr<MessageHead> pMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn) {
    std::unique_ptr<NetMessage> pNetMessage = std::make_unique<NetMessage>(std::move(pMessageHead), pMessageBodyBytes, pConn);
    m_kNetRecvMessageDeque.PushBack(std::move(pNetMessage));
}

void NetMessageMgr::AddSendMessage(std::unique_ptr<NetMessage> pNetMessage) {
    m_kNetSendMessageDeque.PushBack(std::move(pNetMessage));
}

void NetMessageMgr::AddSendMessage(std::unique_ptr<MessageHead> pMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn) {
    std::unique_ptr<NetMessage> kNetMessage = std::make_unique<NetMessage>(std::move(pMessageHead), pMessageBodyBytes, pConn);
    m_kNetSendMessageDeque.PushBack(std::move(kNetMessage));
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetRecvMessageDeque() {
    std::deque<std::unique_ptr<NetMessage>> kMessageDeque;
    kMessageDeque.clear();
    m_kNetRecvMessageDeque.Swap(kMessageDeque);
    return kMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetRecvMessageDequeTimeout(uint32_t iTimeoutMs) {
    std::deque<std::unique_ptr<NetMessage>> kMessageDeque;
    kMessageDeque.clear();
    m_kNetRecvMessageDeque.SwapTimeout(iTimeoutMs, kMessageDeque);
    return kMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetSendMessageDeque() {
    std::deque<std::unique_ptr<NetMessage>> kMessageDeque;
    kMessageDeque.clear();
    m_kNetSendMessageDeque.Swap(kMessageDeque);
    return kMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetSendMessageDequeTimeout(uint32_t iTimeoutMs) {
    std::deque<std::unique_ptr<NetMessage>> kMessageDeque;
    kMessageDeque.clear();
    m_kNetSendMessageDeque.SwapTimeout(iTimeoutMs, kMessageDeque);
    return kMessageDeque;
}