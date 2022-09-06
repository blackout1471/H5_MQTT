#pragma once

#include "MqttPackages/Packages.h"
#include "Server/MqttClient.h"

namespace MQTT {
	namespace Rules {
		class ConnectValidator
		{
		public:
			enum Action {
				Disconnect,
				RejectUserIdentifier,
				RejectProtocolLevel,
				ContinueState,
				CreateNewState
			};
		public:
			ConnectValidator();
			virtual ~ConnectValidator();

			// Validates the current connect package and the current client, which action is the most properiate to take
			// Returns: The action to take.
			Action ValidateClient(const MqttPackages::ConnectPackage& package,
				std::vector<Server::MqttClient*>& clientStates,
				Server::MqttClient*& currentClient);

		private:
			/*
			* If clients client id is empty, then generate and set to state.
			*/
			void SetCurrentClientId(const std::string& packageClientId, MQTT::Server::MqttClient* currentClient);

			/*
			* Whether the client should disconnect.
			* Returns: true if it should, false otherwise.
			*/
			bool ShouldDisconnectClient(const std::string& packageClientId, 
				const std::vector<Server::MqttClient*>& clientStates, 
				const std::string& protocolName, 
				const MqttPackages::ConnectPackage& package,
				Server::MqttClient* currentClient);
		};
	}
}