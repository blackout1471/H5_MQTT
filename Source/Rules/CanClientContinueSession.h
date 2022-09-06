#pragma once
#include "IRule.h"
#include "Server/MqttClient.h"
#include "Protocol/Managers/MqttManager.h"

#include <vector>
#include <string>

namespace MQTT {
	namespace Rules {
		class CanClientContinueSession : public IRule
		{
		public:
			CanClientContinueSession(const std::string& clientId, const Server::MqttClient* clientState) 
				:  m_ClientId(clientId), m_ClientState(clientState) {};
			virtual ~CanClientContinueSession() {};

			/*
			* Checks if clientstate is valid, and whether packageclientId is the correct size.
			* If packagesize is 0 or clientstate is not valid generate reject package.
			* else generate acknowledgement package.
			* Returns: True if acknowledgement package is generated, false else
			*/
			virtual bool Validate() override {

				if (m_ClientId.size() == 0 || !m_ClientState)
					return false;
				else
					return true;

				return false;
			};

		private:
			const std::string& m_ClientId;
			const Server::MqttClient* m_ClientState;
		};
	}
}