#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishStoreNonQoSMessagesRule : public IRule
			{
			public:
				PublishStoreNonQoSMessagesRule(const PublishPackage& package, SubscribeManager& manager) : m_Package(package), m_Manager(manager) {};
				~PublishStoreNonQoSMessagesRule() {};

				/*
				* Checks whether both retain and qos = 0, and discards the previous retain message if they are.
				* After discarding the previous messages, the retain message will be the one in the package.
				* Returns: true if retain and qos = 0, false else.
				*/
				virtual bool Validate() override {
					
					if (m_Package.HeaderFlag & PublishHeaderFlag::Retain &&
						(m_Package.HeaderFlag & PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & PublishHeaderFlag::QoSMsb) == 0)
					{
						std::vector<unsigned char> topic(m_Package.VariableHeader.TopicName.begin(), m_Package.VariableHeader.TopicName.end());

						auto matchinTree = m_Manager.GetMatchingBTree(topic);


						if (matchinTree != nullptr)
						{
							matchinTree->GetSavedMessages().clear();							

							auto qos = (m_Package.HeaderFlag & PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & PublishHeaderFlag::QoSMsb);
							matchinTree->AddSavedMessage(SubscribeSavedMessage(m_Package.Payload, qos));
						}

						return true;
					}

					return false;
				}

			private:
				const PublishPackage& m_Package;
				SubscribeManager& m_Manager;
			};
		}
	}
}
