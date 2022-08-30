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

				auto shouldContinueSession = (RuleEngine({
					{new ContinueSessionRule(package.ConnectVariableHeader.VariableLevel, packageClientId, clientStates), true}
				}).Run());

				if (shouldContinueSession)
					return CalculateClientSessionState(currentClient, packageClientId, clientStates);

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

			ConnectValidator::Action ConnectValidator::CalculateClientSessionState(Server::MqttClient*& currentClient, const std::string& packageClientId, const std::vector<Server::MqttClient*>& clientStates)
			{
				delete currentClient;
				currentClient = Server::FindClient(packageClientId, clientStates);

				auto canContinueSession = (RuleEngine({
					{ new CanClientContinueSession(packageClientId, currentClient), true }
					}).Run());

				if (canContinueSession)
					return ContinueState;
				else
					return RejectUser;
			}

			bool ConnectValidator::ShouldDisconnectClient(const std::string& packageClientId, const std::vector<Server::MqttClient*>& clientStates, const std::string& protocolName, const ConnectPackage& package, Server::MqttClient* currentClient)
			{
				return !(RuleEngine({
					{ new ClientConnectedRule(packageClientId, clientStates), false },
					{ new CorrectProtocolNameRule(protocolName), true },
					{ new Protocol311Rule(package.ConnectVariableHeader.Level), true },
					{ new ConnectReservedFlagSetRule(package.ConnectVariableHeader.VariableLevel), false },
					{ new IsCredentialFlagIncorrectRule(package.ConnectVariableHeader.VariableLevel), false },
					{ new ConnectWillRule(currentClient, package.ConnectVariableHeader.VariableLevel, package.ConnectPayload.WillMessage), true }
				}).Run());
			}



		}
	}
}