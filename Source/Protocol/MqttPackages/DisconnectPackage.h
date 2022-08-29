#pragma once
#include <Protocol/MqttPackages/ControlHeader.h>
#include <Protocol/MqttPackages/ConnectVariableHeader.h>
namespace MQTT {
	namespace Protocol {
		struct DisconnectPackage
		{
		public:
			ControlHeader ControlHeader;
		};

	}

};