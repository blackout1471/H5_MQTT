#pragma once
#include "Protocol/Managers/MqttManager.h"
#include "Protocol/Converters/ConnectConverter.h"
#include "IServer.h"
#include "MqttClient.h"
#include "Protocol/Converters/SubscribeConverter.h"
#include "Protocol/Managers/SubscribeManager.h"
#include "Protocol/MqttPackages/Packages.h"
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
			//Sets mqtt client state to diconnected and diconnects socket client.
			void DisconnectClientState(const Client& client);

			void OnClientConnect(const Client& client, const Protocol::ConnectPackage& package);
			void OnClientSubscribed(const Client& client, const Protocol::SubscribePackage& package);
			void OnClientDisconnect(const Client& client, const Protocol::DisconnectPackage& package);


			MqttClient* GetClientState(const std::string& clientId);
		private:
			IServer* m_Server;
			Protocol::MqttManager m_Manager;
			std::vector<MqttClient*> m_ClientStates;
			IServer* m_Server;
			Protocol::MqttManager m_Manager;
			Protocol::SubscribeManager m_SubscribeManager;
			std::vector<MqttClient*> m_ClientStates;
		};
	}
}