#pragma once
#include "Rules/IRule.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class RuleEngine {
			public:
				RuleEngine(const std::vector<IRule*>& rules);
				virtual ~RuleEngine();

				bool Run();
			private:
				std::vector<IRule*> m_Rules;
			};
		}
	}
}