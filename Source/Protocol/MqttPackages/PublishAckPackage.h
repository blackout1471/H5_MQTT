#pragma once
#include "Packages.h"
#include <cstdint>

namespace MQTT {
	namespace Protocol {
		struct PublishAckPackage {
			ControlHeader Header;
			int16_t PacketIdentifier;
		};
	}
}