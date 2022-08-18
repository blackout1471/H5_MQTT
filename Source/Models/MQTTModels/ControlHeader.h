#pragma once
#include "ControlPackageType.h"

namespace MQTT
{
	namespace Models
	{
		struct ControlHeader {
		public:
			ControlPackageType m_PackageType;
		};
	}
}