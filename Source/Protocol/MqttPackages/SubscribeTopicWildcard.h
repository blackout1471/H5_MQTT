#pragma once
namespace MQTT {
	namespace Protocol {
		enum SubscribeTopicWildcardType : unsigned char {
			Hashtag = 38,
			Dollar,
			Plus
		};
	}
}