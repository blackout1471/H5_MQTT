#pragma once
#include "IRule.h"
#include "Server/MqttClient.h"

#include <vector>
#include <string>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class GenerateSessionMessageRule : public IRule
			{
			public:
				GenerateSessionMessageRule(const std::string& clientId, const Server::MqttClient* clientState, std::vector<unsigned char>& message) 
					:  m_ClientId(clientId), m_ClientState(clientState), m_Message(message) {};
				virtual ~GenerateSessionMessageRule() {};

				/*
				* Checks if clientstate is valid, and whether packageclientId is the correct size.
				* If packagesize is 0 or clientstate is not valid generate reject package.
				* else generate acknowledgement package.
				* Returns: True if acknowledgement package is generated, false else
				*/
				virtual bool Validate() override {

					if (m_ClientId.size() == 0 || !m_ClientState)

				};

			private:
				const std::string& m_ClientId;
				const Server::MqttClient* m_ClientState;
				std::vector<unsigned char>& m_Message;


			};
		}
	}
}