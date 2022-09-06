#pragma once

#include "IPackageConverter.h"
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class ConnectConverter final : public IPackageConverter<MqttPackages::ConnectPackage>
			{
			public:
				ConnectConverter();
				~ConnectConverter();

				/// <inheritdoc/>
				virtual const MqttPackages::ConnectPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				/// <inheritdoc />
				virtual const std::vector<unsigned char> ToBuffer(const MqttPackages::ConnectPackage& to) override;

			private:
				/*
				* Converts the unsigned char array to connect variable header package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int ConvertToVariableHeader(const unsigned char* dataPtr, MqttPackages::ConnectVariableHeader& package);
				/*
				* Converts the unsigned char array to connect package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int ConvertToPayload(const unsigned char* dataPtr, const MqttPackages::ConnectVariableHeader& headerPackage, MqttPackages::ConnectPayload& package);
			};
		}
	}
}