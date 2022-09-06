#pragma once
#include "IPackageConverter.h"
#include "MqttPackages/Packages.h"


namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class PublishConverter : IPackageConverter<MqttPackages::PublishPackage>
			{
			public:
				PublishConverter();
				~PublishConverter();

				// <inheritDoc />
				virtual const MqttPackages::PublishPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::PublishPackage& to) override;
			};
		}
	}
}

