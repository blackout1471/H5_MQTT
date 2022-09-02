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
		public:
			bool ValidPackage(const SubscribePackage& subscribePackage);
			// Searches only in the parents
			BTree* GetParentBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			// Adds client to subscriptions if the subscribe package is valid
			void AddToSubscriptions(std::string clientID, SubscribePackage subscribePackage);
			std::vector<unsigned char> CreateSubAckBuffer(const Protocol::SubscribePackage& subPackage);

			// Finds all clients that are subscribed to the given topic, or are subscriber of a parent with wildcard
			std::vector<SubscribeClient> GetSubscribedClients(std::vector<unsigned char> topic);
			void AddParentTree(BTree* bTree);
			void DeleteTrees();

		private:
			std::vector<BTree*> m_Subscriptions;

			// Copies a list of subscibers from one list to another, if client already exists in 'to' the QoS of that client is updated
			void AddSubscribers(std::vector<SubscribeClient>& to, std::vector<SubscribeClient*>& from);
		};

	}
}


