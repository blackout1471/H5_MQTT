#pragma once
#include <cstdint>
#include <vector>
#include "ConnectFlagType.h"

namespace MQTT
{
	namespace Models
	{
		struct ConnectVariableHeader
		{
		public:
			int16_t NameLength;
			std::vector<unsigned char> ProtocolName;
			unsigned char Level;
			ConnectFlagType VariableLevel;
			int16_t KeepAlive;
		};
	}
}