#pragma once
namespace MQTT {
	namespace Protocol {
		enum SubscribeTopicWildcardType : unsigned char {
			NoWildcard = 0,
			Hashtag = 35,
			Dollar,
			Plus
		};
	}
}