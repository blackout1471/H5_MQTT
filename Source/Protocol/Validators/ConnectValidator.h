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
					RejectUser,
					ContinueState,
					CreateNewState
				};
			public:
				ConnectValidator();
				virtual ~ConnectValidator();

				Action ValidateClient(const Protocol::ConnectPackage& package, 
					const std::vector<Server::MqttClient*>& clientStates,
					Server::MqttClient* currentClient);

			private:

				void SetCurrentClientId(const std::string& packageClientId, MQTT::Server::MqttClient* currentClient);

				ConnectValidator::Action CalculateClientSessionState(Server::MqttClient*& currentClient, 
					const std::string& packageClientId, 
					const std::vector<Server::MqttClient*>& clientStates);

				bool ShouldDisconnectClient(const std::string& packageClientId, 
					const std::vector<Server::MqttClient*>& clientStates, 
					const std::string& protocolName, 
					const ConnectPackage& package, 
					Server::MqttClient* currentClient);
			};
		}
	}
}