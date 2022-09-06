#pragma once
#include <MqttPackages/ControlHeader.h>
#include <MqttPackages/ConnectVariableHeader.h>
namespace MQTT {
	namespace MqttPackages {
		struct DisconnectPackage
		{
		public:
			ControlHeader Header;
		};

	}

};