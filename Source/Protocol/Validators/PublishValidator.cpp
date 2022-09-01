#include "mqttpch.h"
#include "PublishValidator.h"


namespace MQTT {
	namespace Protocol {
		namespace Validators {
			PublishValidator::PublishValidator() {}
			PublishValidator::~PublishValidator() {}

			PublishValidator::Action PublishValidator::ValidatePackage(const PublishPackage& package, const Server::MqttClient& client)
			{


				return Acknowledge;
			}
		}
	}
}