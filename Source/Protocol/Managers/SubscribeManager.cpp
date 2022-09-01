#include "mqttpch.h"
#include "SubscribeManager.h"
#include "Protocol/Converters/SubscribeAcknowledgementConverter.h"

namespace MQTT {
	namespace Protocol {

		/*BTree* MQTT::Protocol::SubscribeManager::GetBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard)
		{
			for (int i = 0; i < m_Subscriptions.size(); i++)
			{
				auto tree = m_Subscriptions[i]->GetFullMatch(topic, wildcard);

				if (tree != nullptr)
					return tree;
			}

			return nullptr;
		}*/

		void SubscribeManager::AddToSubscriptions(std::string clientID, SubscribePackage subscribePackage)
		{
			for (auto subscribeTopic : subscribePackage.SubscribePayload.Topics)
			{
				auto subClient = new Protocol::SubscribeClient(clientID, subscribeTopic.QoS);
				auto parentWildcard = subscribeTopic.HaveChild ? Protocol::NoWildcard : subscribeTopic.Wildcard;

				Protocol::BTree* matchingTree = GetParentBTree(subscribeTopic.Paths[0], parentWildcard);
				Protocol::BTree* parentTree = nullptr;

				for (int i = 0; i < subscribeTopic.Paths.size(); i++)
				{
					// Check if there is any full match of the current topic
					if (parentTree != nullptr)
						matchingTree = parentTree->GetFullMatch(subscribeTopic.Paths.at(i), subscribeTopic.Wildcard);

					if (matchingTree != nullptr) {
						parentTree = matchingTree;
						continue;
					}

					if (parentTree == nullptr)
					{
						// its the last topic, set the client 
						if ((i + 1) >= subscribeTopic.Paths.size())
							parentTree = Protocol::BTree::NewBTree(subClient, subscribeTopic.Paths.at(i), subscribeTopic.Wildcard);

						else
							parentTree = Protocol::BTree::NewBTree(subscribeTopic.Paths.at(i), Protocol::NoWildcard);

						AddParentTree(parentTree);
					}
					else
					{
						// its the last topic
						if ((i + 1) >= subscribeTopic.Paths.size())
							parentTree = Protocol::BTree::NewBTree(parentTree, subClient, subscribeTopic.Paths.at(i), subscribeTopic.Wildcard);
						else
							parentTree = Protocol::BTree::NewBTree(parentTree, subscribeTopic.Paths.at(i));
					}
				}
			}
		}

	




		BTree* SubscribeManager::GetParentBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard)
		{
			for (int i = 0; i < m_Subscriptions.size(); i++)
			{
				if (m_Subscriptions[i]->GetTopic() == topic && m_Subscriptions[i]->GetWildcard() == wildcard) {
					return m_Subscriptions[i];
				}
			}

			return nullptr;
		}


		void MQTT::Protocol::SubscribeManager::AddParentTree(BTree* bTree)
		{
			m_Subscriptions.push_back(bTree);
		}

		void MQTT::Protocol::SubscribeManager::DeleteTrees()
		{
			for (int i = 0; i < m_Subscriptions.size(); i++)
			{
				delete m_Subscriptions[i];
			}
		}

		std::vector<unsigned char> SubscribeManager::CreateSubAckBuffer(const Protocol::SubscribePackage& subPackage)
		{
			Protocol::Converters::SubscribeAcknowledgementConverter converter;

			Protocol::SubscribeAcknowledgementPackage ackPackage = Protocol::SubscribeAcknowledgementPackage();
			ackPackage.Header.PackageType = Protocol::ControlPackageType::SubAck;
			ackPackage.VariableHeader.PacketIdentifier = subPackage.SubscribeVariableHeader.PacketIdentifier;


			for (int i = 0; i < subPackage.SubscribePayload.Topics.size(); i++)
			{
				auto q = (SubscribeAcknowledgementQoS)subPackage.SubscribePayload.Topics.at(i).QoS;
				ackPackage.Payload.payload.push_back(q);
			}

			return converter.ToBuffer(ackPackage);
		}


	}
}