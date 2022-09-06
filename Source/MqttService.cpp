#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Converters/ConverterUtility.h"
#include "Rules/ConnectValidator.h"
#include "Rules/PublishValidator.h"
#include "Converters/DisconnectConverter.h"
#include "Converters/PublishAcknowledgeConverter.h"
#include <algorithm>

namespace MQTT {
	MqttService::MqttService(Server::IServer* server) : m_Server(server), m_ClientStates(), m_Manager()
	{
		InitialiseServer();
	}

	MqttService::~MqttService()
	{
		for (auto clientStates : m_ClientStates)
			delete clientStates;
	}

	void MqttService::Start()
	{
		m_Server->Start();
	}

	void MqttService::Stop()
	{
		m_Server->Stop();
	}

	void MqttService::OnReceivedData(const Server::Client& client, const std::vector<unsigned char>& buffer)
	{
		auto type = Converters::ConverterUtility::GetPackageType(buffer[0]);

		switch (type)
		{
		case MqttPackages::Connect:
			OnClientConnect(
				client,
				Converters::ConnectConverter().ToPackage(buffer)
			);
			break;
		case MqttPackages::Publish:
			OnClientPublish(
				client,
				buffer
			);
			break;
		case MqttPackages::Subscribe:
			OnClientSubscribed(
				client,
				Converters::SubscribeConverter().ToPackage(buffer)
			);
			break;
		case MqttPackages::Disconnect:
			OnClientDisconnect(
				client
			);
			break;
		default:
			for (int i = 0; i < buffer.size(); i++)
			{
				printf("%02X ", buffer[i]);
			}
		}
	}

	void MqttService::OnClientConnect(const Server::Client& client, const MqttPackages::ConnectPackage& package)
	{
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
			m_Server->Disconnect(client);
			break;
		case Rules::ConnectValidator::RejectUserIdentifier:
			m_Server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Refused_Identifier_Rejected));
			break;
		case Rules::ConnectValidator::RejectProtocolLevel:
			m_Server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Refused_Unacceptable_Protocol_Version));
			break;
		case Rules::ConnectValidator::ContinueState:
			m_Server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Accepted));
			clientState->IsConnected = true;
			break;
		case Rules::ConnectValidator::CreateNewState:
			m_ClientStates.push_back(clientState);
			m_Server->Send(client, m_Manager.GenerateConnectAckMessage(MqttPackages::Accepted));
			clientState->IsConnected = true;
			break;
		}
	}
	void MqttService::OnClientSubscribed(const Server::Client& client, const MqttPackages::SubscribePackage& package)
	{
		bool validPackage = m_SubscribeManager.ValidPackage(package);
		if (!validPackage) {
			DisconnectClientState(client);
			return;
		}
		auto mqttClient = GetClientStateFromIdentifier(client.GetIdentifier());

		m_SubscribeManager.AddToSubscriptions(mqttClient->ClientId, package);

		auto buffer = m_SubscribeManager.CreateSubAckBuffer(package);

		m_Server->Send(client, buffer);
	}

	void MqttService::OnClientDisconnect(const Server::Client& client)
	{
		DisconnectClientState(client);
	}

	void MqttService::OnClientPublish(const Server::Client& client, const std::vector<unsigned char>& buffer)
	{
		auto package = Converters::PublishConverter().ToPackage(buffer);
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
			DisconnectClientState(client);
			break;
		case Rules::PublishValidator::AcknowledgePublish:
		{
			if (package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb)
				m_Server->Send(client, pulishAckConverter.ToBuffer(publishAckPackage));

			//TODO: Implement QoS 1 & 2
			std::vector<unsigned char> topic(package.VariableHeader.TopicName.begin(), package.VariableHeader.TopicName.end());
			auto subscribedClients = m_SubscribeManager.GetSubscribedClients(topic);
			for (auto& subClient : subscribedClients)
			{
				auto mqttClient = GetClientStateFromClientId(subClient.GetClientID());
				auto c = GetClientFromIdentifier(mqttClient->ConnectionIdentifier);

				if (c != nullptr)
				{
					m_Server->Send(*c, buffer);
				}
			}
		}
		break;
		case Rules::PublishValidator::DisconnectClient:
			DisconnectClientState(client);
			break;
		default:
			break;
		}
	}

	// If a client state with matching connection id exists, set is connected to false and remove will message.
	void MqttService::DisconnectClientState(const Server::Client& client)
	{
		for (auto& clientState : m_ClientStates)
		{
			if (clientState->ConnectionIdentifier == client.GetIdentifier())
			{
				clientState->IsConnected = false;
				clientState->WillMessage = "";
			}
		}
		m_Server->Disconnect(client);
	}

	Protocol::MqttClient* MqttService::GetClientStateFromClientId(const std::string& clientId)
	{
		for (auto* clientState : m_ClientStates)
			if (clientState->ClientId == clientId)
				return clientState;

		return nullptr;
	}

	Protocol::MqttClient* MqttService::GetClientStateFromIdentifier(const std::string& identifier)
	{
		for (auto* clientState : m_ClientStates)
			if (clientState->ConnectionIdentifier == identifier)
				return clientState;

		return nullptr;
	}

	Server::Client* MqttService::GetClientFromIdentifier(const std::string& identifier)
	{
		for (auto* client : m_Server->GetClients())
			if (client->GetIdentifier() == identifier)
				return client;

		return nullptr;
	}


	void MqttService::InitialiseServer()
	{
		m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
	}
}
