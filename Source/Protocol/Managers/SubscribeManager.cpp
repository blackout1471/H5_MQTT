#include "mqttpch.h"
#include "SubscribeManager.h"
#include "Protocol/Validators/Rules/Rules.h"

namespace MQTT {
	namespace Protocol {

		void SubscribeManager::AddToSubscriptions(std::string clientID, SubscribePackage subscribePackage)
		{
			for (auto subscribeTopic : subscribePackage.Payload.Topics)
			{
				auto subClient = new Protocol::SubscribeClient(clientID, subscribeTopic.QoS);
				auto parentWildcard = subscribeTopic.HaveChild ? Protocol::NoWildcard : subscribeTopic.Wildcard;
				Protocol::BTree* matchingTree = nullptr;

				if (subscribeTopic.Paths.size() == 0)
					m_Subscriptions.push_back(Protocol::BTree::NewBTree(subClient, {}, subscribeTopic.Wildcard));
				else
					matchingTree = GetParentBTree(subscribeTopic.Paths[0], parentWildcard);

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

		void SubscribeManager::AddSubscribers(std::vector<SubscribeClient>& to, std::vector<SubscribeClient*>& from)
		{
			for (auto f : from)
				to.push_back(*f);
		}

		bool SubscribeManager::ValidPackage(const SubscribePackage& subscribePackage)
		{
			return !(Validators::RuleEngine({
				{ new Validators::SubscribeWildcardRule({ Dollar, Plus }, subscribePackage.Payload.Topics), false },
				{ new Validators::SubscribeTopicLength(subscribePackage.Payload.Topics), false }
				}).Run());
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
			ackPackage.VariableHeader.PacketIdentifier = subPackage.VariableHeader.PacketIdentifier;
									
			for (auto topic : subPackage.Payload.Topics)
			{
				if (topic.QoS < 0 || topic.QoS > 2)
					ackPackage.Payload.Payload.push_back(SubscribeAcknowledgementQoS::Failure);
				else
					ackPackage.Payload.Payload.push_back((SubscribeAcknowledgementQoS)topic.QoS);
			}

			return converter.ToBuffer(ackPackage);
		}

		std::vector<SubscribeClient> SubscribeManager::GetSubscribedClients(std::vector<unsigned char> topic)
		{
			std::vector<unsigned char> currentPath;
			std::vector <std::vector<unsigned char>> paths;

			for (int i = 0; i < topic.size(); i++)
			{
				if (topic[i] == '/')
				{
					paths.push_back(currentPath);
					currentPath.clear();
				}
				else
				{
					currentPath.push_back(topic[i]);
				}
			}

			if (currentPath.size() != 0)
				paths.push_back(currentPath);

			std::vector<SubscribeClient> subscribers;

			// The almighty seer of all
			auto theSpamLover = GetParentBTree({}, Hashtag);

			if (theSpamLover != nullptr)
				AddSubscribers(subscribers, theSpamLover->GetSubClients());

			auto parent = GetParentBTree(paths[0], Protocol::Hashtag);

			if (parent != nullptr)
				AddSubscribers(subscribers, parent->GetSubClients());

			parent = GetParentBTree(paths[0], Protocol::NoWildcard);

			if (parent == nullptr)
				return subscribers;

			for (int i = 1; i < paths.size(); i++)
			{
				if (parent->GetWildcard() == Protocol::Hashtag)
					AddSubscribers(subscribers, parent->GetSubClients());

				parent = parent->GetTopicMatch(paths[i]);
			}

			if (parent != nullptr)
				if (parent->GetTopic() == paths.at(paths.size() - 1))
					AddSubscribers(subscribers, parent->GetSubClients());

			return subscribers;
		}
	}
}