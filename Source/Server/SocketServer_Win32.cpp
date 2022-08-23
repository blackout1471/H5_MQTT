#include "mqttpch.h"
#include "SocketServer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
namespace MQTT {
	namespace Server {

		static WSADATA s_WsaData;
		static bool s_Initialized = false;
		static SOCKET ClientSocket = INVALID_SOCKET;
		static SOCKET ListenSocket = INVALID_SOCKET;
		static struct addrinfo hints;
		static struct addrinfo* result = NULL;

		void SocketServer::Start()
		{
			//Initialization of WSA
			if (!s_Initialized)
			{
				int iResult = WSAStartup(MAKEWORD(2, 2), &s_WsaData);
				if (iResult != 0) {
					//Logging here
					printf("WSAStartup failed with error: %d\n", iResult);
					//throw error
				}
				s_Initialized = true;
			}

			//Configuration
			ConfigureAddressInfo(m_Port);

			//Creates a listening socket
			CreateSocket();

			//Sets up TCP for listening socket
			SetupTCP();

			//Starts listening for a client
			Listen();

			//While alive, listen and accept clients
			while (1) {

				//Accepts a client
				Accept();
			}

		}
		void SocketServer::Stop()
		{
		}
		void SocketServer::Disconnect(const Client& client)
		{
		}
		void SocketServer::Send(const Client& client, const std::vector<unsigned char>& data)
		{
		}

		//Configures ip address and port for server.
		void SocketServer::ConfigureAddressInfo(int port) {

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			// Resolve the server address and port.
			int iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
			if (iResult != 0) {
				//Logging here
				printf("getaddrinfo failed with error: %d\n", iResult);
				WSACleanup();
				//Throw error
			}
		}

		// Create a SOCKET for the server to listen for client connections.
		void SocketServer::CreateSocket() {

			ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

			if (ListenSocket == INVALID_SOCKET) {
				//Logging here instead of printf
				printf("socket failed with error: %ld\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				//throw error here
			}
		}

		//Setup the TCP listening socket
		void SocketServer::SetupTCP() {

			int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

			if (iResult == SOCKET_ERROR) {

				//Logging here
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(ListenSocket);
				WSACleanup();
				//Throw error here
			}

			freeaddrinfo(result);
		}

		void SocketServer::Listen() {
			int iResult = listen(ListenSocket, SOMAXCONN);
			if (iResult == SOCKET_ERROR) {
				//logging here
				printf("listen failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				//throw error here
			}
		}

		void SocketServer::Accept()
		{

			ClientSocket = accept(ListenSocket, NULL, NULL);

			if (ClientSocket == INVALID_SOCKET) {
				//logging here
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				//throw error
			}
			printf("Client accepted");
		}
	}
}