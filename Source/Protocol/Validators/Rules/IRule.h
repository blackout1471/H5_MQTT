#pragma once

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class IRule {
			public:
				virtual bool Validate() = 0;
			};
		}
	}
}