#pragma once
#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			template <class T>
			class IPackageConverter {
			public:
				// Converts message to corresponding package
				virtual T ConvertToPackage(std::vector<unsigned char> _message) = 0;
				// Converts package to valid message
				virtual std::vector<unsigned char> ConvertFromPackage(T _package) = 0;
			};
		}
	}
}