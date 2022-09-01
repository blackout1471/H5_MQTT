#pragma once
#include "Protocol/BTree.h"
#include "Protocol/Converters/SubscribeConverter.h"
#include "Protocol/Converters/SubscribeAcknowledgementConverter.h"
#include "Protocol/Validators/RuleEngine.h"
#include "Protocol/Validators/Rules/IRule.h"

namespace MQTT {
	namespace Protocol {
		class SubscribeManager
		{
			std::vector<BTree*> m_Subscriptions;
			void AddSubscribers(std::vector<SubscribeClient>& to, std::vector<SubscribeClient*>& from);

		public:

			bool ValidPackage(const SubscribePackage& subscribePackage);

			// Searches only in the parents
			BTree* GetParentBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			void AddToSubscriptions(std::string clientID, SubscribePackage subscribePackage);
			std::vector<unsigned char> CreateSubAckBuffer(const Protocol::SubscribePackage& subPackage);
			std::vector<SubscribeClient> GetSubscribedClients(std::vector<unsigned char> topic);
			void AddParentTree(BTree* bTree);
			void DeleteTrees();
		};

	}
}


