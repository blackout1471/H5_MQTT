#pragma once
#include "Packages.h"
#include <cstdint>

namespace MQTT {
	namespace Protocol {
		struct PublishAcknowledgePackage {
			ControlHeader Header;
			int16_t PacketIdentifier;
		};
	}
}