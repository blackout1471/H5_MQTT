#pragma once
#include "IRule.h"

#include <string>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class CorrectProtocolNameRule : public IRule
			{
			public:
				CorrectProtocolNameRule(const std::string& protocolName)
					: m_ProtocolName(protocolName) {};

				/*
				* Checks whether the protocol name matches the given correct one.
				* Returns: true if matches false if not.
				*/
				inline virtual bool Validate() override {
					return m_ProtocolName == "mqtt";
				}

			private:
				const std::string& m_ProtocolName;
			};
		}
	}
}