#pragma once
#include "MqttPackages/Packages.h"
#include "IPackageConverter.h"
#include "ConverterUtility.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class PublishAcknowledgeConverter : public IPackageConverter<MqttPackages::PublishAcknowledgePackage>
			{
			public:
				// <inheritDoc />
				virtual const MqttPackages::PublishAcknowledgePackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::PublishAcknowledgePackage& to) override;
			};
		}
	}
}


