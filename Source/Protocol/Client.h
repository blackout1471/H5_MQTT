#pragma once
#include <string>

namespace MQTT {
	namespace Protocol {
		struct Client {
	
		public:
			Client(const std::string& ipAddress, const std::string& identifier) 
				: m_IpAddress(ipAddress), m_Identifier(identifier) {}

			inline const std::string& GetIpAddress() const { return m_IpAddress; }
			inline const std::string& GetIdentifier() const { return m_Identifier; }

		private:
			std::string m_IpAddress;
			std::string m_Identifier;
		};
	}
}