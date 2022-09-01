#pragma once

#include "Protocol/MqttPackages/Packages.h"
#include "Server/MqttClient.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {

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
				Action ValidateClient(const Protocol::ConnectPackage& package, 
					const std::vector<Server::MqttClient*>& clientStates,
					Server::MqttClient* currentClient);

			private:
				void SetCurrentClientId(const std::string& packageClientId, MQTT::Server::MqttClient* currentClient);

				bool ShouldDisconnectClient(const std::string& packageClientId, 
					const std::vector<Server::MqttClient*>& clientStates, 
					const std::string& protocolName, 
					const ConnectPackage& package, 
					Server::MqttClient* currentClient);
			};
		}
	}
}