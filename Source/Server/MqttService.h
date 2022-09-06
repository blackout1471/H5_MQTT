#pragma once
#include "Protocol/Managers/MqttManager.h"
#include "Protocol/Converters/ConnectConverter.h"
#include "Protocol/Converters/PublishConverter.h"
#include "IServer.h"
#include "MqttClient.h"
#include "Protocol/Managers/SubscribeManager.h"
#include "MqttPackages/Packages.h"
namespace MQTT {
	namespace Server {
		class MqttService {

		public:
			/*
			* Creates the service with the specified server.
			*/
			MqttService(IServer* server);
			~MqttService();

			// Starts the server and begin goes in a loop to listen for incoming clients.
			void Start();

			/*
			* Stops the server and disconnects all clients.
			*/
			void Stop();

		private:
			/*
			* Initialises the server and sets the required data and events.
			*/
			void InitialiseServer();

			/*
			* Called when the server recieves data.
			*/
			void OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer);

			//Sets mqtt client state to diconnected and diconnects socket client.
			void DisconnectClientState(const Client& client);

			/*
			* Called when the recieved data has been identified as a connect package.
			*/
			void OnClientConnect(const Client& client, const MqttPackages::ConnectPackage& package);

			/*
			* Called when the recieved data has been identified as a Subscribe package.
			*/
			void OnClientSubscribed(const Client& client, const MqttPackages::SubscribePackage& package);

			/*
			* Called when the recieved data has been identified as a disconnect package.
			*/
			void OnClientDisconnect(const Client& client);
      
      /*
			* Called when the recieved data has been identified as a publish package.
			*/
			void OnClientPublish(const Client& client, const std::vector<unsigned char>& buffer);
      
			/*
			* Retrieves the client state from the given client id.
			* returns: nullptr if not found.
			*/
			MqttClient* GetClientStateFromClientId(const std::string& clientId);

			/*
			* Retrieves the client state from the given identifier.
			* Returns: clientstate or nullptr if not found.
			*/
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