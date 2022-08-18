#pragma once
#include <string>

namespace Models {

	struct Client {
	private:

		std::string m_IpAddress;
		std::string m_Identifier;

	public:
		Client(std::string _ipAddress, std::string _identifier) {
			m_IpAddress = _ipAddress;
			m_Identifier = _identifier;
		}

		inline std::string GetIpAddress() { return m_IpAddress; }
		inline std::string GetIdentifier() { return m_Identifier; }
	};
}