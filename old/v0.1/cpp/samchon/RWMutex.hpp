#pragma once
#include <samchon/SamchonLibrary.hpp>

namespace std
{
	class mutex;
	template<typename _Ty> struct atomic;
	class condition_variable;
};

namespace samchon
{
	using namespace std;

	class SAMCHON_LIBRARY_API RWMutex
	{
	private:
		//��� ���¸� �������� ����
		atomic<size_t> *readingCount;
		atomic<bool> *isWriting;

		//��� ��ƿ��Ƽ
		condition_variable *readCV;
		condition_variable *writeCV;

		mutex *readMutex;
		mutex *writeMutex;

	public:
		RWMutex();
		virtual ~RWMutex();

		void readLock() const;
		void readUnlock() const;

		void writeLock();
		void writeUnlock();
	};
};