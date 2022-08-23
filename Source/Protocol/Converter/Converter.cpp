#include "mqttpch.h"
#include "Converter.h"
#include "Protocol/ConnectPackage.h"

namespace MQTT {
	namespace Protocol {
		namespace Converter {
			Converter::Converter() {}

			Converter::~Converter() {}

			ControlPackageType Converter::GetPackageType(const unsigned char data)
			{
				return (ControlPackageType)((data >> 4) & 0xff);
			}

			int16_t Converter::ByteToInt(unsigned char byte1, unsigned char byte2)
			{
				return (byte1 << 8) | byte2;
			}

			template<>
			bool Converter::Convert<std::vector<unsigned char>, ConnectPackage>(const std::vector<unsigned char>& from, ConnectPackage& to)
			{
				if (from.size() < 3)
					return false;

				const unsigned char* data = from.data();


				to.ControlHeader.PackageType = GetPackageType(*data);
				data++;

				unsigned char remainLength = *data;
				data++;

				to.ConnectVariableHeader.NameLength = ByteToInt(*data, data[1]);
				data += 2;

				to.ConnectVariableHeader.ProtocolName.assign(data, data + to.ConnectVariableHeader.NameLength);
				data += to.ConnectVariableHeader.NameLength;

				to.ConnectVariableHeader.Level = *data;
				data++;

				to.ConnectVariableHeader.VariableLevel = ConnectFlagType(*data);
				data++;

				to.ConnectVariableHeader.KeepAlive = ByteToInt(*data, data[1]);
				data += 2;

				bool hasPayload = (from.data() + remainLength + 2) != data;
				if (!hasPayload)
					return true;

				to.ConnectPayload.ClientIdLength = ByteToInt(*data, data[1]);
				data += 2;

				to.ConnectPayload.ClientId.assign(data, data + to.ConnectPayload.ClientIdLength);
				data += to.ConnectPayload.ClientIdLength;


				if (to.ConnectVariableHeader.VariableLevel & ConnectFlagType::Will_Flag)
				{
					int willLength = ByteToInt(*data, data[1]);
					data += 2;

					auto willTopic = std::string();
					willTopic.assign(data, data + willLength);
					data += willLength;

					auto willMessageLength = ByteToInt(*data, data[1]);
					data += 2;

					auto willMessage = std::string();
					willMessage.assign(data, data + willMessageLength);
					data += willMessageLength;
				}
				
				if (to.ConnectVariableHeader.VariableLevel & ConnectFlagType::Username)
				{
					to.ConnectPayload.UsernameLength = ByteToInt(*data, data[1]);
					data += 2;

					to.ConnectPayload.Username.assign(data, data + to.ConnectPayload.UsernameLength);
					data += to.ConnectPayload.UsernameLength;
				}


				if (to.ConnectVariableHeader.VariableLevel & ConnectFlagType::Password)
				{
					to.ConnectPayload.PasswordLength = ByteToInt(*data, data[1]);
					data += 2;

					to.ConnectPayload.Password.assign(data, data + to.ConnectPayload.PasswordLength);
					data += to.ConnectPayload.PasswordLength;
				}

				return true;
			}

			template<>
			bool Converter::Convert<std::vector<unsigned char>, ConnectPackage>(const std::vector<unsigned char>&, ConnectPackage&);
		}
	}
};