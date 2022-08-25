#pragma once
#include "Server/SocketServer.h"
#include "Protocol/Managers/MqttManager.h"

namespace MQTT
{
	namespace Server
	{
		class MqttService {

		public:
			MqttService(){
				m_Server = new Server::SocketServer(1883);
				m_Server->OnReceivedData = std::bind(&MqttService::PackageReceived, this, std::placeholders::_1, std::placeholders::_2);
				m_Server->Start();
			}

			void PackageReceived(const MQTT::Server::Client& _client, const std::vector<unsigned char>& _message);



		private:
			Protocol::MqttManager m_MqttManager;
			Server::IServer* m_Server;
		};
	}
}