#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Protocol/Converters/ConverterUtility.h"
#include "Protocol/Validators/ConnectValidator.h"
#include "Protocol/Validators/PublishValidator.h"
#include "Protocol/Converters/DisconnectConverter.h"
#include "Protocol/Converters/PublishAckConverter.h"
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
				OnClientSubscribed(
					client,
					Protocol::Converters::SubscribeConverter().ToPackage(buffer)
				);
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
		void MqttService::OnClientSubscribed(const Client& client, const Protocol::SubscribePackage& package)
		{
			bool validPackage = m_SubscribeManager.ValidPackage(package);

			if (!validPackage) {
				DisconnectClientState(client);
				return;
			}

			m_SubscribeManager.AddToSubscriptions(client.GetIdentifier(), package);

			auto buffer = m_SubscribeManager.CreateSubAckBuffer(package);

			m_Server->Send(client, buffer);
		}

		void MqttService::OnClientDisconnect(const Client& client)
		{
			DisconnectClientState(client);
		}

		void MqttService::OnClientPublish(const Client& client, const Protocol::PublishPackage& package)
		{
			auto copyPackage = package;
			auto mqttClientState = GetClientStateFromIdentifier(client.GetIdentifier());

			auto action = Protocol::Validators::PublishValidator()
				.ValidatePackage(copyPackage, *mqttClientState, m_SubscribeManager);

			auto pulishAckConverter = Protocol::Converters::PublishAckConverter();
			Protocol::PublishAckPackage publishAckPackage;
			publishAckPackage.Header.PackageType = Protocol::ControlPackageType::PublAck;
			publishAckPackage.PacketIdentifier = package.VariableHeader.PacketIdentifier;
			switch (action)
			{
			case MQTT::Protocol::Validators::PublishValidator::RejectPublish:	
				// TODO: Figure out how we handle publish reject. [3.3.5 Actions]
				//If a Server implementation does not authorize a PUBLISH to be performed by a Client; 
				//it has no way of informing that Client. It MUST either make a positive acknowledgement,
				//according to the normal QoS rules, or close the Network Connection		
				
				DisconnectClientState(client);
				break;
			case MQTT::Protocol::Validators::PublishValidator::AcknowledgePublish:
				m_Server->Send(client, pulishAckConverter.ToBuffer(publishAckPackage));
				break;
			case MQTT::Protocol::Validators::PublishValidator::DisconnectClient:
				DisconnectClientState(client);
				break;
			default:
				break;
			}
		}

		// If a client state with matching connection id exists, set is connected to false and remove will message.
		void MqttService::DisconnectClientState(const Client& client)
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
