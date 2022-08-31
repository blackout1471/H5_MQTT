#include "mqttpch.h"
#include "SubscribeManager.h"


namespace MQTT {
	namespace Protocol {

		BTree* MQTT::Protocol::SubscribeManager::GetBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard)
		{
			for (int i = 0; i < m_Subscriptions.size(); i++)
			{
				auto tree = m_Subscriptions[i]->GetFullMatch(topic, wildcard);

				if (tree != nullptr)
					return tree;
			}

			return nullptr;
		}

		BTree* SubscribeManager::GetBTreeByType(std::vector<unsigned char> topic)
		{
			for (int i = 0; i < m_Subscriptions.size(); i++)
			{
				auto tree = m_Subscriptions[i]->GetTopicMatch(topic);

				if (tree != nullptr)
					return tree;
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

	}
}