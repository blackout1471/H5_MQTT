#include "mqttpch.h"
#include "SocketServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace MQTT {
	namespace Server {

        SocketServer::SocketServer(int port) 
				: m_Port(port), m_Socket(0), m_Clients() {};

        SocketServer::~SocketServer() 
        {
            Stop();  
        };

		void SocketServer::Start()
		{
            struct sockaddr_in serv_addr = {0};

            unsigned char sendBuff[64] = {0};

            m_Socket = socket(AF_INET, SOCK_STREAM, 0);

            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            serv_addr.sin_port = htons(m_Port);

            bind(m_Socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

            listen(m_Socket, 10);

            int connfd = 0;
            while (connfd == 0)
            {
                connfd = accept(m_Socket, (struct sockaddr*)NULL, NULL);
                m_Clients.push_back(Client("123", "1", connfd));
            }

            while (1)
            {
                for(const auto& client : m_Clients)
                {
                    if (int amount = recv(connfd, sendBuff, 64, 0))
                    {
                        if (OnReceivedData)
                        {
                            auto data = std::vector<unsigned char>();
                            data.insert(data.end(), &sendBuff[0], &sendBuff[amount]);
                            OnReceivedData(client, data);
                        }
                    }
                }
            }
		}

		void SocketServer::Stop()
		{
            for(const auto& client : m_Clients)
                Disconnect(client);

            shutdown(m_Socket, SHUT_RDWR);
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