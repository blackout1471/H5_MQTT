#pragma once

namespace MQTT {
	namespace Protocol {
		enum PublishHeaderFlag : unsigned char
		{
			Retain	= 1,
			QoSMsb	= 2,
			QoSLsb	= 4,
			Dup		= 8
		};

		inline static PublishHeaderFlag operator|(PublishHeaderFlag a, PublishHeaderFlag b)
		{
			return static_cast<PublishHeaderFlag>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline static PublishHeaderFlag operator~(PublishHeaderFlag a)
		{
			return static_cast<PublishHeaderFlag>(~static_cast<int>(a));
		}

		inline static PublishHeaderFlag& operator|=(PublishHeaderFlag& a, PublishHeaderFlag b)
		{
			return a = a | b;
		}

		inline static PublishHeaderFlag operator&(PublishHeaderFlag a, PublishHeaderFlag b)
		{
			return static_cast<PublishHeaderFlag>(static_cast<int>(a) & static_cast<int>(b));
		}
	}
}