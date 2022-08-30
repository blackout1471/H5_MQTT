#include "mqttpch.h"
#include "ConnectConverter.h"
#include "ConverterUtility.h"

#include <stdexcept>

namespace MQTT {
    namespace Protocol {
        namespace Converters {

            ConnectConverter::ConnectConverter() {}

            ConnectConverter::~ConnectConverter() {}

            const ConnectPackage ConnectConverter::ToPackage(const std::vector<unsigned char>& buffer)
            {
                auto package = ConnectPackage();

                // Check if the buffer has the minimum requirements
                if (buffer.size() < 3)
                    return package;

                const unsigned char* data = buffer.data();

                // Get package type. Should always be Connect :)
                package.Header.PackageType = ConverterUtility::GetPackageType(*data);
                data++;

                unsigned char remainLength = *data;
                data++;

                // Convert the variable header.
                data += ConvertToVariableHeader(data, package.VariableHeader);

                // If we have payload then convert.
                bool hasPayload = (buffer.data() + remainLength + 2) != data;
                if (!hasPayload)
                    return package;

                int payloadSize = ConvertToPayload(data, package.VariableHeader, package.Payload);

                return package;
            }

            const std::vector<unsigned char> ConnectConverter::ToBuffer(const ConnectPackage& to)
            {
                throw std::logic_error("Not implemented yet");
            }

            int ConnectConverter::ConvertToVariableHeader(const unsigned char* dataPtr, ConnectVariableHeader& package)
            {
                const unsigned char* data = dataPtr;

                auto nameLength = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                package.ProtocolName = std::string(data, data + nameLength);
                data += nameLength;

                package.Level = *data;
                data++;

                package.VariableLevel = ConnectFlagType(*data);
                data++;

                package.KeepAlive = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                return data - dataPtr;
            }

            int ConnectConverter::ConvertToPayload(const unsigned char* dataPtr, const ConnectVariableHeader& headerPackage, ConnectPayload& package)
            {
                const unsigned char* data = dataPtr;

                auto clientIdLength = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                package.ClientId = std::string(data, data + clientIdLength);
                data += clientIdLength;


                if (headerPackage.VariableLevel & ConnectFlagType::Will_Flag)
                {
                    auto willLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.WillTopic = std::string(data, data + willLength);
                    data += willLength;

                    auto willMessageLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.WillMessage = std::string(data, data + willMessageLength);
                    data += willMessageLength;
                }

                if (headerPackage.VariableLevel & ConnectFlagType::Username)
                {
                    auto usernameLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.Username = std::string(data, data + usernameLength);
                    data += usernameLength;
                }


                if (headerPackage.VariableLevel & ConnectFlagType::Password)
                {
                    auto passwordLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.Password = std::string(data, data + passwordLength);
                    data += passwordLength;
                }

                return data - dataPtr;
            }
        }
    }
}
