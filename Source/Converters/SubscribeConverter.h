#pragma once
#include "IPackageConverter.h"
#include "MqttPackages/Packages.h"
namespace MQTT {
	namespace Converters
	{
		class SubscribeConverter : public IPackageConverter<MqttPackages::SubscribePackage>
		{
		public:
			// <inheritDoc />
			virtual const MqttPackages::SubscribePackage ToPackage(const std::vector<unsigned char>& buffer) override;

			// <inheritDoc />
			virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::SubscribePackage& to) override;

		private:
			/*
			* Converts the given buffer, package size and offset to a new subscribe payload package.
			*/
			const MqttPackages::SubscribePayload ConvertPayload(const std::vector<unsigned char>& buffer, int packageSize, int offset);
		};
	}
}

