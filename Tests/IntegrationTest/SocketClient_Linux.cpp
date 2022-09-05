#include "SocketClient.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>

struct LinSocketClient {
    sockaddr_in AddrInfo;
    int Socket;
};

static std::vector<LinSocketClient> s_Clients = std::vector<LinSocketClient>();



SocketClient::SocketClient(const std::string& ip, const std::string& port) : m_Ip(ip), m_Port(port), m_Id(-1) {

    s_Clients.push_back({ {0}, -1 });
    m_Id = s_Clients.size() - 1;

    LinSocketClient& client = s_Clients[m_Id];
    client.AddrInfo.sin_family = AF_INET;
    client.AddrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    client.AddrInfo.sin_port = std::stoi(port);
}

SocketClient::~SocketClient() {
    close(s_Clients[m_Id].Socket);
}

void SocketClient::Connect() {

    LinSocketClient& client = s_Clients[m_Id];

    // Resolve the server address and port
    auto iResult = inet_pton(AF_INET, m_Ip.c_str(), &client.AddrInfo.sin_addr);
    if (iResult <= 0) {
        if (m_ErrorEvent) m_ErrorEvent();
    }

    // Create a SOCKET for connecting to server
    client.Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client.Socket <= 0) {
        if (m_ErrorEvent) m_ErrorEvent();
    }

    iResult = connect(client.Socket, (struct sockaddr*)&client.AddrInfo, sizeof(client.AddrInfo));
    if (iResult <= 0) {
        close(client.Socket);
        if (m_ErrorEvent) m_ErrorEvent();
    }
}

void SocketClient::Send(const std::vector<unsigned char>& data) {

    LinSocketClient& client = s_Clients[m_Id];

    auto iResult = send(client.Socket, (const char*)data.data(), data.size(), 0);
    if (iResult <= 0) {
        if (m_ErrorEvent) m_ErrorEvent();
    }
}

std::vector<unsigned char> SocketClient::Listen() {
    LinSocketClient& client = s_Clients[m_Id];

    char recvbuf[1024] = { 0 };

    auto iResult = recv(client.Socket, recvbuf, 1024, 0);

    if (iResult <= 0)
        if (m_ErrorEvent) m_ErrorEvent();

    return std::vector<unsigned char>(recvbuf, recvbuf + iResult);
}

void SocketClient::SetErrorEvent(const std::function<void()>& ev) {
    m_ErrorEvent = ev;
}