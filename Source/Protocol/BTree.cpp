#include "mqttpch.h"
#include "BTree.h"


namespace MQTT {
	namespace Protocol {
		BTree::~BTree()
		{
			for (int i = 0; i < m_Children.size(); i++)
				delete m_Children[i];

			for (auto& sub : m_SubClients)
				delete sub;
		}
		void BTree::AddClient(SubscribeClient* subClient)
		{
			bool foundSubClient = false;

			for (int i = 0; i < m_SubClients.size(); i++)
			{
				if (m_SubClients[i]->ClientID == subClient->ClientID) {
					m_SubClients[i]->QoS = subClient->QoS;
					foundSubClient = true;
					break;
				}
			}

			if (!foundSubClient)
				m_SubClients.push_back(subClient);
		}
		BTree* BTree::GetFullMatch(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard)
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
