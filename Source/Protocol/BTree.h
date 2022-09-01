#pragma once
#include <vector>
#include <string>
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		class BTree
		{
			std::vector<BTree*> m_Children;
			BTree* m_Parent = nullptr;
			std::vector<unsigned char> m_Topic;
			std::vector<SubscribeClient*> m_SubClients;
			std::vector<SubscribeSavedTopic> m_SubSavedTopics;
			SubscribeTopicWildcardType m_Wildcard = SubscribeTopicWildcardType::NoWildcard;

		public:

			~BTree();

			void AddClient(SubscribeClient* subClient);
			BTree* GetFullMatch(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			BTree* GetTopicMatch(std::vector<unsigned char> topic);

			std::vector<unsigned char> GetTopic() {
				return m_Topic;
			}

			SubscribeTopicWildcardType GetWildcard() {
				return m_Wildcard;
			}
			std::vector<SubscribeClient*>& GetSubClients() {
				return m_SubClients;
			}


			static BTree* NewBTree(SubscribeClient* subClient, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard) {
				BTree* btree = new BTree();
				btree->AddClient(subClient);
				btree->m_Topic = topic;
				btree->m_Wildcard = wildcard;
				return btree;
			}


			static BTree* NewBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard) {
				BTree* btree = new BTree();
				btree->m_Topic = topic;
				btree->m_Wildcard = wildcard;
				return btree;
			}


			static BTree* NewBTree(BTree* parent, SubscribeClient* subClient, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard) {
				BTree* btree = new BTree();
				btree->m_Parent = parent;
				btree->AddClient(subClient);
				btree->m_Topic = topic;
				btree->m_Wildcard = wildcard;

				parent->m_Children.push_back(btree);

				return btree;
			}

			static BTree* NewBTree(BTree* parent, std::vector<unsigned char> topic) {
				BTree* btree = new BTree();
				btree->m_Parent = parent;
				btree->m_Topic = topic;

				parent->m_Children.push_back(btree);

				return btree;
			}
		};
	}
}
