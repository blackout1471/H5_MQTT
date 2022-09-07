#pragma once
#include "Rules/IRule.h"

#include <vector>

namespace MQTT {
	namespace Rules {
		struct EngineRule {
			/*
			* The rule to run
			*/
			IRule* Rule;

			/*
			* When the rule should fail.
			*/
			bool PassOn = false;
		};

		class RuleEngine {
		public:
			/*
			* Creates the rule engines with the specified rules
			*/
			RuleEngine(const std::vector<EngineRule>& rules);
			virtual ~RuleEngine();

			/*
			* Runs the rules linear.
			* Return: True if all validations are Ok, false if not.
			*/
			bool Run();
		private:
			std::vector<EngineRule> m_Rules;
		};
	}
};