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

			void AddClient(MqttPackages::SubscribeClient* subClient);
			BTree* GetFullMatch(std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard);
			BTree* GetTopicMatch(std::vector<unsigned char> topic);

			const std::vector<unsigned char>& GetTopic() {
				return m_Topic;
			}

			const MqttPackages::SubscribeTopicWildcardType& GetWildcard() {
				return m_Wildcard;
			}
			std::vector<MqttPackages::SubscribeClient*>& GetSubClients() {
				return m_SubClients;
			}
			void AddSavedMessage(MqttPackages::SubscribeSavedMessage saveMessage) {
				m_SubSavedMessages.push_back(saveMessage);
			}
			std::vector<MqttPackages::SubscribeSavedMessage>& GetSavedMessages() {
				return m_SubSavedMessages;
			}

			// Creates a new tree
			static BTree* NewBTree(MqttPackages::SubscribeClient* subClient, std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(BTree* parent, MqttPackages::SubscribeClient* subClient, std::vector<unsigned char> topic, MqttPackages::SubscribeTopicWildcardType wildcard);

			// Creates a new tree
			static BTree* NewBTree(BTree* parent, std::vector<unsigned char> topic);

		private:
			std::vector<BTree*> m_Children;
			BTree* m_Parent = nullptr;
			std::vector<unsigned char> m_Topic;
			std::vector<MqttPackages::SubscribeClient*> m_SubClients;
			std::vector<MqttPackages::SubscribeSavedMessage> m_SubSavedMessages;
			MqttPackages::SubscribeTopicWildcardType m_Wildcard = MqttPackages::SubscribeTopicWildcardType::NoWildcard;
		};
	}
}
