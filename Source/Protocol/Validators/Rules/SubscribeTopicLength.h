#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"
#include <vector>
namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class SubscribeTopicLength : public IRule {
			public:
				SubscribeTopicLength(std::vector<Protocol::SubscribeTopic> topics) : m_Topics(topics) {};

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
				std::vector<Protocol::SubscribeTopic> m_Topics;
			};
		}
	}
}