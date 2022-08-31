#pragma once
#include <vector>
#include "SubscribeTopicWildcard.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribeTopic {
			std::vector<std::vector<unsigned char>> Paths;
			SubscribeTopicWildcardType Wildcard = SubscribeTopicWildcardType::NoWildcard;
			unsigned char QoS;
			bool HaveChild = false; // TODO: change later to binary tree
		};
	}
}