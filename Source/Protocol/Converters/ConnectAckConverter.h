#pragma once
#include "IPackageConverter.h"
#include "MqttPackages/ConnectAckPackage.h"


namespace MQTT {
	namespace Protocol {
		namespace Converters
		{
			class ConnectAckConverter : IPackageConverter<MqttPackages::ConnectAckPackage>
			{
			public:
				// <inheritDoc />
				virtual const MqttPackages::ConnectAckPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::ConnectAckPackage& package) override;
			};
		}
	}
}