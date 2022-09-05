#pragma once
#include "Protocol/MqttPackages/Packages.h"
#include "IPackageConverter.h"
#include "ConverterUtility.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class PublishAcknowledgeConverter : public IPackageConverter<PublishAcknowledgePackage>
			{
			public:
				virtual const PublishAcknowledgePackage ToPackage(const std::vector<unsigned char>& buffer) override;
				virtual const std::vector<unsigned char> ToBuffer(const PublishAcknowledgePackage& to) override;
			};
		}
	}
}


