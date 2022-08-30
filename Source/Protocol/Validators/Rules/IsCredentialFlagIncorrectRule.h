#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class IsCredentialFlagIncorrectRule : public IRule
			{
			public:
				IsCredentialFlagIncorrectRule(ConnectFlagType flag) : m_Flag(flag) {};
				virtual ~IsCredentialFlagIncorrectRule() {};

				/*
				* Checks whether username is 0 and password is 1.
				* Returns true if it is, false if not.
				*/
				virtual bool Validate() override {
					return !(m_Flag & ConnectFlagType::Username) && (m_Flag & ConnectFlagType::Password);
				}

			private:
				ConnectFlagType m_Flag;
			};

		}
	}
}