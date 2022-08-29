#pragma once
#include "IPackageConverter.h"
#include "Protocol/MqttPackages/Packages.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters
		{
			class SubscribeConverter : public IPackageConverter<SubscribePackage>
			{
			public:
				// Inherited via IPackageConverter
				virtual const SubscribePackage ToPackage(const std::vector<unsigned char>& buffer) override;
				virtual const std::vector<unsigned char> ToBuffer(const SubscribePackage& to) override;
			};
		}
	}
}

