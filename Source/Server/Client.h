#pragma once
#include <string>

namespace MQTT {
	namespace Server {
		struct Client
		{
		public:
			Client(const std::string& ipAddress, const std::string& identifier, int connection) 
				: m_IpAddress(ipAddress), m_Identifier(identifier), m_Connection(connection) {};
			virtual ~Client() {};
			const std::string& GetIpAddress() const { return m_IpAddress; }
			const std::string& GetIdentifier() const { return m_Identifier; }
			const int GetConnection() const { return m_Connection; }
		private:
			std::string m_IpAddress;
			std::string m_Identifier;
			int m_Connection;
		};
	}
}