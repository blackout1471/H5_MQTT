#pragma once
#include "Protocol/Managers/MqttManager.h"
#include "Protocol/Converters/ConnectConverter.h"
#include "IServer.h"
#include "MqttClient.h"

namespace MQTT {
	namespace Server {
		class MqttService {

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
			Protocol::MqttManager m_Manager;
			std::vector<MqttClient*> m_ClientStates;
		};
	}
}