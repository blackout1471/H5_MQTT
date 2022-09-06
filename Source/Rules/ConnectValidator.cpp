#include "mqttpch.h"
#include "ConnectValidator.h"

#include "RuleEngine.h"
#include "Rules.h"
#include "Server/ClientUtility.h"
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Rules {
		ConnectValidator::ConnectValidator() {}
		ConnectValidator::~ConnectValidator() {}

		ConnectValidator::Action ConnectValidator::ValidateClient(const MqttPackages::ConnectPackage& package, std::vector<Server::MqttClient*>& clientStates, Server::MqttClient*& currentClient)
		{
			auto& packageClientId = package.Payload.ClientId;
			auto& protocolName = package.VariableHeader.ProtocolName;

			auto shouldDisconnect = ShouldDisconnectClient(packageClientId, clientStates, protocolName, package, currentClient);
			if (shouldDisconnect)
				return Disconnect;

			auto protocolIsCorrect = Protocol311Rule(package.VariableHeader.Level).Validate();
			if (!protocolIsCorrect)
				return RejectProtocolLevel;

			auto clientIdentifierIsCorrect = ConnectClientIdentifierValidRule(packageClientId, package.VariableHeader.VariableLevel).Validate();
			if (!clientIdentifierIsCorrect)
				return RejectUserIdentifier;

			auto shouldContinueSession = ContinueSessionRule(package.VariableHeader.VariableLevel, packageClientId, clientStates).Validate();
			if (shouldContinueSession)
			{
				delete currentClient;
				currentClient = Server::FindClient(packageClientId, clientStates);
				return ContinueState;
			}

			auto shouldDeleteOldSession = OldSessionExistsAndCleanSession(package.VariableHeader.VariableLevel, packageClientId, clientStates).Validate();
			if (shouldDeleteOldSession)
				Server::DeleteClient(packageClientId, clientStates);

			SetCurrentClientId(packageClientId, currentClient);

			return CreateNewState;
		}

		void ConnectValidator::SetCurrentClientId(const std::string& packageClientId, Server::MqttClient* currentClient)
		{
			if (packageClientId.size() == 0)
				currentClient->ClientId = Server::ClientUtility::GenerateUniqueId();
			else
				currentClient->ClientId = packageClientId;
		}

		bool ConnectValidator::ShouldDisconnectClient(const std::string& packageClientId, const std::vector<Server::MqttClient*>& clientStates, const std::string& protocolName, const MqttPackages::ConnectPackage& package, Server::MqttClient* currentClient)
		{
			return !(RuleEngine({
				{ new ClientConnectedRule(packageClientId, clientStates), false },
				{ new CorrectProtocolNameRule(protocolName), true },
				{ new ConnectReservedFlagSetRule(package.VariableHeader.VariableLevel), false },
				{ new IsCredentialFlagIncorrectRule(package.VariableHeader.VariableLevel), false },
				{ new ConnectWillRule(currentClient, package.VariableHeader.VariableLevel, package.Payload.WillMessage), true }
				}).Run());
		}
	}
}