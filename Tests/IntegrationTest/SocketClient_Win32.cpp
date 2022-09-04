#include "SocketClient.h"
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <vector>

struct WinSocketClient {
	addrinfo* Result;
	addrinfo Hints;
    int Socket;
};

static std::vector<WinSocketClient> s_Clients = std::vector<WinSocketClient>();

SocketClient::SocketClient(const std::string& ip, const std::string& port) : m_Ip(ip), m_Port(port), m_Id(-1) {
    s_Clients.push_back({ nullptr, {0}, -1});
    m_Id = s_Clients.size() - 1;

    WinSocketClient& client = s_Clients[m_Id];
    client.Hints.ai_family = AF_INET;
    client.Hints.ai_socktype = SOCK_STREAM;
    client.Hints.ai_protocol = IPPROTO_TCP;
}

SocketClient::~SocketClient() {
    closesocket(s_Clients[m_Id].Socket);
}

void SocketClient::Connect() {

    WinSocketClient& client = s_Clients[m_Id];

    // Resolve the server address and port
    auto iResult = getaddrinfo(m_Ip.c_str(), m_Port.c_str(), &client.Hints, &client.Result);
    if (iResult != 0) {
        if (m_ErrorEvent) m_ErrorEvent();
    }

    // Create a SOCKET for connecting to server
    client.Socket = socket(client.Result->ai_family, client.Result->ai_socktype, client.Result->ai_protocol);
    if (client.Socket == INVALID_SOCKET) {
        if (m_ErrorEvent) m_ErrorEvent();
    }

    iResult = connect(client.Socket, client.Result->ai_addr, (int)client.Result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(client.Socket);
        if (m_ErrorEvent) m_ErrorEvent();
    }
}

void SocketClient::Send(const std::vector<unsigned char>& data) {

    WinSocketClient& client = s_Clients[m_Id];

    auto iResult = send(client.Socket, (const char*)data.data(), data.size(), 0);
    if (iResult == SOCKET_ERROR) {
        if (m_ErrorEvent) m_ErrorEvent();
    }
}

std::vector<unsigned char> SocketClient::Listen() {
    WinSocketClient& client = s_Clients[m_Id];

    char recvbuf[1024] = { 0 };

    auto iResult = recv(client.Socket, recvbuf, 1024, 0);

    if (iResult <= 0)
        if (m_ErrorEvent) m_ErrorEvent();

    return std::vector<unsigned char>(recvbuf, recvbuf + iResult);
}

void SocketClient::SetErrorEvent(const std::function<void()>& ev) {
    m_ErrorEvent = ev;
}