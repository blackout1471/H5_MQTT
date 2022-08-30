#pragma once

#include "IRule.h"
#include "Server/MqttClient.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class Protocol311Rule : public IRule {
			public:
				Protocol311Rule(unsigned char level) : m_ProtocolLevel(level) {};

				/*
				* Checks whether the given protocol level is the correct one
				* Return: true if protocol equals 311 standard (4) and false if not.
				*/
				inline virtual bool Validate() override 
				{
					return m_ProtocolLevel == 4;
				}

			private:
				const unsigned char m_ProtocolLevel;
			};
		}
	}
}