#include "IProtocolHandler.h"
#include "MqttClient.h"
#include "Server/IServer.h"
#include "Managers/MqttManager.h"
#include "Managers/SubscribeManager.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		class Mqtt311Handler final : public IProtocolHandler
		{
		public:
			Mqtt311Handler();
			~Mqtt311Handler();

			// <inheritDoc />
			virtual void OnClientConnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) override;
			// <inheritDoc />
			virtual void OnClientDisconnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) override;
			// <inheritDoc />
			virtual void OnClientPublished(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) override;
			// <inheritDoc />
			virtual void OnClientSubscribed(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) override;

		private:
			// If a client state with matching connection id exists, set is connected to false and remove will message.
			void DisconnectClientState(const Server::Client& client, Server::IServer* server);

			/*
			* Retrieves the client state from the given client id.
			* returns: nullptr if not found.
			*/
			Protocol::MqttClient* GetClientStateFromClientId(const std::string& clientId);

			/*
			* Retrieves the client state from the given identifier.
			* Returns: clientstate or nullptr if not found.
			*/
			Protocol::MqttClient* GetClientStateFromIdentifier(const std::string& identifier);
			Server::Client* GetClientFromIdentifier(const std::string& identifier, const Server::IServer* server);

		private:
			std::vector<Protocol::MqttClient*> m_ClientStates;
			MqttManager m_Manager;
			SubscribeManager m_SubscribeManager;
		};
	}
}