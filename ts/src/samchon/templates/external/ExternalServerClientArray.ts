﻿/// <reference path="../../API.ts" />

/// <reference path="ExternalSystemArray.ts" />

namespace samchon.templates.external
{
	/**
	 * An interface for an {@link ExternalSystemArray} accepts {@link ExternalSystem external clients} as a
	 * {@link IServer server} and connects to {@link IExternalServer} as **client**, at the same time.
	 * 
	 * The easiest way to defining an {@link IExternalServerClientArray} who opens server, accepts
	 * {@link ExternalSystem external clients} and connects to {@link IExternalServer external servers} is to extending 
	 * one of below, who are derived from this interface {@link IExternalServerClientArray}. However, if you can't 
	 * specify an {@link ExternalSystemArray} to be whether server or client or even can both them, then make a class 
	 * (let's name it as **BaseSystemArray**) extending {@link ExternalSystemArray} and make a new class (now, I name 
	 * it **BaseServerClientArray**) extending **BaseSystemArray** and implementing this interface 
	 * {@link IExternalServerClientArray}. Define the **BaseServerClientArray** following those codes on below:
	 * 
	 * <ul>
	 *	<li> {@link ExternalServerClientArray}:
	 *		<a href="https://github.com/samchon/framework/blob/master/ts/src/samchon/templates/external/ExternalServerClientArray.ts"
	 *		   target="_blank"> View source code on GitHub </a>
	 *	</li>
	 *	<li> {@link ParallelServerClientArray}:
	 *		<a href="https://github.com/samchon/framework/blob/master/ts/src/samchon/templates/master/ParallelServerClientArray.ts"
	 *		   target="_blank"> View source code on GitHub </a>
	 *	</li>
	 *	<li> {@link DistributedServerClientArray}:
	 *		<a href="https://github.com/samchon/framework/blob/master/ts/src/samchon/templates/master/DistributedServerClientArray.ts"
	 *		   target="_blank"> View source code on GitHub </a>
	 *	</li>
	 * </ul>
	 * 
	 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export interface IExternalServerClientArray<System extends ExternalSystem>
		extends IExternalClientArray<System>
	{
		/**
		 * Connect to {@link IExternalServer external servers}.
		 * 
		 * This method calls children elements' method {@link IExternalServer.connect} gradually.
		 */
		connect(): void;
	}

	/**
	 * An array and manager of {@link IExternalServer external servers} and {@link ExternalSystem external clients}.
	 * 
	 * The {@link ExternalServerClientArray} is an abstract class, derived from the {@link ExternalSystemArray} class,
	 * opening a server accepting {@link ExternalSystem external clients} and being a client connecting to
	 * {@link IExternalServer external servers} at the same time.
	 *
	 * Extends this {@link ExternalServerClientArray} and overrides below methods. After the overridings, open server
	 * with {@link open open()} method and connect to {@link IExternalServer external servers} through the
	 * {@link connect connect()} method.
	 *
	 * - {@link createServerBase createServerBase()}
	 * - {@link createExternalClient createExternalClient()}
	 * - {@link createExternalServer createExternalServer()}
	 * 
	 * #### [Inherited] {@link ExternalSystemArray}
	 * The {@link ExternalSystemArray} is an abstract class containing and managing external system drivers,
	 * {@link ExternalSystem} objects. Within framewokr of network, {@link ExternalSystemArray} represents your system
	 * and children {@link ExternalSystem} objects represent remote, external systems connected with your system.
	 * With this {@link ExternalSystemArray}, you can manage multiple external systems as a group.
	 *
	 * <a href="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
	 *		  target="_blank">
	 *	<img src="http://samchon.github.io/framework/images/design/ts_class_diagram/templates_external_system.png"
	 *		 style="max-width: 100%" />
	 * </a>
	 *
	 * #### Proxy Pattern
	 * The {@link ExternalSystemArray} class can use *Proxy Pattern*. In framework within user, which
	 * {@link ExternalSystem external system} is connected with {@link ExternalSystemArray this system}, it's not
	 * important. Only interested in user's perspective is *which can be done*.
	 *
	 * By using the *logical proxy*, user dont't need to know which {@link ExternalSystemRole role} is belonged
	 * to which {@link ExternalSystem system}. Just access to a role directly from {@link ExternalSystemArray.getRole}.
	 * Sends and receives {@link Invoke} message via the {@link ExternalSystemRole role}.
	 *
	 * <ul>
	 *	<li>
	 *		{@link ExternalSystemRole} can be accessed from {@link ExternalSystemArray} directly, without inteferring
	 *		from {@link ExternalSystem}, with {@link ExternalSystemArray.getRole}.
	 *	</li>
	 *	<li>
	 *		When you want to send an {@link Invoke} message to the belonged {@link ExternalSystem system}, just call
	 *		{@link ExternalSystemRole.sendData ExternalSystemRole.sendData()}. Then, the message will be sent to the
	 *		external system.
	 *	</li>
	 *	<li> Those strategy is called *Proxy Pattern*. </li>
	 * </ul>
	 *
	 * @handbook [Templates - External System](https://github.com/samchon/framework/wiki/TypeScript-Templates-External_System)
	 * @author Jeongho Nam <http://samchon.org>
	 */
	export abstract class ExternalServerClientArray<T extends ExternalSystem>
		extends ExternalClientArray<T>
		implements IExternalServerClientArray<T>
	{
		/* ---------------------------------------------------------
			CONSTRUCTORS
		--------------------------------------------------------- */
		/**
		 * Default Constructor.
		 */
		public constructor()
		{
			super();
		}

		/**
		 * Factory method of a child Entity.
		 *
		 * This method is migrated to {@link createExternalServer}. Override the {@link createExternalServer} method.
		 *
		 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
		 * @return A new child Entity via {@link createExternalServer createExternalServer()}.
		 */
		public createChild(xml: library.XML): T
		{
			return this.createExternalServer(xml);
		}

		/**
		 * Factory method creating an {@link IExternalServer} object.
		 *
		 * @param xml An {@link XML} object represents child element, so that can identify the type of child to create.
		 * @return A newly created {@link IExternalServer} object.
		 */
		protected abstract createExternalServer(xml: library.XML): T;

		/* ---------------------------------------------------------
			METHOD OF CLIENT
		--------------------------------------------------------- */
		/**
		 * @inheritdoc
		 */
		public connect(): void
		{
			for (let i: number = 0; i < this.size(); i++)
			{
				let system: ExternalSystem = this.at(i);
				if ((system as IExternalServer).connect == undefined)
					continue;

				(system as IExternalServer).connect();
			}
		}
	}
}