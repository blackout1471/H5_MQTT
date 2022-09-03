#include "SocketUtility.h"
#include <ws2tcpip.h>
#include <WinSock2.h>

int create_socket(int port)
{
	auto addrInfo = addrinfo{ 0 };
	addrInfo.ai_family = AF_UNSPEC;
	addrInfo.ai_socktype = SOCK_STREAM;
	addrInfo.ai_protocol = IPPROTO_TCP;

	addrinfo* result;
	auto iResult = getaddrinfo("127.0.0.1", "1884", &addrInfo, &result);
	if (iResult != 0)
		return iResult;

	auto connSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connSocket == INVALID_SOCKET)
		return -1;

	return connSocket;
}

void close_socket(int sockfd) {
	closesocket(sockfd);
}