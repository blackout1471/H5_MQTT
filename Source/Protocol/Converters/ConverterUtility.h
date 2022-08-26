#pragma once
#include "Protocol/MqttPackages/Packages.h"

#include <cstdint>


namespace MQTT {
	namespace Protocol {
		namespace Converters {
			class ConverterUtility final {
			private:
				ConverterUtility() {};
				virtual ~ConverterUtility() {};

			public:
				/*
				* Converts byte1 as MSB and byte2 as LSB
				* Returns: The result of the conversion.
				*/
				static int16_t ByteToInt(unsigned char byte1, unsigned char byte2) 
				{
					return (byte1 << 8) | byte2;
				}

				/*
				* Retrieves the package type from the given unsigned char
				*/
				static ControlPackageType GetPackageType(const unsigned char data) 
				{
					return ControlPackageType((data >> 4) & 0xff);
				}
			};
		}
	}
}