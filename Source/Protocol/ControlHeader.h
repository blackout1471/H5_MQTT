#pragma once
#include "ControlPackageType.h"

namespace MQTT
{
	namespace Protocol
	{
		struct ControlHeader {
			ControlPackageType m_PackageType;
		};
	}
}