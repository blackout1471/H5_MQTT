#pragma once
#include "Protocol/Managers/MqttManager.h"
#include "Protocol/Converters/ConnectConverter.h"
#include "Protocol/Converters/PublishConverter.h"
#include "IServer.h"
#include "MqttClient.h"
#include "Protocol/Managers/SubscribeManager.h"
#include "Protocol/MqttPackages/Packages.h"
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
			//Sets mqtt client state to diconnected and diconnects socket client.
			void DisconnectClientState(const Client& client);

			void OnClientConnect(const Client& client, const Protocol::ConnectPackage& package);
			void OnClientSubscribed(const Client& client, const Protocol::SubscribePackage& package);
			void OnClientDisconnect(const Client& client);
			void OnClientPublish(const Client& client, const Protocol::PublishPackage& package);


			MqttClient* GetClientStateFromClientId(const std::string& clientId);
			MqttClient* GetClientStateFromIdentifier(const std::string& identifier);
			Client* GetClientFromIdentifier(const std::string& identifier);
		private:
			IServer* m_Server;
			Protocol::MqttManager m_Manager;
			std::vector<MqttClient*> m_ClientStates;
			Protocol::SubscribeManager m_SubscribeManager;
		};
	}
}