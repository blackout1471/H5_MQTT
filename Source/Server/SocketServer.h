#pragma once
#include "IServer.h"

#include <thread>

namespace MQTT {
	namespace Server {
		class SocketServer : public IServer
		{
		public:
			// Creates the server with the given port.
			SocketServer(int port);
			~SocketServer();

			// <inheritDoc />
			virtual void Start() override;
			// <inheritDoc />
			virtual void Stop() override;
			// <inheritDoc />
			virtual void Disconnect(const Client& client) override;
			// <inheritDoc />
			virtual void Send(const Client& client, const std::vector<unsigned char>& data) override;

		private:
			// Configures the socket servers port and address.
			void ConfigureAddressInfo(int port);
			// Creates and assigns the socket in the member variable.
			void CreateSocket();
			// Begin the binding of the socket.
			void SetupTCP();
			// Begins the listening for the socket.
			void Listen();
			// Accepts new clients and creates a thread for receiving data.
			void Accept();

			static void ReadClientData(const Client& client, const SocketServer& server);
		private:
			bool m_IsRunning;
			int m_Port;
			int m_Socket;
			// TODO: Create struct where both clients and threads are in it.
			std::vector<Client> m_Clients;
			std::vector<std::thread> m_ClientReaderThreads;
		};
	}
};