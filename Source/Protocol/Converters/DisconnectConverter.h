#pragma once
#include "IPackageConverter.h"
#include "Protocol/MqttPackages/Packages.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class DisconnectConverter final : public IPackageConverter<DisconnectPackage>
			{
			public:
				// Inherited via IPackageConverter
				virtual const DisconnectPackage ToPackage(const std::vector<unsigned char>& buffer) override;
				virtual const std::vector<unsigned char> ToBuffer(const DisconnectPackage& to) override;
			};
		}
	}
}

