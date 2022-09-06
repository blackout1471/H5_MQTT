#include "mqttpch.h"
#include "BTree.h"


namespace MQTT {
	namespace Protocol {
		BTree::~BTree()
		{
			for (auto* child : m_Children)
				delete child;

			for (auto* subClient : m_SubClients)
				delete subClient;
		}

		// Creates a new tree
		BTree* BTree::NewBTree(MqttPackages::SubscribeClient* subClient, std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard) {
			BTree* btree = new BTree();
			btree->AddClient(subClient);
			btree->m_Topic = topic;
			btree->m_Wildcard = wildcard;
			return btree;
		}

		// Creates a new tree
		BTree* BTree::NewBTree(std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard) {
			BTree* btree = new BTree();
			btree->m_Topic = topic;
			btree->m_Wildcard = wildcard;
			return btree;
		}

		// Creates a new tree
		BTree* BTree::NewBTree(BTree* parent, MqttPackages::SubscribeClient* subClient, std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard) {
			BTree* btree = new BTree();
			btree->m_Parent = parent;
			btree->AddClient(subClient);
			btree->m_Topic = topic;
			btree->m_Wildcard = wildcard;

			parent->m_Children.push_back(btree);

			return btree;
		}

		// Creates a new tree
		BTree* BTree::NewBTree(BTree* parent, std::vector<unsigned char> topic) {
			BTree* btree = new BTree();
			btree->m_Parent = parent;
			btree->m_Topic = topic;

			parent->m_Children.push_back(btree);

			return btree;
		}

		void BTree::AddClient(MqttPackages::SubscribeClient* subClient)
		{
			bool foundSubClient = false;

			for (int i = 0; i < m_SubClients.size(); i++)
			{
				if (m_SubClients[i]->GetClientID() == subClient->GetClientID()) {
					m_SubClients[i]->SetQoS(subClient->GetQoS());
					foundSubClient = true;
					break;
				}
			}

			if (!foundSubClient)
				m_SubClients.push_back(subClient);
		}
		BTree* BTree::GetFullMatch(std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard)
		{
			if (topic == m_Topic && m_Wildcard == wildcard)
				return this;

			for (int i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i]->GetTopic() == topic && m_Children[i]->GetWildcard() == wildcard)
					return m_Children[i];
			}

			return nullptr;
		}
		BTree* BTree::GetTopicMatch(std::vector<unsigned char> topic)
		{
			if (topic == m_Topic)
				return this;

			for (int i = 0; i < m_Children.size(); i++)
			{
				if (m_Children[i]->GetTopic() == topic)
					return m_Children[i];
			}

			return nullptr;
		}
	}
}
