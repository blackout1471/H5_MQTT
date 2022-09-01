#include "mqttpch.h"
#include "BTree.h"


namespace MQTT {
	namespace Protocol {

		BTree* BTree::GetFullMatch(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard)
		{
			if (topic == m_Topic && m_Wildcard == wildcard)
				return this;

			for (int i = 0; i < m_Children.size(); i++)
			{
				if(m_Children[i]->GetTopic() == topic && m_Children[i]->GetWildcard() == wildcard)
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
