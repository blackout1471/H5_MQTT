#include "mqttpch.h"
#include "PublishValidator.h"
#include "Rules/Rules.h"
#include <Protocol/Validators/RuleEngine.h>


namespace MQTT {
	namespace Protocol {
		namespace Validators {
			PublishValidator::PublishValidator() {}
			PublishValidator::~PublishValidator() {}

			PublishValidator::Action PublishValidator::ValidatePackage(PublishPackage& package, const Server::MqttClient& client)
			{
				auto shouldDisconnect = !ValidateQoSBytesRule(package)
					.Validate();
				if (shouldDisconnect)
					return DisconnectClient;

				auto ruleHandler = std::vector<IRule*>{
					new PublishQosRule(package),
					new PublishStoreMessageRule(package),
					new PublishStoreNonQoSMessagesRule(package)
				};
				for (auto* rule : ruleHandler)
				{
					rule->Validate();
					delete rule;
				}

				auto shouldRejectPublish = RuleEngine({
					{new PublishValidTopicNameRule(package), true},
					{new IsStringEmptyRule(package.VariableHeader.TopicName), false}
				}).Run();
				if (!shouldRejectPublish)
					return RejectPublish;

				return AcknowledgePublish;
			}
		}
	}
}