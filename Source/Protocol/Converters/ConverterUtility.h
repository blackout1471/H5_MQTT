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

				static int GetPacketSize(const std::vector<unsigned char>& buffer, int offset) {
					int msbSize = buffer[offset];
					int totalSize = 0;
					int t = (msbSize >> 7);
					int tt = (msbSize >> 8);
					// pcakte is good
					if ((msbSize < 128))
						return ByteToInt(msbSize, buffer[offset + 1]);

					while ((msbSize > 128)) {
						totalSize += (msbSize << 7);
						offset++;
						msbSize = buffer[offset];
					}

					return totalSize;

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