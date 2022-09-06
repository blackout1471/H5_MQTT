#pragma once
#include <vector>
#include "SubscribeTopicWildcard.h"
#include "SubscribeTopic.h"
namespace MQTT {
	namespace MqttPackages {
		struct SubscribePayload {
		public:
			std::vector<SubscribeTopic> Topics;
		};
	}
}