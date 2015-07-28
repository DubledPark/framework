#pragma once
#include <samchon\API.hpp>

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace std
{
	template <typename _Ty> struct atomic;
	class condition_variable;
	class mutex;
};
namespace samchon
{
	namespace library
	{
		class SAMCHON_FRAMEWORK_API RWMutex
		{
		private:
			//��� ���¸� �������� ����
			std::atomic<size_t> *readingCount;
			std::atomic<bool> *isWriting;
			std::mutex *minusMtx;

			//��� ��ƿ��Ƽ
			std::condition_variable *cv;
			std::mutex *readMtx;
			std::mutex *writeMtx;

		public:
			RWMutex();
			~RWMutex();

			void readLock() const;
			void readUnlock() const;

			void writeLock();
			void writeUnlock();
		};

		class SAMCHON_FRAMEWORK_API RWUniqueLock
		{
		private:
			RWMutex *mtx;
			int direction;

		public:
			enum
			{
				READ = 0,
				WRITE = 1
			};

			RWUniqueLock(RWMutex&, int direction);
			~RWUniqueLock();
		};
		class SAMCHON_FRAMEWORK_API ReadUniqueLock
			: public RWUniqueLock
		{
		private:
			typedef RWUniqueLock super;

		public:
			ReadUniqueLock(RWMutex&);
		};
		class SAMCHON_FRAMEWORK_API WriteUniqueLock
			: public RWUniqueLock
		{
		private:
			typedef RWUniqueLock super;

		public:
			WriteUniqueLock(RWMutex&);
		};
	};
};