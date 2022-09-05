#pragma once
#include <Protocol/MqttPackages/SubscribeAcknowledgementPackage.h>
#include "IPackageConverter.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class SubscribeAcknowledgementConverter final : public IPackageConverter<SubscribeAcknowledgementPackage>
			{
			public:
				// <inheritDoc />
				virtual const SubscribeAcknowledgementPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const SubscribeAcknowledgementPackage& to) override;
			};
		}
	}
}

