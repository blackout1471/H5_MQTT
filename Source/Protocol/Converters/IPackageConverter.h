#pragma once

#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			template <class T>
			class IPackageConverter
			{
			public:
				IPackageConverter() {};
				virtual ~IPackageConverter() {};

				/*
				* Converts between the templated class and the buffer
				* Returns: The package from the template type
				*/
				virtual const T ToPackage(const std::vector<unsigned char>& buffer) = 0;

				/*
				* Converts between templated class and buffer
				* Returns: The buffer converted from templated type.
				*/
				virtual const std::vector<unsigned char> ToBuffer(const T& to) = 0;
			};
		}
	}
}