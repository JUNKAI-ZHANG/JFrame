#include "INetConnection.h"

INetConnection::INetConnection(std::unique_ptr<IConnectionContext>&& pConnectionContext) {
    m_pConnectionContext = std::move(pConnectionContext);
    m_pRecvBuffer = std::make_shared<RingBuffer>();
}

INetConnection::~INetConnection() {
}

void INetConnection::SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage) {
}
void INetConnection::SendMsg(const char* pMsg, uint32_t iLen) {
}
void INetConnection::Close() {
}

uint64_t INetConnection::GetConnGuid() {
    return (((uint64_t)GetIp()) << 16) | GetPort();
}

uint32_t INetConnection::GetIp() {
    return m_pConnectionContext->GetIp();
}

uint16_t INetConnection::GetPort() {
    return m_pConnectionContext->GetPort();
}

int32_t INetConnection::GetSocketFd() {
    return m_pConnectionContext->GetSocketFd();
}

uint64_t INetConnection::GetConnMs() {
    return m_pConnectionContext->GetConnMs();
}

std::shared_ptr<RingBuffer> INetConnection::GetRecvBuffer() {
    return m_pRecvBuffer;
}

std::string INetConnection::GetIpStr() {
    char pIpStr[16] = {0};
    snprintf(pIpStr, sizeof(pIpStr), "%d.%d.%d.%d",
             (GetIp() & 0xFF000000) >> 24,
             (GetIp() & 0x00FF0000) >> 16,
             (GetIp() & 0x0000FF00) >> 8,
             (GetIp() & 0x000000FF));
    return std::string(pIpStr);
}

std::string INetConnection::ToString() {
    char pStr[128] = {0};
    snprintf(pStr, sizeof(pStr), "ip:%s port:%d socket_fd:%d conn_ms:%lu", GetIpStr().c_str(), GetPort(), GetSocketFd(), GetConnMs());
    return std::string(pStr);
}

//============================================================================================================

CSConnection::CSConnection(std::unique_ptr<CSConnectionContext>&& kCSConnectionContext)
    : INetConnection(std::move(kCSConnectionContext)) {
}

CSConnection::~CSConnection() {}

void CSConnection::SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage) {
}

void CSConnection::SendMsg(const char* pMsg, uint32_t iLen) {
    // TODO
}

void CSConnection::Close() {
    // TODO
}

//============================================================================================================
SSConnection::SSConnection(std::unique_ptr<SSConnectionContext>&& kSSConnectionContext)
    : INetConnection(std::move(kSSConnectionContext)) {
}

SSConnection::~SSConnection() {}

void SSConnection::SendMsg(MsgId::MsgId eMsgId, google::protobuf::Message& kMessage) {
    std::string kMessageBodyBytes = kMessage.SerializeAsString();
    MessageHead kMessageHead;
    kMessageHead.SetMsgId(eMsgId);
    kMessageHead.SetMsgLen(MESSAGE_HEAD_SIZE + kMessageBodyBytes.size());
    kMessageHead.SetMsgSrc((uint16_t)m_pConnectionContext->m_eLocalType);
    kMessageHead.SetMsgDst((uint16_t)m_pConnectionContext->m_ePeerType);
    kMessageHead.SetMsgSeq(1);  // todo : net seq Generate
    kMessageHead.SetMsgTime(TimeUtil.GetNowS());
    std::string kMessageHeadBytes = kMessageHead.EncodeMessageHeadBytes();
    this->SendMsg((kMessageHeadBytes + kMessageBodyBytes).c_str(), kMessageHead.GetMsgLen());
}

void SSConnection::SendMsg(const char* pMsg, uint32_t iLen) {
    NET_SERVICE.GetNetSocket()->SendMsg(GetSocketFd(), pMsg, iLen);
}

void SSConnection::Close() {
    // TODO
}