#pragma once
#include "Protocol/Packages.h"

#include <cstdint>

namespace MQTT {
	namespace Protocol {
		namespace Converter {
			class Converter final {
			public:
				Converter();
				~Converter();

				/*
				* Converts between package data and vectors with unsigned char
				* Returns: The success of the conversion
				*/ 
				template <class T, class Y>
				bool Convert(const T& from, Y& to);

			private:
				/*
				* Converts the unsigned char array to connect variable header package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int IntermiadateConvert(const unsigned char* dataPtr, ConnectVariableHeader& package);

				/*
				* Converts the unsigned char array to connect package
				* Returns: the size of the ConnectVariableHeader in the buffer, -1 if error occurred.
				*/
				int IntermiadateConvert(const unsigned char* dataPtr, const ConnectVariableHeader& headerPackage, ConnectPayload& package);

			private:
				/*
				* Retrieves the package type from the given unsigned char
				*/
				ControlPackageType GetPackageType(const unsigned char data);
				/*
				* Converts byte1 as MSB and byte2 as LSB
				* Returns: The result of the conversion.
				*/
				int16_t ByteToInt(unsigned char byte1, unsigned char byte2);
			};
		}
	}
}