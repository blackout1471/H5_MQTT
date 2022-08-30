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

			// Starts the server and begin goes in a loop to listen for incoming clients.
			void Start();

		private:
			void InitialiseServer();
			void OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer);

			void OnClientConnect(const Client& client, const Protocol::ConnectPackage& package);


			MqttClient* GetClientState(const std::string& clientId);
			std::string GenerateUniqueId();
		private:
			IServer* m_Server;
			Protocol::MqttManager m_Manager;
			std::vector<MqttClient*> m_ClientStates;
		};
	}
}