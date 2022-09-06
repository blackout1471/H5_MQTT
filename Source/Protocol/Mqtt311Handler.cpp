#include "mqttpch.h"
#include "Mqtt311Handler.h"

#include "Rules/Rules.h"
#include "Rules/RuleEngine.h"
#include "Rules/ConnectValidator.h"
#include "Rules/PublishValidator.h"

#include "Converters/ConnectConverter.h"
#include "Converters/PublishAcknowledgeConverter.h"
#include "Converters/PublishConverter.h"

namespace MQTT {
	namespace Protocol {
		Mqtt311Handler::Mqtt311Handler() : m_ClientStates(), m_Manager(), m_SubscribeManager()
		{
		}

		Mqtt311Handler::~Mqtt311Handler()
		{
			for (auto& clientState : m_ClientStates)
				delete clientState;
		}

		void Mqtt311Handler::OnClientConnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer)
		{
			auto package = Converters::ConnectConverter().ToPackage(buffer);
			if (OnRequestEvent) OnRequestEvent(MqttPackages::Connect, client, &package);

			auto& packageClientId = package.Payload.ClientId;
			auto& protocolName = package.VariableHeader.ProtocolName;
			auto* clientState = new Protocol::MqttClient();

			clientState->ConnectionFlags = package.VariableHeader.VariableLevel;

			auto action = Rules::ConnectValidator()
				.ValidateClient(package, m_ClientStates, clientState);

			clientState->ConnectionIdentifier = client.GetIdentifier();

			switch (action)
			{
			case Rules::ConnectValidator::Disconnect:
				server->Disconnect(client);
				break;
			case Rules::ConnectValidator::RejectUserIdentifier:
				server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Refused_Identifier_Rejected));
				break;
			case Rules::ConnectValidator::RejectProtocolLevel:
				server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Refused_Unacceptable_Protocol_Version));
				break;
			case Rules::ConnectValidator::ContinueState:
				server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Accepted));
				clientState->IsConnected = true;
				break;
			case Rules::ConnectValidator::CreateNewState:
				m_ClientStates.push_back(clientState);
				server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Accepted));
				clientState->IsConnected = true;
				break;
			}
		}

		void Mqtt311Handler::OnClientDisconnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer)
		{
			if (OnRequestEvent) OnRequestEvent(MqttPackages::Disconnect, client, nullptr);

			DisconnectClientState(client, server);
		}

		void Mqtt311Handler::OnClientPublished(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer)
		{
			auto package = Converters::PublishConverter().ToPackage(buffer);
			if (OnRequestEvent) OnRequestEvent(MqttPackages::Publish, client, &package);

			auto copyPackage = package;
			auto mqttClientState = GetClientStateFromIdentifier(client.GetIdentifier());

			auto action = Rules::PublishValidator()
				.ValidatePackage(copyPackage, *mqttClientState, m_SubscribeManager);

			auto pulishAckConverter = Converters::PublishAcknowledgeConverter();
			MqttPackages::PublishAcknowledgePackage publishAckPackage;
			publishAckPackage.Header.PackageType = MqttPackages::ControlPackageType::PublAck;
			publishAckPackage.PacketIdentifier = package.VariableHeader.PacketIdentifier;

			switch (action)
			{
			case Rules::PublishValidator::RejectPublish:
				DisconnectClientState(client, server);
				break;
			case Rules::PublishValidator::AcknowledgePublish:
			{
				if (package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb)
					server->Send(client, pulishAckConverter.ToBuffer(publishAckPackage));

				//TODO: Implement QoS 1 & 2
				std::vector<unsigned char> topic(package.VariableHeader.TopicName.begin(), package.VariableHeader.TopicName.end());
				auto subscribedClients = m_SubscribeManager.GetSubscribedClients(topic);
				for (auto& subClient : subscribedClients)
				{
					auto mqttClient = GetClientStateFromClientId(subClient.GetClientID());
					auto c = GetClientFromIdentifier(mqttClient->ConnectionIdentifier, server);

					if (c != nullptr)
					{
						server->Send(*c, buffer);
					}
				}
			}
			break;
			case Rules::PublishValidator::DisconnectClient:
				DisconnectClientState(client, server);
				break;
			default:
				break;
			}
		}

		void Mqtt311Handler::OnClientSubscribed(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer)
		{
			auto package = Converters::SubscribeConverter().ToPackage(buffer);
			if (OnRequestEvent) OnRequestEvent(MqttPackages::Subscribe, client, &package);

			bool validPackage = m_SubscribeManager.ValidPackage(package);
			if (!validPackage) {
				DisconnectClientState(client, server);
				return;
			}
			auto mqttClient = GetClientStateFromIdentifier(client.GetIdentifier());

			m_SubscribeManager.AddToSubscriptions(mqttClient->ClientId, package);

			auto sendBuffer = m_SubscribeManager.CreateSubAckBuffer(package);

			server->Send(client, sendBuffer);
		}

		// If a client state with matching connection id exists, set is connected to false and remove will message.
		void Mqtt311Handler::DisconnectClientState(const Server::Client& client, Server::IServer* server)
		{
			for (auto& clientState : m_ClientStates)
			{
				if (clientState->ConnectionIdentifier == client.GetIdentifier())
				{
					clientState->IsConnected = false;
					clientState->WillMessage = "";
				}
			}
			server->Disconnect(client);
		}

		Protocol::MqttClient* Mqtt311Handler::GetClientStateFromClientId(const std::string& clientId)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ClientId == clientId)
					return clientState;

			return nullptr;
		}

		Protocol::MqttClient* Mqtt311Handler::GetClientStateFromIdentifier(const std::string& identifier)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ConnectionIdentifier == identifier)
					return clientState;

			return nullptr;
		}

		Server::Client* Mqtt311Handler::GetClientFromIdentifier(const std::string& identifier, const Server::IServer* server)
		{
			for (auto* client : server->GetClients())
				if (client->GetIdentifier() == identifier)
					return client;

			return nullptr;
		}
	}
}