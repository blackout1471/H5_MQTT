#pragma once
#include "Protocol/BTree.h"
#include "Converters/SubscribeConverter.h"
#include "Converters/SubscribeAcknowledgementConverter.h"
#include "Rules/RuleEngine.h"
#include "Rules/IRule.h"

namespace MQTT {
	namespace Protocol {
		class SubscribeManager
		{
		public:
			bool ValidPackage(const MqttPackages::SubscribePackage& subscribePackage);

			BTree* GetParentBTree(std::vector<unsigned char> topic);
			// Searches only in the parents
			BTree* GetParentBTree(std::vector<unsigned char> path, MqttPackages::SubscribeTopicWildcardType wildcard);
			// Adds client to subscriptions if the subscribe package is valid
			void AddToSubscriptions(std::string clientID, MqttPackages::SubscribePackage subscribePackage);
			std::vector<unsigned char> CreateSubAckBuffer(const MqttPackages::SubscribePackage& subPackage);

			// Finds all clients that are subscribed to the given topic, or are subscriber of a parent with wildcard
			std::vector<MqttPackages::SubscribeClient> GetSubscribedClients(std::vector<unsigned char> path);

			BTree* GetMatchingBTree(std::vector<unsigned char> path);

			void AddParentTree(BTree* bTree);
			void DeleteTrees();

		private:
			std::vector<BTree*> m_Subscriptions;

			// Splits topic into a list of topics
			std::vector<std::vector<unsigned char>> GetPathsFromPath(std::vector<unsigned char> path);

			// Copies a list of subscibers from one list to another, if client already exists in 'to' the QoS of that client is updated
			void AddSubscribers(std::vector<MqttPackages::SubscribeClient>& to, std::vector<MqttPackages::SubscribeClient*>& from);
		};

	}
}


