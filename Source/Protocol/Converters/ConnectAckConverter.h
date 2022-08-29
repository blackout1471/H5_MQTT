#pragma once
#include "IPackageConverter.h"
#include "Protocol/MqttPackages/ConnectAckPackage.h"


namespace MQTT {
	namespace Protocol {
		namespace Converters
		{
			class ConnectAckConverter : IPackageConverter<ConnectAckPackage>
			{
			public:
				virtual const ConnectAckPackage ToPackage(const std::vector<unsigned char>& buffer) override;
				virtual const std::vector<unsigned char> ToBuffer(const ConnectAckPackage& package) override;
			};
		}
	}
}