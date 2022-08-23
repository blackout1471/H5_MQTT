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

				template <class T, class Y>
				bool Convert(const T& from, Y& to);

			private:


			private:
				ControlPackageType GetPackageType(const unsigned char data);
				int16_t ByteToInt(unsigned char byte1, unsigned char byte2);
			};
		}
	}
}