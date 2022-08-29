#pragma once
#include "ConnectFlagType.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Protocol {
		struct ConnectVariableHeader
		{
			std::string ProtocolName;

			unsigned char Level;
			ConnectFlagType VariableLevel;

			int16_t KeepAlive;
		};
	}
}