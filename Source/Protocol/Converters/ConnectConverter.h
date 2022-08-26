#pragma once

#include "IPackageConverter.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class ConnectConverter final : public IPackageConverter<ConnectPackage>
			{
			public:
				ConnectConverter();
				~ConnectConverter();

				// <inheritDoc />
				virtual const ConnectPackage ToPackage(const std::vector<unsigned char>& buffer) override;

				// <inheritDoc />
				virtual const std::vector<unsigned char> ToBuffer(const ConnectPackage& to) override;

			private:
				/*
				* Converts the unsigned char array to connect variable header package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int ConvertToVariableHeader(const unsigned char* dataPtr, ConnectVariableHeader& package);
				/*
				* Converts the unsigned char array to connect package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int ConvertToPayload(const unsigned char* dataPtr, const ConnectVariableHeader& headerPackage, ConnectPayload& package);
			};
		}
	}
}