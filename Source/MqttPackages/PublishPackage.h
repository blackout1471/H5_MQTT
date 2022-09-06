#pragma once
#include "ControlHeader.h"
#include "PublishHeaderFlag.h"
#include "PublishVariableHeader.h"

namespace MQTT {
	namespace MqttPackages {
		struct PublishPackage
		{
		public:
			ControlHeader Header;
			PublishHeaderFlag HeaderFlag;
			PublishVariableHeader VariableHeader;
			std::string Payload;
		};
	}
}