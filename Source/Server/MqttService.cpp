#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Protocol/Converters/ConverterUtility.h"
#include "Protocol/Validators/ConnectValidator.h"

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
			auto& packageClientId = package.ConnectPayload.ClientId;
			auto& protocolName = package.ConnectVariableHeader.ProtocolName;
			auto* clientState = new MqttClient();
			
			clientState->ConnectionFlags = package.ConnectVariableHeader.VariableLevel;

			auto action = Protocol::Validators::ConnectValidator()
				.ValidateClient(package, m_ClientStates, clientState);

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

		MqttClient* MqttService::GetClientState(const std::string& clientId)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ClientId == clientId)
					return clientState;

			return nullptr;
		}

		std::string MqttService::GenerateUniqueId()
		{
			// TODO: Create unique id
			return "1";
		}

		void MqttService::InitialiseServer()
		{
			m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}