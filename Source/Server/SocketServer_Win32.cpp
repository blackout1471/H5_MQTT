#include "mqttpch.h"
#include "SocketServer.h"
#include "Server/MqttService.h"
#include "ClientUtility.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

namespace MQTT {
	namespace Server {

		static WSADATA s_WsaData;
		static bool s_Initialized = false;
		struct sockaddr_in serv_addr = { 0 };
		static struct addrinfo hints;
		static struct addrinfo* result = NULL;

		SocketServer::SocketServer(int port)
			: m_Port(port), m_IsRunning(false), m_Socket(0) 
		{
			//Initialization of WSA
			if (!s_Initialized)
			{
				int iResult = WSAStartup(MAKEWORD(2, 2), &s_WsaData);
				if (iResult != 0) {
					if (ErrorEvent)
						ErrorEvent("WSAStartup failed");
				}
				s_Initialized = true;
			}
		};

		SocketServer::~SocketServer()
		{
			for (auto client : m_Clients)
				delete client;

			if (s_Initialized)
			{
				WSACleanup();
				s_Initialized = false;
			}
		}

		bool SocketServer::IsRunning() const {
			return m_IsRunning;
		}

		void SocketServer::Start()
		{	
			//Configuration
			ConfigureAddressInfo(m_Port);

			//Creates a listening socket
			CreateSocket();

			//Sets up TCP for listening socket
			SetupTCP();

			//Starts listening for a client
			Listen();

			m_IsRunning = true;
			//While alive, listen and accept clients
			while (m_IsRunning) {

				//Accepts a client
				Accept();
			}

		}
		void SocketServer::Stop()
		{
			for (const auto& client : m_Clients)
				Disconnect(*client);

			//freeaddrinfo(result);
			closesocket(m_Socket);
			m_IsRunning = false;
			s_Initialized = false;
			
		}
		void SocketServer::Disconnect(const Client& client)
		{
			closesocket(client.GetConnection());
			auto it = std::find_if(m_Clients.begin(), m_Clients.end(), [&](Client* c) {
				return c->GetIdentifier() == client.GetIdentifier();
				});
			if (it != m_Clients.end())
				m_Clients.erase(it);
		}

		void SocketServer::Send(const Client& client, const std::vector<unsigned char>& data)
		{
			int iSendResult = send(client.GetConnection(), (const char*)(data.data()), data.size(), 0);
			if (iSendResult == SOCKET_ERROR) {
				if (ErrorEvent)
					ErrorEvent("WSA error: " + std::to_string(WSAGetLastError()));
				closesocket(client.GetConnection());
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
				if (ErrorEvent)
					ErrorEvent("getaddinfo error: " + std::to_string(iResult));
				WSACleanup();
				s_Initialized = false;
				//Throw error
			}
		}

		// Create a SOCKET for the server to listen for client connections.
		void SocketServer::CreateSocket() {

			m_Socket = socket(AF_INET, SOCK_STREAM, 0);

			if (m_Socket == INVALID_SOCKET) {
				if (ErrorEvent)
					ErrorEvent("socket failed with error: " + std::to_string(WSAGetLastError()));

				freeaddrinfo(result);
				WSACleanup();
				s_Initialized = false;
			}
		}

		//Setup the TCP listening socket
		void SocketServer::SetupTCP() {

			int iResult = bind(m_Socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

			if (iResult == SOCKET_ERROR) {

				if (ErrorEvent)
					ErrorEvent("bind failed with error: " + std::to_string(WSAGetLastError()));
				freeaddrinfo(result);
				closesocket(m_Socket);
				WSACleanup();
				s_Initialized = false;
			}

			freeaddrinfo(result);
		}

		void SocketServer::Listen() {
			int iResult = listen(m_Socket, 10);
			if (iResult == SOCKET_ERROR) {
				//logging here
				if (ErrorEvent)
					ErrorEvent("Listen failed with error: " + std::to_string(WSAGetLastError()));
				closesocket(m_Socket);
				WSACleanup();
				s_Initialized = false;
				//throw error here
			}
		}

		void SocketServer::Accept()
		{
			int clientSocket = accept(m_Socket, (struct sockaddr*)NULL, NULL);

			if (clientSocket > 0) {
				m_Clients.push_back(new Client("123", ClientUtility::GenerateUniqueId(), clientSocket));
				m_ClientReaderThreads.push_back(std::thread(SocketServer::ReadClientData, std::cref(*m_Clients[m_Clients.size() - 1]), std::cref(*this)));
			}
			else
			{
				if (ErrorEvent)
					ErrorEvent("Accept failed with error: " + std::to_string(WSAGetLastError()));
			}

		}

		void SocketServer::ReadClientData(const Client& client, const SocketServer& server)
		{
			char sendBuff[1024] = { 0 };

			while (1)
			{
				if (int amount = recv(client.GetConnection(), sendBuff, 1024, 0))
				{
					if (amount < 0)
					{
						if (server.ErrorEvent)
							server.ErrorEvent("failed with error: " + std::to_string(WSAGetLastError()));

						return;
					}

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