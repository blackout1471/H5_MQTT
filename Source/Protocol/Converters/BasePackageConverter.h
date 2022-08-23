#pragma once
#include <vector>
#include "mqttpch.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			template <class T>
			class BasePackageConverter {
			public:
				virtual T ConvertToPackage(std::vector<unsigned char> _message) = 0;
				virtual std::vector<unsigned char> ConvertFromPackage(T _package) = 0;
			};
		}
	}
}