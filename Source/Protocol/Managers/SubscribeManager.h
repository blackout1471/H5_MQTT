#pragma once
#include "Protocol/BTree.h"
namespace MQTT {
	namespace Protocol {
		class SubscribeManager
		{
			std::vector<BTree*> m_Subscriptions;
		public:
			// Searches only in the parents
			BTree* GetParentBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			void AddParentTree(BTree* bTree);
			void DeleteTrees();
		};

	}
}


