#include "RuleEngine.h"
#pragma once

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			RuleEngine::RuleEngine(const std::vector<IRule*>& rules) : m_Rules(rules)
			{}

			RuleEngine::~RuleEngine()
			{
				for (auto* rule : m_Rules)
					delete rule;
			}

			bool RuleEngine::Run()
			{
				for (auto* rule : m_Rules)
					if (!rule->Validate())
						return false;

				return true;
			}
		}
	}
}