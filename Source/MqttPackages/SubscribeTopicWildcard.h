#pragma once
namespace MQTT {
	namespace MqttPackages {
		enum SubscribeTopicWildcardType : unsigned char {
			NoWildcard = 0,
			Hashtag = 35,
			Dollar = 36,
			Plus = 43
		};
	}
}