#include "mqttpch.h"
#include "RuleEngine.h"
#pragma once

namespace MQTT {
	namespace Rules {
		RuleEngine::RuleEngine(const std::vector<EngineRule>& rules) : m_Rules(rules)
		{};

		RuleEngine::~RuleEngine()
		{
			for (auto rule : m_Rules)
				delete rule.Rule;
		};

		bool RuleEngine::Run()
		{
			for (auto rule : m_Rules)
				if (rule.PassOn != rule.Rule->Validate())
					return false;

			return true;
		};
	}
};