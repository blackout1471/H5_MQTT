#pragma once
#include "Protocol/MqttPackages/Packages.h"
#include "IPackageConverter.h"
#include "ConverterUtility.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class PublishAckConverter : public IPackageConverter<PublishAckPackage>
			{
				// Inherited via IPackageConverter
				virtual const PublishAckPackage ToPackage(const std::vector<unsigned char>& buffer) override;
				virtual const std::vector<unsigned char> ToBuffer(const PublishAckPackage& to) override;
			};
		}
	}
}


