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
		struct sockaddr_in serv_addr = { 0 };
		static struct addrinfo hints;
		static struct addrinfo* result = NULL;

		SocketServer::SocketServer(int port)
			: m_Port(port), m_IsRunning(false), m_Socket(0) {};

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

			freeaddrinfo(result);
			closesocket(m_Socket);
			WSACleanup();
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
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(client.GetConnection());
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
				s_Initialized = false;
				//Throw error
			}
		}

		// Create a SOCKET for the server to listen for client connections.
		void SocketServer::CreateSocket() {

			m_Socket = socket(AF_INET, SOCK_STREAM, 0);

			if (m_Socket == INVALID_SOCKET) {
				//Logging here instead of printf
				printf("socket failed with error: %ld\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				s_Initialized = false;
				//throw error here
			}
		}

		//Setup the TCP listening socket
		void SocketServer::SetupTCP() {

			int iResult = bind(m_Socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

			if (iResult == SOCKET_ERROR) {

				//Logging here
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(m_Socket);
				WSACleanup();
				s_Initialized = false;
				//Throw error here
			}

			freeaddrinfo(result);
		}

		void SocketServer::Listen() {
			int iResult = listen(m_Socket, 10);
			if (iResult == SOCKET_ERROR) {
				//logging here
				printf("listen failed with error: %d\n", WSAGetLastError());
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
				m_Clients.push_back(new Client("123", GenerateUniqueId(), clientSocket));
				m_ClientReaderThreads.push_back(std::thread(SocketServer::ReadClientData, std::cref(*m_Clients[m_Clients.size() - 1]), std::cref(*this)));
			}
			else
			{
				//logging here
				printf("accept failed with error: %d\n", WSAGetLastError());
				//throw error
			}

		}
		std::string SocketServer::GenerateUniqueId()
		{
			GUID identifier;
			CoCreateGuid(&identifier);

			char guid_cstr[39];
			snprintf(guid_cstr, sizeof(guid_cstr),
				"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
				identifier.Data1, identifier.Data2, identifier.Data3,
				identifier.Data4[0], identifier.Data4[1], identifier.Data4[2], identifier.Data4[3],
				identifier.Data4[4], identifier.Data4[5], identifier.Data4[6], identifier.Data4[7]);

			return std::string(guid_cstr);
			
		}
		void SocketServer::ReadClientData(const Client& client, const SocketServer& server)
		{
			char sendBuff[64] = { 0 };

			while (1)
			{
				if (int amount = recv(client.GetConnection(), sendBuff, 64, 0))
				{
					if (amount < 0)
					{
						printf("error: %d\n", WSAGetLastError());
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