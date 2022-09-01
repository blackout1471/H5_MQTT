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
			void AddToSubscriptions(std::string clientID, SubscribePackage subscribePackage);
			std::vector<unsigned char> CreateSubAckBuffer(const Protocol::SubscribePackage& subPackage);
			void AddParentTree(BTree* bTree);
			void DeleteTrees();
		};

	}
}


