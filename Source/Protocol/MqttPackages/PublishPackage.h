#pragma once
#include "ControlHeader.h"
#include "PublishHeaderFlag.h"
#include "PublishVariableHeader.h"

namespace MQTT {
	namespace Protocol {
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