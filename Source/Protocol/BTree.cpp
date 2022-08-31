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
				auto tree = m_Children[i]->GetFullMatch(topic, wildcard);

				if (tree != nullptr)
					return tree;
			}

			return nullptr;
		}
		BTree* BTree::GetTopicMatch(std::vector<unsigned char> topic)
		{
			if (topic == m_Topic)
				return this;

			for (int i = 0; i < m_Children.size(); i++)
			{
				auto tree = m_Children[i]->GetTopicMatch(topic);

				if (tree != nullptr)
					return tree;
			}

			return nullptr;
		}
	}
}
