#pragma once
#include "IPackageConverter.h"
#include "Protocol/ConnectAckPackage.h"


namespace MQTT {


	namespace Protocol {


		namespace Converters
		{
			class ConnectAckConverter : IPackageConverter<ConnectAckPackage>
			{
			public:
				virtual ConnectAckPackage ConvertToPackage(std::vector<unsigned char> _message) override;
				virtual std::vector<unsigned char> ConvertFromPackage(ConnectAckPackage _package) override;
			};
		}
	}
}