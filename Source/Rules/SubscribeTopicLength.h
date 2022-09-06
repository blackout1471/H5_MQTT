#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"
#include <vector>
namespace MQTT {
	namespace Rules {
		class SubscribeTopicLength : public IRule {
		public:
			SubscribeTopicLength(std::vector<MqttPackages::SubscribeTopic> topics) : m_Topics(topics) {};

			/*
			* Checks whether the payload topic is longer then 1 character
			* Return: false if topic size is 0
			*/
			inline virtual bool Validate() override
			{
				for (auto topic : m_Topics)
				{
					for (auto& path : topic.Paths)
					{
						if (path.size() == 0)
							return false;
					}
				}
				return true;
			}

		private:
			std::vector<MqttPackages::SubscribeTopic> m_Topics;
		};
	}
}