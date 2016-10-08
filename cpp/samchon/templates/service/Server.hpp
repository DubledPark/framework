#pragma once
#include <samchon/API.hpp>

#include <samchon/protocol/WebServer.hpp>
#include <samchon/protocol/IProtocol.hpp>

#include <samchon/HashMap.hpp>
#include <samchon/library/RWMutex.hpp>

namespace samchon
{
namespace templates
{

/**
 * A system template for Cloud Service.
 * 
 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/CPP-Templates-Cloud_Service)
 * @author Jeongho Nam <http://samchon.org>
 */
namespace service
{
	class User;
	class Client;

	/**
	 * A cloud server.
	 * 
	 * The {@link Server} is an abstract server class, who can build a real-time cloud server, that is following the 
	 * web-socket protocol. Extends this {@link Server} and related classes and overrides abstract methods under below. 
	 * After the overridings, open this {@link Server cloud server} using the {@link open open()} method.
	 * 
	 * - Objects in composite relationship and their factory methods
	 *   - {@link User}: {@link Server.createUser Server.createUser()}
	 *   - {@link Client}: {@link User.createClient User.createClient()}
	 *   - {@link Service}: {@liok Client.createService Client.createService()}
	 * - {@link Invoke} message chains; {@link IProtocol.replyData replyData}
	 *   - {@link Server.replyData}
	 *   - {@link User.replyData}
	 *   - {@link Client.replyData}
	 *   - {@link Service.replyData}
	 * 
	 * ![Class Diagram](http://samchon.github.io/framework/images/design/cpp_class_diagram/templates_cloud_service.png)
	 *
	 * @handbook [Templates - Cloud Service](https://github.com/samchon/framework/wiki/CPP-Templates-Cloud_Service)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	class SAMCHON_FRAMEWORK_API Server
		: public protocol::WebServer,
		public virtual protocol::IProtocol
	{
		friend class User;

	private:
		typedef WebServer super;

		HashMap<std::string, std::shared_ptr<User>> session_map;
		HashMap<std::string, std::shared_ptr<User>> account_map;

		library::RWMutex session_map_mtx;
		library::RWMutex account_map_mtx;

	public:
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		Server();

		/**
		 * Default Destructor.
		 */
		virtual ~Server();

	protected:
		/**
		 * Factory method creating {@link User} object.
		 * 
		 * @return A newly created {@link User} object.
		 */
		virtual auto createUser() -> User* = 0;

	public:
		/* ---------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------- */
		/**
		 * Test wheter an {@link User} exists with the *accountID*.
		 * 
		 * @param accountID Account id of {@link User} to find.
		 * @return Exists or not.
		 */
		auto has(const std::string &accountID) const -> bool
		{
			return account_map.has(accountID);
		};

		/**
		 * Get an {@link User} object by its *accountID*.
		 * 
		 * @param accountID Account id of {@link User} to get.
		 * @return An {@link User} object.
		 */
		auto get(const std::string &accountID) const -> std::shared_ptr<User>
		{
			return account_map.get(accountID);
		};

		/* ---------------------------------------------------------
			MESSAGE CHAIN
		--------------------------------------------------------- */
		/**
		 * Send an {@link Invoke} message.
		 * 
		 * Sends an {@link Invoke} message to all remote clients through the belonged {@link User} and {@link Client} 
		 * objects. Sending the {@link Invoke} message to all remote clients, it's came true by passing through 
		 * {@link User.sendData User.sendData()}. And the {@link User.sendData} also pass through the 
		 * {@link Client.sendData Client.sendData()}.
		 * 
		 * ```typescript
		 * class protocol.service.Server
		 * {
		 *     public sendData(invoke: Invoke): void
		 *     {
		 *         for (user: User in this)
		 *             for (client: Client in user)
		 *                 client.sendData(invoke);
		 *     }
		 * }
		 * ```
		 * 
		 * @param invoke {@link Invoke} message to send to all remote clients.
		 */
		virtual void sendData(std::shared_ptr<protocol::Invoke>) override;

		/**
		 * Handle a replied {@link Invoke} message.
		 * 
		 * The {@link Server.replyData Server.replyData()} is an abstract method that handling {@link Invoke} message 
		 * that should be handled in the {@link Server} level. Overrides this {@link replyData replyData()} method and 
		 * defines what to do with the {@link Invoke} message in this {@link Server} level.
		 * 
		 * @param invoke An {@link Invoke invoke} message to be handled in {@link Server} level.
		 */
		virtual void replyData(std::shared_ptr<protocol::Invoke>) = 0;

	private:
		/* ---------------------------------------------------------
			CLIENT I/O
		--------------------------------------------------------- */
		/**
		 * Add a newly connected remote client.
		 * 
		 * When a {@link WebClientDriver remote client} connects to this cloud server, then {@link Server} queries the
		 * {WebClientDriver.getSessionID session id} of the {@link WebClientDriver remote client}. If the 
		 * {WebClientDriver.getSessionID session id} is new one, then creates a new {@link User} object.
		 * 
		 * At next, creates a {@link Client} object who represents the newly connected remote client and insert the 
		 * {@link Client} object to the matched {@link User} object which is new or ordinary one following the 
		 * {WebClientDriver.getSessionID session id}. At last, a {@link Service} object can be created with referencing 
		 * the {@link WebClientDriver.getPath path}.
		 * 
		 * List of objects can be created by this method.
		 * - {@link User} by {@link createUser createUser()}.
		 * - {@link Client} by {@link User.createClient User.createClient()}.
		 * - {@link Service} by {@link Client.createService Client.createService()}.
		 * 
		 * @param driver A web communicator for remote client.
		 */
		virtual void addClient(std::shared_ptr<protocol::ClientDriver>) final;

		void erase_user(User*);
	};
};
};
};