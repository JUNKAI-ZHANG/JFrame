#include "NetMessageMgr.h"

NetMessageMgr::NetMessageMgr() {
}

NetMessageMgr::~NetMessageMgr() {
}

void NetMessageMgr::AddRecvMessage(std::unique_ptr<NetMessage>& pNetMessage) {
    m_kNetRecvMessageDeque.PushBack(std::move(pNetMessage));
}

void NetMessageMgr::AddRecvMessage(MessageHead kMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn) {
    std::unique_ptr<NetMessage> kNetMessage = std::make_unique<NetMessage>(kMessageHead, pMessageBodyBytes, pConn);
    m_kNetRecvMessageDeque.PushBack(std::move(kNetMessage));
}

void NetMessageMgr::AddSendMessage(std::unique_ptr<NetMessage>& pNetMessage) {
    m_kNetSendMessageDeque.PushBack(std::move(pNetMessage));
}

void NetMessageMgr::AddSendMessage(MessageHead kMessageHead, char* pMessageBodyBytes, std::shared_ptr<INetConnection> pConn) {
    std::unique_ptr<NetMessage> kNetMessage = std::make_unique<NetMessage>(kMessageHead, pMessageBodyBytes, pConn);
    m_kNetSendMessageDeque.PushBack(std::move(kNetMessage));
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetRecvMessageDeque() {
    std::deque<std::unique_ptr<NetMessage>> pMessageDeque;
    pMessageDeque.clear();
    m_kNetRecvMessageDeque.Swap(pMessageDeque);
    return pMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetRecvMessageDequeTimeout(uint32_t iTimeoutMs) {
    std::deque<std::unique_ptr<NetMessage>> pMessageDeque;
    pMessageDeque.clear();
    m_kNetRecvMessageDeque.SwapTimeout(iTimeoutMs, pMessageDeque);
    return pMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetSendMessageDeque() {
    std::deque<std::unique_ptr<NetMessage>> pMessageDeque;
    pMessageDeque.clear();
    m_kNetSendMessageDeque.Swap(pMessageDeque);
    return pMessageDeque;
}

std::deque<std::unique_ptr<NetMessage>> NetMessageMgr::GetSendMessageDequeTimeout(uint32_t iTimeoutMs) {
    std::deque<std::unique_ptr<NetMessage>> pMessageDeque;
    pMessageDeque.clear();
    m_kNetSendMessageDeque.SwapTimeout(iTimeoutMs, pMessageDeque);
    return pMessageDeque;
}