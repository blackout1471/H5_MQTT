#include "mqttpch.h"
#include "ConnectValidator.h"

#include "Protocol/Validators/RuleEngine.h"
#include "Protocol/Validators/Rules/Rules.h"
using namespace MQTT::Protocol::Validators;

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			ConnectValidator::ConnectValidator()
			{
			}
			ConnectValidator::~ConnectValidator()
			{
			}

			ConnectValidator::Action ConnectValidator::ValidateClient(const Protocol::ConnectPackage& package, const std::vector<Server::MqttClient*>& clientStates, Server::MqttClient* currentClient)
			{
				auto& packageClientId = package.ConnectPayload.ClientId;
				auto& protocolName = package.ConnectVariableHeader.ProtocolName;

				auto shouldDisconnect = ShouldDisconnectClient(packageClientId, clientStates, protocolName, package, currentClient);
				if (shouldDisconnect)
					return Disconnect;

				auto protocolIsCorrect = Protocol311Rule(package.ConnectVariableHeader.Level).Validate();
				if (!protocolIsCorrect)
					return RejectProtocolLevel;

				auto clientIdentifierIsCorrect = ConnectClientIdentifierValidRule(packageClientId, package.ConnectVariableHeader.VariableLevel).Validate();
				if (!clientIdentifierIsCorrect)
					return RejectUserIdentifier;

				auto shouldContinueSession = ContinueSessionRule(package.ConnectVariableHeader.VariableLevel, packageClientId, clientStates).Validate();
				if (shouldContinueSession)
				{
					delete currentClient;
					currentClient = Server::FindClient(packageClientId, clientStates);
					return ContinueState;
				}

				SetCurrentClientId(packageClientId, currentClient);

				return CreateNewState;
			}

			void ConnectValidator::SetCurrentClientId(const std::string& packageClientId, Server::MqttClient* currentClient)
			{
				if (packageClientId.size() == 0)
					currentClient->ClientId = "1"; // TODO :: Generate unique id
				else
					currentClient->ClientId = packageClientId;
			}

			bool ConnectValidator::ShouldDisconnectClient(const std::string& packageClientId, const std::vector<Server::MqttClient*>& clientStates, const std::string& protocolName, const ConnectPackage& package, Server::MqttClient* currentClient)
			{
				return !(RuleEngine({
					{ new ClientConnectedRule(packageClientId, clientStates), false },
					{ new CorrectProtocolNameRule(protocolName), true },
					{ new ConnectReservedFlagSetRule(package.ConnectVariableHeader.VariableLevel), false },
					{ new IsCredentialFlagIncorrectRule(package.ConnectVariableHeader.VariableLevel), false },
					{ new ConnectWillRule(currentClient, package.ConnectVariableHeader.VariableLevel, package.ConnectPayload.WillMessage), true }
				}).Run());
			}



		}
	}
}