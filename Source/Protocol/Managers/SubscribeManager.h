#pragma once
#include "Protocol/BTree.h"
namespace MQTT {
	namespace Protocol {
		class SubscribeManager
		{
			std::vector<BTree*> m_Subscriptions;
		public:
			BTree* GetBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			BTree* GetBTreeByType(std::vector<unsigned char> topic);
			void AddParentTree(BTree* bTree);
			void DeleteTrees();
		};

	}
}


