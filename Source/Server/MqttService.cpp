#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Protocol/Converters/ConverterUtility.h"
#include "Protocol/Validators/ConnectValidator.h"
#include "Protocol/Validators/PublishValidator.h"
#include "Protocol/Converters/DisconnectConverter.h"

#include <algorithm>

namespace MQTT {
	namespace Server {
		MqttService::MqttService(IServer* server) : m_Server(server), m_ClientStates(), m_Manager()
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

		void MqttService::OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer)
		{
			auto type = Protocol::Converters::ConverterUtility::GetPackageType(buffer[0]);

			switch (type)
			{
			case MQTT::Protocol::Connect:
				OnClientConnect(
					client, 
					Protocol::Converters::ConnectConverter().ToPackage(buffer)
				);
				break;
			case MQTT::Protocol::ConnectAck:
				break;
			case MQTT::Protocol::Publish:
				OnClientPublish(
					client,
					Protocol::Converters::PublishConverter().ToPackage(buffer)
				);
				break;
			case MQTT::Protocol::PublAck:
				break;
			case MQTT::Protocol::PubRec:
				break;
			case MQTT::Protocol::PubRel:
				break;
			case MQTT::Protocol::PubComp:
				break;
			case MQTT::Protocol::Subscribe:
				break;
			case MQTT::Protocol::SubAck:
				break;
			case MQTT::Protocol::Unsubscribe:
				break;
			case MQTT::Protocol::UnsubAck:
				break;
			case MQTT::Protocol::PingReq:
				break;
			case MQTT::Protocol::PingResp:
				break;
			case MQTT::Protocol::Disconnect:
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

		void MqttService::OnClientConnect(const Client& client, const Protocol::ConnectPackage& package)
		{
			auto& packageClientId = package.Payload.ClientId;
			auto& protocolName = package.VariableHeader.ProtocolName;
			auto* clientState = new MqttClient();
			
			clientState->ConnectionFlags = package.VariableHeader.VariableLevel;

			auto action = Protocol::Validators::ConnectValidator()
				.ValidateClient(package, m_ClientStates, clientState);

			clientState->ConnectionIdentifier = client.GetIdentifier();

			switch (action)
			{
			case MQTT::Protocol::Validators::ConnectValidator::Disconnect:
				m_Server->Disconnect(client);
				break;
			case MQTT::Protocol::Validators::ConnectValidator::RejectUserIdentifier:
				m_Server->Send(client, m_Manager.GenerateConnectAckMessage(Protocol::Refused_Identifier_Rejected));
				break;
			case MQTT::Protocol::Validators::ConnectValidator::RejectProtocolLevel:
				m_Server->Send(client, m_Manager.GenerateConnectAckMessage(Protocol::Refused_Unacceptable_Protocol_Version));
				break;
			case MQTT::Protocol::Validators::ConnectValidator::ContinueState:
				m_Server->Send(client, m_Manager.GenerateConnectAckMessage(Protocol::Accepted));
				clientState->IsConnected = true;
				break;
			case MQTT::Protocol::Validators::ConnectValidator::CreateNewState:
				m_ClientStates.push_back(clientState);
				m_Server->Send(client, m_Manager.GenerateConnectAckMessage(Protocol::Accepted));
				clientState->IsConnected = true;
				break;
			}
		}

		void MqttService::OnClientDisconnect(const Client& client)
		{
			//TODO: Remove will message when storage of it is implemented.
			DisconnectClientState(client);
		}

		void MqttService::OnClientPublish(const Client& client, const Protocol::PublishPackage& package)
		{
			auto copyPackage = package;
			auto mqttClientState = GetClientStateFromIdentifier(client.GetIdentifier());

			auto action = Protocol::Validators::PublishValidator()
				.ValidatePackage(copyPackage, *mqttClientState);

			switch (action)
			{
			case MQTT::Protocol::Validators::PublishValidator::RejectPublish:
				// TODO :: Reject publish package.
				break;
			case MQTT::Protocol::Validators::PublishValidator::AcknowledgePublish:
				// Todo :: Acknowledge publish package
				break;
			case MQTT::Protocol::Validators::PublishValidator::DisconnectClient:
				DisconnectClientState(client);
				break;
			default:
				break;
			}
		}

		void MqttService::DisconnectClientState(const Client& client)
		{
			for (auto& clientState : m_ClientStates)
			{
				if (clientState->ConnectionIdentifier == client.GetIdentifier())
					clientState->IsConnected = false;
			}
			m_Server->Disconnect(client);
		}

		MqttClient* MqttService::GetClientStateFromClientId(const std::string& clientId)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ClientId == clientId)
					return clientState;

			return nullptr;
		}

		MqttClient* MqttService::GetClientStateFromIdentifier(const std::string& identifier)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ConnectionIdentifier == identifier)
					return clientState;

			return nullptr;
		}

		void MqttService::InitialiseServer()
		{
			m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}