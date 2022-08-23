#pragma once
#include <cstdint>
#include <vector>

namespace MQTT {
	namespace Protocol {

		struct ConnectPayload
		{
			int16_t ClientIdLength;
			std::vector<unsigned char> ClientId;

			int16_t UsernameLength;
			std::vector<unsigned char> Username;

			int16_t PasswordLength;
			std::vector<unsigned char> Password;
		};
	}
}