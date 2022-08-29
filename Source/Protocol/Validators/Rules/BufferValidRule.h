#pragma once
#include "IRule.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class BufferValidRule : public IRule {
			public:
				BufferValidRule(const std::vector<unsigned char>& buffer) : m_Buffer(buffer) {}

				/*
				* Validate whether the buffer is bigger than 2 bytes
				*/
				virtual virtual bool Validate() override
				{
					return m_Buffer.size() > 2;
				}

			private:
				const std::vector<unsigned char>& m_Buffer;
			};
		}
	}
}