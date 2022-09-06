#pragma once
#include <MqttPackages/SubscribeAcknowledgementPackage.h>
#include "IPackageConverter.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class SubscribeAcknowledgementConverter final : public IPackageConverter<MqttPackages::SubscribeAcknowledgementPackage>
			{
			public:
				// <inheritDoc />
				virtual const MqttPackages::SubscribeAcknowledgementPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::SubscribeAcknowledgementPackage& to) override;
			};
		}
	}
}

