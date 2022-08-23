#pragma once
#include "BasePackageConverter.h"
#include "Protocol/ConnectAckPackage.h"


namespace MQTT {


	namespace Protocol {


		namespace Converters
		{
			class ConnectAckConverter : BasePackageConverter<ConnectAckPackage>
			{
			public:
				virtual ConnectAckPackage ConvertToPackage(std::vector<unsigned char> _message) override;
				virtual std::vector<unsigned char> ConvertFromPackage(ConnectAckPackage _package) override;
			};
		}
	}
}