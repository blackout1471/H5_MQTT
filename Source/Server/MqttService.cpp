#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Protocol/Converters/ConverterUtility.h"

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

			m_SubscribeManager.DeleteTrees();

			delete m_Server;
		}

		void MqttService::Start()
		{
			m_Server->Start();
		}

		void MqttService::OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer)
		{
			auto type = Protocol::Converters::ConverterUtility::GetPackageType(buffer[0]);

			// TODO :: do validation
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

			auto shouldDisconnectClient = !(RuleEngine({
				{new ClientConnectedRule(packageClientId, m_ClientStates), false},
				{new CorrectProtocolNameRule(protocolName), true},
				{new Protocol311Rule(package.ConnectVariableHeader.Level), true},
				{new ConnectReservedFlagSetRule(package.ConnectVariableHeader.VariableLevel), false},
				{new IsCredentialFlagIncorrectRule(package.ConnectVariableHeader.VariableLevel), false},
				{new ConnectWillRule(clientState, package.ConnectVariableHeader.VariableLevel), true}
				}).Run());

			if (shouldDisconnectClient)
			{
				m_Server->Disconnect(client);
				return;
			}

			auto shouldContinueSession = (RuleEngine({
				{new ContinueSessionRule(package.ConnectVariableHeader.VariableLevel, packageClientId, m_ClientStates), true}
				}).Run());

			// Todo :: Move 
			if (shouldContinueSession)
			{
				delete clientState;

				clientState = GetClientState(packageClientId);
				if (!clientState)
				{
					m_Server->Disconnect(client);
					return;
				}

				if (packageClientId.size() == 0)
				{
					auto ackMessage = m_Manager.GenerateConnectAckMessage(Protocol::Accepted);
					m_Server->Send(client, ackMessage);
					m_Server->Disconnect(client);
				}
				else
				{
					auto ackMessage = m_Manager.GenerateConnectAckMessage(Protocol::Accepted);
					m_Server->Send(client, ackMessage);
				}

				return;
			}


			if (packageClientId == "")
				clientState->ClientId = GenerateUniqueId();
			else
				clientState->ClientId = packageClientId;


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

		std::string MqttService::GenerateUniqueId()
		{
			// TODO: Create unique id
			return "1";
		}

		void MqttService::OnClientSubscribed(const Client& client, const Protocol::SubscribePackage& package)
		{
			for (int i = 0; i < package.SubscribePayload.Topics.size(); i++)
			{
				auto topic = package.SubscribePayload.Topics.at(i);

				Protocol::BTree* matchingTree = nullptr;

				if (!topic.HaveChild)
				{
					matchingTree = m_SubscribeManager.GetBTree(topic.Topics[0], topic.Wildcard);

					if (matchingTree == nullptr) {
						m_SubscribeManager.AddParentTree(Protocol::BTree::NewBTree(topic.Topics[0], topic.Wildcard));
					}
					else {
						matchingTree->AddClient(client.GetIdentifier());
					}
				}
				else
				{
					Protocol::BTree* parentTree = nullptr;


					for (int j = 0; j < topic.Topics.size(); j++)
					{
						//TODO: Remove after testing
						if (topic.Topics.at(j).size() == 7)
						{
							printf("Yt hit");
						}

						matchingTree = m_SubscribeManager.GetBTree(topic.Topics.at(j), topic.Wildcard);

						if (matchingTree == nullptr) {

							if (parentTree == nullptr) {

								if ((j + 1) >= topic.Topics.size())
								{
									// its the last topic
									parentTree = Protocol::BTree::NewBTree(client.GetIdentifier(), topic.Topics.at(j), topic.Wildcard);
									m_SubscribeManager.AddParentTree(parentTree);
								}
								else
								{
									parentTree = Protocol::BTree::NewBTree(topic.Topics.at(j), Protocol::NoWildcard);

									m_SubscribeManager.AddParentTree(parentTree);
								}
							}
							else
							{
								if ((j + 1) >= topic.Topics.size())
								{
									// its the last topic
									parentTree = Protocol::BTree::NewBTree(parentTree, client.GetIdentifier(), topic.Topics.at(j), topic.Wildcard);
								}
								else
								{
									parentTree = Protocol::BTree::NewBTree(parentTree, topic.Topics.at(j));
								}
							}
						}
						else {

							parentTree = matchingTree;
						}

					}



					/*if (matchingTree != nullptr)
					{
						matchingTree->AddClient(client.GetIdentifier());
					}
					else
					{
						auto newTree = Protocol::BTree::NewBTree(client.GetIdentifier(), topic.Topic, topic.Wildcard);
						m_SubscribeManager.AddParentTree(newTree);
					}*/

				}
				/*Protocol::BTree::NewBTree(client.GetIdentifier(), )*/

				// TODO: Add rules

			}
			printf("a");
			/*	auto ackMessage = m_Manager.GenerateConnectAckMessage(Protocol::Accepted);

				m_Server->Send(client, ackMessage);*/
		}

		void MqttService::InitialiseServer()
		{
			m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}
