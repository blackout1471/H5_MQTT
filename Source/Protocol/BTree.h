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
			std::vector<std::string> m_ClientsIDs;
			SubscribeTopicWildcardType m_Wildcard = SubscribeTopicWildcardType::NoWildcard;

		public:

			~BTree() {
				for (int i = 0; i < m_Children.size(); i++)
				{
					delete m_Children[i];
				}
			}

			void AddClient(std::string clientID) {
				m_ClientsIDs.push_back(clientID);
			}

			SubscribeTopicWildcardType GetWildcard() {
				return m_Wildcard;
			}

			BTree* GetFullMatch(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			BTree* GetTopicMatch(std::vector<unsigned char> topic);


			static BTree* NewBTree(std::string clientID, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard) {
				BTree* btree = new BTree();
				btree->m_ClientsIDs.push_back(clientID);
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


			static BTree* NewBTree(BTree* parent, std::string clientID, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard) {
				BTree* btree = new BTree();
				btree->m_Parent = parent;
				btree->m_ClientsIDs.push_back(clientID);
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
