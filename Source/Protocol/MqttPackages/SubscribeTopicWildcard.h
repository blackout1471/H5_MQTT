#pragma once
namespace MQTT {
	namespace Protocol {
		enum SubscribeTopicWildcardType : unsigned char {
			Hashtag,
			Dollar,
			Plus
		};
	}
}