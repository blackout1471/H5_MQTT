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
				// <inheritDoc />
				virtual const SubscribePackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const SubscribePackage& to) override;

			private:
				/*
				* Converts the given buffer, package size and offset to a new subscribe payload package.
				*/
				const SubscribePayload ConvertPayload(const std::vector<unsigned char>& buffer, int packageSize, int offset);
			};
		}
	}
}

