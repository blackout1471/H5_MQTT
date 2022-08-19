#include "mqttpch.h"
#include "SocketServer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
namespace MQTT {
	namespace Server {
		static WSADATA s_WsaData;
		static bool s_Initialized = false;
		static SOCKET ListenSocket = INVALID_SOCKET;
		static struct addrinfo hints;
		static struct addrinfo* result = NULL;
		void SocketServer::Start() 
		{
			
			int iResult;

			Init();

			//Configures info on hints and res
			ConfigureAddressInfo(m_Port);
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

		//Initialization
		void Init() {
			if (!s_Initialized)
			{
				int iResult = WSAStartup(MAKEWORD(2, 2), &s_WsaData);
				if (iResult != 0) {
					//Logging here
					//throw error
				}
				s_Initialized = true;
			}
		}

		void ConfigureAddressInfo(int port) {

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_flags = AI_PASSIVE;

			// Resolve the server address and port
			int iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
			if (iResult != 0) {
				//Logging here
				WSACleanup();
				//Throw error
			}
		}
	}
}