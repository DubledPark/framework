#pragma once
#include <samchon/API.hpp>

#include <atomic>

namespace std
{
	template <typename _Ty>
	struct atomic;
};
namespace samchon
{
	namespace library
	{
		class RWMutex;

		/**
		* @brief Shared lock from a RWMutex.
		*
		* @details
		* <p> A SharedWriteLock is an object manages a RWMutex with shared ownership in both states. </p>
		*	\li locked
		*	\li unlocked
		*
		* <p> On default construction, the object acquires a RWMutex object, for whose locking and 
		* unlocking operations becomes responsible. When copy constructions, responsibilities of
		* locking and unlocking RWMutex are shared with copied with those SharedWriteLock objects. </p>
		*
		* <p> The class shared_lock is a general-purpose shared mutex ownership wrapper allowing deferred 
		* locking, timed locking and transfer of lock ownership. Locking a shared_lock locks the associated 
		* shared mutex in shared mode (to lock it in exclusive mode, std::unique_lock can be used) </p>
		*
		* <p> This class guarantees a unlocked status on destruction of all shared objects (even if not 
		* called explicitly). Therefore it is especially useful as an object with automatic duration, 
		* as it guarantees the RWMutex object is properly unlocked in case an exception is thrown. </p>
		*
		* <p> Referenced comments of std::unique_lock </p>
		*	\li http://www.cplusplus.com/reference/mutex/unique_lock/
		*
		* @image html cpp/library_critical_section.png
		* @image latex cpp/library_critical_section.png
		*
		* @note
		* <p> Though, that the SharedWriteLock object does not manage the lifetime of the RWMutex
		* object in any way: the duration of the RWMutex object shall extend at least until
		* the destruction of the SharedWriteLock that manages it. </p>
		*
		* @see UniqueAcquire
		* @author Jeongho Nam
		*/
		class SAMCHON_FRAMEWORK_API SharedWriteLock
		{
		private:
			/**
			* @brief Managed RWMutex
			*/
			RWMutex *semaphore;

			/**
			* @brief Referencing count sharing same RWMutex.
			*/
			std::atomic<size_t> *reference;

			/**
			* @brief Whether the mutex was locked by SharedWriteLock
			*/
			std::atomic<bool> *isLocked;

		public:
			/* -----------------------------------------------------------
			CONSTRUCTORS
			----------------------------------------------------------- */
			/**
			* @brief Construct from RWMutex
			*
			* @param semaphore RWMutex to manage
			* @param doLock Whether to lock directly or not
			*/
			SharedWriteLock(RWMutex &, bool = true);

			/**
			* @brief Copy Constructor
			*/
			SharedWriteLock(const SharedWriteLock &);

			/**
			* @brief Move Constructor
			*/
			SharedWriteLock(SharedWriteLock&&);

			/**
			* @brief Default Destructor
			*/
			~SharedWriteLock();

			/* -----------------------------------------------------------
			LOCKERS
			----------------------------------------------------------- */
			/**
			* @copydoc RWMutex::writeLock()
			*/
			void lock();

			/**
			* @copydoc RWMutex::writeUnlock()
			*/
			void unlock();

			/**
			* @copydoc RWMutex::tryAcquire()
			*/
			//auto tryLock() -> bool;
		};
	};
};