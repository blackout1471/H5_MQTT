#pragma once
#include <string>

namespace MQTT {
	namespace Server {
		struct Client
		{
		public:
			/*
			* Creates a client with a unique identifier and the connection identifier
			*/
			Client(const std::string& identifier, int connection) 
				: m_Identifier(identifier), m_Connection(connection) {};
			virtual ~Client() {};

			/*
			* Retrieve the unique client identifier
			*/
			const std::string& GetIdentifier() const { return m_Identifier; }

			/*
			* Retrieve the connection identifier.
			*/
			const int GetConnection() const { return m_Connection; }
		private:
			std::string m_Identifier;
			int m_Connection;
		};
	}
}