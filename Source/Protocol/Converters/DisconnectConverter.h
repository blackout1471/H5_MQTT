#pragma once
#include "IPackageConverter.h"
#include "MqttPackages/Packages.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class DisconnectConverter final : public IPackageConverter<MqttPackages::DisconnectPackage>
			{
			public:
				// <inheritDoc />
				virtual const MqttPackages::DisconnectPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::DisconnectPackage& to) override;
			};
		}
	}
}

