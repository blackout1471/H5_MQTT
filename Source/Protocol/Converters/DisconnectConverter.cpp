#include "mqttpch.h"
#include "DisconnectConverter.h"
#include "ConverterUtility.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			const MqttPackages::DisconnectPackage DisconnectConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{
				auto package = MqttPackages::DisconnectPackage();
				package.Header.PackageType = ConverterUtility::GetPackageType(buffer[0]);
				return package;
			}
			const std::vector<unsigned char> DisconnectConverter::ToBuffer(const MqttPackages::DisconnectPackage& to)
			{
				
				return std::vector<unsigned char>();
			}
		}
	}
}
