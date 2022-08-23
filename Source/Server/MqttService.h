#pragma once
#include "Protocol/Converter/ConnectConverter.h"
#include "SocketServer.h"
#include "MqttClient.h"

namespace MQTT {
	namespace Server {
		class MqttService
		{
		public:
			MqttService(IServer* server);
			~MqttService();

			void Start();

		private:
			void InitialiseServer();
			void OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer);
			
			void OnClientConnect(const Client& client, const Protocol::ConnectPackage& package);

		private:
			IServer* m_Server;
			std::vector<MqttClient*> m_ClientStates;
		};
	}
}