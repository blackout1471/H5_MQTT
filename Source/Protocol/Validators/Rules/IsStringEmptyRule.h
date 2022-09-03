#pragma once


#include "IRule.h"
#include <string>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class IsStringEmptyRule : public IRule {
			public:
				IsStringEmptyRule(const std::string& stringValue) : m_StringValue(stringValue) {};
				~IsStringEmptyRule() {};

				/*
				* Checks whether a string is empty
				* Returns: True if empty false otherwise
				*/
				inline virtual bool Validate() override {
					return m_StringValue == "";
				}
			private:
				const std::string& m_StringValue;
			};
		}
	}
}