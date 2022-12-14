#pragma once
#include "IRule.h"

#include <vector>

namespace MQTT {
	namespace Rules {
		class BufferValidRule : public IRule {
		public:
			BufferValidRule(const std::vector<unsigned char>& buffer) : m_Buffer(buffer) {}

			/*
			* Validate whether the buffer is bigger than 2 bytes
			* Returns: True if it is bigger, false otherwise.
			*/
			virtual bool Validate() override
			{
				return m_Buffer.size() > 2;
			}

		private:
			const std::vector<unsigned char>& m_Buffer;
		};
	}
}