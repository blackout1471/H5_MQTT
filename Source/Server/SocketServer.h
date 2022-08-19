#pragma once
#include "IServer.h"
namespace MQTT {
	namespace Server {
		class SocketServer : IServer
		{
		public:
			SocketServer(const int& port) : m_Port(port) {};
			~SocketServer() {};
			// Inherited via IServer
			virtual void Start() override;
			virtual void Stop() override;
			virtual void Disconnect(const Client& client) override;
			virtual void SendMessage(const Client& client, const std::vector<unsigned char>& data) override;
		private:
			int m_Port;
			std::vector<Client> m_Clients;
		};
	}
}

