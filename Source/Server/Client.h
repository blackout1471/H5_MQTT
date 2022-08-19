#pragma once
#include <string>
namespace MQTT {
	namespace Server {
		struct Client
		{
		public:
			Client(const std::string& ipAddress, const std::string& identifier) 
				: m_IpAddress(ipAddress), m_Identifier(identifier) {};
			virtual ~Client(){};
			const std::string& GetIpAddress() { return m_IpAddress; }
			const std::string& GetIdentifier() { return m_Identifier; }
		private:
			std::string m_IpAddress;
			std::string m_Identifier;
		};
	}
}