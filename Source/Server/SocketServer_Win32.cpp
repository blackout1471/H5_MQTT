#include "mqttpch.h"
#include "SocketServer.h"
#include "Server/MqttService.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
namespace MQTT {
	namespace Server {

		static WSADATA s_WsaData;
		static bool s_Initialized = false;
		static int ListenSocket;
		struct sockaddr_in serv_addr = { 0 };
		static struct addrinfo hints;
		static struct addrinfo* result = NULL;
		auto s_ReadThreads = std::vector<std::thread>();

		SocketServer::SocketServer(int port)
			: m_Port(port), m_IsRunning(false) {};
		SocketServer::~SocketServer()
		{
			for (auto client : m_Clients)
				delete client;
		}

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
			for (const auto& client : m_Clients)
				Disconnect(*client);

			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
		}
		void SocketServer::Disconnect(const Client& client)
		{
			closesocket(client.GetConnection());
		}
		void SocketServer::Send(const Client& client, const std::vector<unsigned char>& data)
		{
			int iSendResult = send(client.GetConnection(), (const char*)(data.data()), data.size(), 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(client.GetConnection());
				WSACleanup();
				//throw error
			}
		}

		//Configures ip address and port for server.
		void SocketServer::ConfigureAddressInfo(int port) {

			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			serv_addr.sin_port = htons(port);	

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

			ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

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

			int iResult = bind(ListenSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

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
			int iResult = listen(ListenSocket, 10);
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

			int clientSocket = accept(ListenSocket, (struct sockaddr*)NULL, NULL);

			if (clientSocket > 0) {
				m_Clients.push_back(new Client("123", "1", clientSocket));
				s_ReadThreads.push_back(std::thread(SocketServer::ReadClientData, std::cref(*m_Clients[m_Clients.size() - 1]), std::cref(*this)));
			}
			else
			{
				//logging here
				printf("accept failed with error: %d\n", WSAGetLastError());
				closesocket(ListenSocket);
				WSACleanup();
				//throw error
			}

		}
		void SocketServer::ReadClientData(const Client& client, const SocketServer& server)
		{
			char sendBuff[64] = { 0 };

			while (1)
			{
				if (int amount = recv(client.GetConnection(), sendBuff, 64, 0))
				{
					if (amount < 0)
						printf("error: %d\n", WSAGetLastError());

					if (server.OnReceivedData)
					{
						auto data = std::vector<unsigned char>();
						data.insert(data.end(), &sendBuff[0], &sendBuff[amount]);
						server.OnReceivedData(client, data);
					}
				}
			}
		}
	}
}