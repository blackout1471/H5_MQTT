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
				* Converts a string into a vector of unsigned chars
				* Returns the converted string as a vector of unsigned chars.
				*/
				static std::vector<unsigned char> StringToUCharVector(std::string str)
				{
					return std::vector<unsigned char>(str.begin(), str.end());
				}
				/*
				* Converts byte1 as MSB and byte2 as LSB
				* Returns: The result of the conversion.
				*/
				static unsigned short ByteToShort(unsigned char byte1, unsigned char byte2)
				{
					return static_cast<unsigned short>((byte1 << 8) | byte2);
				}

				/*
				* Retrieves the package type from the given unsigned char
				*/
				static ControlPackageType GetPackageType(const unsigned char data) 
				{
					return ControlPackageType((data >> 4) & 0xff);
				}
				/*
				* Converts a int to 2 bytes splitting the value at the 8th bit.
				* Returns a vector with 2 values data[0]MSB and data[1]LSB
				*/
				static const std::vector<unsigned char> IntToBytes(int value) 
				{
					return std::vector<unsigned char> {
						static_cast<unsigned char>((value >> 8) & 0xff),
						static_cast<unsigned char>(value & 0xff)
					};
				}

			};
		}
	}
}