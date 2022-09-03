#pragma once
#include <vector>
#include <string>
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		class BTree
		{
		public:
			~BTree();

			void AddClient(SubscribeClient* subClient);
			BTree* GetFullMatch(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);
			BTree* GetTopicMatch(std::vector<unsigned char> topic);

			const std::vector<unsigned char>& GetTopic() {
				return m_Topic;
			}

			const SubscribeTopicWildcardType& GetWildcard() {
				return m_Wildcard;
			}
			std::vector<SubscribeClient*>& GetSubClients() {
				return m_SubClients;
			}
			void AddSavedMessage(SubscribeSavedMessage saveMessage) {
				m_SubSavedMessages.push_back(saveMessage);
			}
			std::vector<SubscribeSavedMessage>& GetSavedMessages() {
				return m_SubSavedMessages;
			}

			// Creates a new tree
			static BTree* NewBTree(SubscribeClient* subClient, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(BTree* parent, SubscribeClient* subClient, std::vector<unsigned char> topic, SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(BTree* parent, std::vector<unsigned char> topic);

		private:
			std::vector<BTree*> m_Children;
			BTree* m_Parent = nullptr;
			std::vector<unsigned char> m_Topic;
			std::vector<SubscribeClient*> m_SubClients;
			std::vector<SubscribeSavedMessage> m_SubSavedMessages;
			SubscribeTopicWildcardType m_Wildcard = SubscribeTopicWildcardType::NoWildcard;
		};
	}
}
