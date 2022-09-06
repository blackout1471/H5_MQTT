#pragma once
#include "IPackageConverter.h"
#include "Protocol/MqttPackages/Packages.h"


namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class PublishConverter : IPackageConverter<PublishPackage>
			{
			public:
				PublishConverter();
				~PublishConverter();

				// <inheritDoc />
				virtual const PublishPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const PublishPackage& to) override;
			};
		}
	}
}

