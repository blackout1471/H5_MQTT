#pragma once
#include "IServer.h"
namespace MQTT {
	namespace Server {
		class SocketServer : public IServer
		{
		public:
			SocketServer(int port);
			~SocketServer();
			// Inherited via IServer
			virtual void Start() override;
			virtual void Stop() override;
			virtual void Disconnect(const Client& client) override;
			virtual void Send(const Client& client, const std::vector<unsigned char>& data) override;

		private:
			virtual void ConfigureAddressInfo(int port);
			virtual void CreateSocket();
			virtual void SetupTCP();
			virtual void Listen();
			virtual void Accept();

			static void ReadClientData(const Client& client, const SocketServer& server);
		private:
			static void ReadClientData(const Client& client, const SocketServer& server);
		private:
			int m_Port;
			int m_Socket;
			std::vector<Client> m_Clients;
		};
	}
};