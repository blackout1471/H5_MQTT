#pragma once
#include "Protocol/Managers/MqttManager.h"
#include "Converters/ConnectConverter.h"
#include "Converters/PublishConverter.h"
#include "Server/IServer.h"
#include "Protocol/MqttClient.h"
#include "Protocol/Managers/SubscribeManager.h"
#include "MqttPackages/Packages.h"

#include "Protocol/IProtocolHandler.h"

namespace MQTT {
	class MqttService {

	public:
		/*
		* Creates the service with the specified server.
		*/
		MqttService(Server::IServer* server, Protocol::IProtocolHandler* handler);
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
		void OnReceivedData(const Server::Client& client, const std::vector<unsigned char>& buffer);

		//Sets mqtt client state to diconnected and diconnects socket client.
		void DisconnectClientState(const Server::Client& client);

		/*
		* Called when the recieved data has been identified as a connect package.
		*/
		void OnClientConnect(const Server::Client& client, const MqttPackages::ConnectPackage& package);

		/*
		* Called when the recieved data has been identified as a Subscribe package.
		*/
		void OnClientSubscribed(const Server::Client& client, const MqttPackages::SubscribePackage& package);

		/*
		* Called when the recieved data has been identified as a disconnect package.
		*/
		void OnClientDisconnect(const Server::Client& client);

		/*
			  * Called when the recieved data has been identified as a publish package.
			  */
		void OnClientPublish(const Server::Client& client, const std::vector<unsigned char>& buffer);

		/*
		* Retrieves the client state from the given client id.
		* returns: nullptr if not found.
		*/
		Protocol::MqttClient* GetClientStateFromClientId(const std::string& clientId);

		/*
		* Retrieves the client state from the given identifier.
		* Returns: clientstate or nullptr if not found.
		*/
		Protocol::MqttClient* GetClientStateFromIdentifier(const std::string& identifier);
		Server::Client* GetClientFromIdentifier(const std::string& identifier);
	private:
		Server::IServer* m_Server;
		Protocol::MqttManager m_Manager;
		Protocol::IProtocolHandler* m_ProtocolHandler;
		std::vector<Protocol::MqttClient*> m_ClientStates;
		Protocol::SubscribeManager m_SubscribeManager;
	};
}