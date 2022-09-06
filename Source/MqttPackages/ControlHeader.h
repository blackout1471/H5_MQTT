#pragma once
#include "ControlPackageType.h"

namespace MQTT
{
	namespace MqttPackages
	{
		struct ControlHeader {
			ControlPackageType PackageType;
		};
	}
}