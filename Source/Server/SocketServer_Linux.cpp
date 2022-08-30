#include "mqttpch.h"
#include "SocketServer.h"
#include "ClientUtility.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace MQTT {
	namespace Server {

		void SocketServer::ReadClientData(const Client& client, const SocketServer& server)
		{
			unsigned char sendBuff[64] = { 0 };

			while (1)
			{
				if (int amount = recv(client.GetConnection(), sendBuff, 64, 0))
				{
					if (server.OnReceivedData)
					{
						auto data = std::vector<unsigned char>();
						data.insert(data.end(), &sendBuff[0], &sendBuff[amount]);
						server.OnReceivedData(client, data);
					}
				}
			}
		}

		struct sockaddr_in serv_addr = { 0 };

		SocketServer::SocketServer(int port)
			: m_Port(port), m_Socket(0), m_Clients(), m_ClientReaderThreads(), m_IsRunning(false) {};

		SocketServer::~SocketServer()
		{
			shutdown(m_Socket, SHUT_RDWR);
		};

		void SocketServer::ConfigureAddressInfo(int port)
		{
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
			serv_addr.sin_port = htons(port);
		}

		void SocketServer::CreateSocket()
		{
			m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		}

		void SocketServer::SetupTCP()
		{
			bind(m_Socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		}

		void SocketServer::Listen()
		{
			listen(m_Socket, 10);
		}

		void SocketServer::Accept()
		{
			int connfd = accept(m_Socket, (struct sockaddr*)NULL, NULL);
			if (connfd > 0)
			{
				m_Clients.push_back(new Client("123", GenerateUniqueId(), connfd));
				m_ClientReaderThreads.push_back(std::thread(SocketServer::ReadClientData, std::cref(*m_Clients[m_Clients.size() - 1]), std::cref(*this)));
			}
		}

		void SocketServer::Start()
		{
			m_IsRunning = true;

			//Configuration
			ConfigureAddressInfo(m_Port);

			//Creates a listening socket
			CreateSocket();

			//Sets up TCP for listening socket
			SetupTCP();

			//Starts listening for a client
			Listen();

			while (m_IsRunning)
			{
				Accept();
			}

			m_ClientReaderThreads.clear();
			for (const auto& client : m_Clients)
				Disconnect(*client);

			m_Clients.clear();
		}

		void SocketServer::Stop()
		{
			m_IsRunning = false;
		}

		void SocketServer::Disconnect(const Client& client)
		{
			// TODO: Delete client from list
			close(client.GetConnection());
		}

		void SocketServer::Send(const Client& client, const std::vector<unsigned char>& data)
		{
			send(client.GetConnection(), data.data(), data.size(), 0);
		}
	}
}