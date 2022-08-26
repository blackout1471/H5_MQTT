#pragma once
#include "Rules/IRule.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class RuleEngine {
			public:
				/*
				* Creates the rule engines with the specified rules
				*/
				RuleEngine(const std::vector<IRule*>& rules);
				virtual ~RuleEngine();

				/*
				* Runs the.
				* Return: True if all validations are Ok, false if not.
				*/
				bool Run();
			private:
				std::vector<IRule*> m_Rules;
			};
		}
	}
}