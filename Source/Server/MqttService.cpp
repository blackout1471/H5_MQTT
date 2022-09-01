#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "ClientUtility.h"
#include "Protocol/Converters/ConverterUtility.h"
#include "Protocol/Converters/ConnectConverter.h"
#include "Protocol/Converters/DisconnectConverter.h"

// Todo :: Remove after testing
#include "Protocol/Validators/RuleEngine.h"
#include "Protocol/Validators/Rules/Rules.h"
using namespace MQTT::Protocol::Validators;

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

			delete m_Server;
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
				OnClientDisconnect(
					client, 
					Protocol::Converters::DisconnectConverter().ToPackage(buffer)
				);
				break;
			default:
				for (int i = 0; i < buffer.size(); i++)
				{
					printf("%02X ", buffer[i]);
				}
			}
		}

		void MqttService::DisconnectClientState(const Client& client)
		{
			for (auto& arr_client : m_ClientStates)
			{
				if (arr_client->ConnectionIdentifier == client.GetIdentifier())
					arr_client->IsConnected = false;
			}
			m_Server->Disconnect(client);
		}


		void MqttService::OnClientConnect(const Client& client, const Protocol::ConnectPackage& package)
		{
			auto& packageClientId = package.Payload.ClientId;
			auto& protocolName = package.VariableHeader.ProtocolName;
			auto* clientState = new MqttClient();
			
			clientState->ConnectionFlags = package.VariableHeader.VariableLevel;

			auto shouldDisconnectClient = !(RuleEngine({
				{new ClientConnectedRule(packageClientId, m_ClientStates), false},
				{new CorrectProtocolNameRule(protocolName), true},
				{new Protocol311Rule(package.VariableHeader.Level), true},
				{new ConnectReservedFlagSetRule(package.VariableHeader.VariableLevel), false},
				{new IsCredentialFlagIncorrectRule(package.VariableHeader.VariableLevel), false},
				{new ConnectWillRule(clientState, package.VariableHeader.VariableLevel, package.Payload.WillMessage), true}
			}).Run());

			if (shouldDisconnectClient)
			{
				m_Server->Disconnect(client);
				return;
			}

			std::vector<unsigned char> message;
			auto shouldContinueSession = (RuleEngine({
				{new ContinueSessionRule(package.VariableHeader.VariableLevel, packageClientId, m_ClientStates), true}
			}).Run());

			if (shouldContinueSession)
			{
				delete clientState;
				clientState = GetClientState(packageClientId);

				auto canContinueSession = (RuleEngine({ 
					{new GenerateSessionMessageRule(packageClientId, clientState, message), true }
				}).Run());

				m_Server->Send(client, message);

				// TODO :: Use member function to disconnect
				if (!canContinueSession)
					m_Server->Disconnect(client);

				return;
			}

			if (packageClientId == "")
				clientState->ClientId = ClientUtility::GenerateUniqueId();
			else
				clientState->ClientId = packageClientId;

			clientState->ConnectionIdentifier = client.GetIdentifier();

			m_ClientStates.push_back(clientState);
			
			clientState->IsConnected = true;

			auto ackMessage = m_Manager.GenerateConnectAckMessage(Protocol::Accepted);
			m_Server->Send(client, ackMessage);
		}

		MqttClient* MqttService::GetClientState(const std::string& clientId)
		{
			for (auto* clientState : m_ClientStates)
				if (clientState->ClientId == clientId)
					return clientState;

			return nullptr;
		}


		void MqttService::OnClientDisconnect(const Client& client, const Protocol::DisconnectPackage& package)
		{
			//TODO: Remove will message when storage of it is implemented.
			DisconnectClientState(client);
		}
		void MqttService::InitialiseServer()
		{
			m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}
