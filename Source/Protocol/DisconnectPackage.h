#pragma once
#include <Protocol/ControlHeader.h>
#include <Protocol/ConnectVariableHeader.h>
namespace MQTT {
	namespace Protocol {
		struct DisconnectPackage
		{
		public:
			ControlHeader ControlHeader;
		};

	}

};