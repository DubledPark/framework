#pragma once
#include <samchon/protocol/master/ParallelClientArray.hpp>

#include <samchon/example/interaction/ChiefDriver.hpp>
#include <samchon/example/interaction/SlaveDriver.hpp>
#include <samchon/protocol/Invoke.hpp>

#include <array>
#include <iostream>
#include <mutex>

namespace samchon
{
	namespace example
	{
		namespace interaction
		{
			/**
			 * @brief A master of parallel system solving something.
			 * 		  
			 * @details
			 * <p> Master is an abstract and example class, for providing guidance of realizing parallel processing 
			 * system for users, within framework of master. The Master class represents a master of parallel 
			 * processing system in the master side. </p>
			 * 
			 * <p> A master system (represented by Master and MasterDriver) is associated with a chief system 
			 * (represented by Cheif and CheifDriver) and slave systems (Slave and SlaveDriver). The master system
			 * gets optimization order from chief system and shifts and distributes orders to its slave systems. When
			 * slave systems report optimization result, the master system aggregates those results and reports the
			 * aggregated results to chief system. </p>
			 * 
			 * <p> @image html  cpp/example_interaction.png
			 *	   @image latex cpp/example_interaction.png </p>
			 * 
			 * <p> @image html  conception/example_interaction_network_diagram.png
			 *	   @image latex conception/example_interaction_network_diagram.png </p>
			 * 
			 * \par [Inherited]
			 *		@copydetails master::ParallelClientArray
			 * 
			 * @see protocol::master
			 * @author Jeongho Nam
			 */
			class Master
				: public protocol::master::ParallelClientArray
			{
			private:
				typedef protocol::master::ParallelClientArray super;

			protected:
				/**
				 * @brief A boundary object interacting with a Cheif.
				 */
				std::unique_ptr<ChiefDriver> chiefDriver;

				/**
				 * @brief A mutex for optimization.
				 * 		  
				 * @details 
				 * The mutex exists for ensuring concurrency of aggregation of optimization reported from slave systems.
				 */
				std::mutex mtx;

				/**
				 * @brief Number of slaves who'd completed an optimization.
				 */
				size_t optimized;

			public:
				/* ------------------------------------------------------------
					CONSTRUCTORS
				------------------------------------------------------------ */
				/**
				 * @brief Construct from a port number to listen.
				 * 		  
				 * @param port Port number to listen.
				 */
				Master(int port)
					: super()
				{
					this->port = port;
					chiefDriver.reset(new ChiefDriver(this, port + 10));

					optimized = 0;
				};
				virtual ~Master() = default;

				virtual void start() override
				{
					std::array<std::thread, 2> threadArray =
					{
						std::thread([this]()
						{
							super::start();
						}),
						std::thread([this]()
						{
							chiefDriver->open();
						})
					};

					for (auto it = threadArray.begin(); it != threadArray.end(); it++)
						it->join();
				};

			protected:
				virtual auto createChild(std::shared_ptr<library::XML>) -> protocol::ExternalSystem* override
				{
					return new SlaveDriver();
				};

			public:
				/* ------------------------------------------------------------
					INVOKE MESSAGE CHAIN
				------------------------------------------------------------ */
				virtual void replyData(std::shared_ptr<protocol::Invoke> invoke) override
				{
					if (invoke->getListener() == "optimize")
						optimize(invoke->at(0)->getvalueAsXML());
					else if (invoke->getListener() == "replyOptimization")
						replyOptimization(invoke->at(0)->getvalueAsXML());
				};

			protected:
				virtual void addClient(protocol::Socket *socket) override
				{
					std::cout << "A client has connected." << std::endl;

					super::addClient(socket);
				};

				/**
				 * @brief Optimize something.
				 * 
				 * @details
				 * <p> Master gets an XML object representing parametric value and basic data used for optimzation are
				 * came from Cheif. Master constructs some object to optimize and distributes the optimization order to
				 * master's slave systems by segmentation. </p>
				 * 		  		  
				 * @param xml XML object representing parametric value and basic data used for optimization.
				 */
				virtual void optimize(std::shared_ptr<library::XML> xml)
				{
					std::cout << "----------------------------------------------------------------------------" << std::endl;
					std::cout << "	OPTIMIZE" << std::endl;
					std::cout << "----------------------------------------------------------------------------" << std::endl;

					optimized = 0;
				};

				/**
				 * @brief Handle (replied) optimized value from a slave system.
				 * 		  
				 * @details
				 * <p> When gets optimization result from Slave systems, Master aggregates them and derives the best
				 * solution between those results and reports the best solution to the Cheif system. </p>
				 * 		  
				 * @param xml XML object representing the optimized value.
				 */
				virtual void replyOptimization(std::shared_ptr<library::XML> xml) = 0;
			};
		};
	};
};