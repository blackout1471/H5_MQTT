#pragma once
#include "ConnectPayload.h"
#include "ConnectVariableHeader.h"
#include "ControlHeader.h"

namespace MQTT {
	namespace Protocol {
		struct ConnectPackage
		{
		public:
			ControlHeader ControlHeader;
			ConnectVariableHeader ConnectVariableHeader;
			ConnectPayload ConnectPayload;
		};

	}

};