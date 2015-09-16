#include <samchon/RWMutex.hpp>

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace samchon
{
	RWMutex::RWMutex()
	{
		readingCount = new atomic<size_t>();
		isWriting = new atomic<bool>();

		readCV = new condition_variable();
		writeCV = new condition_variable();

		readMutex = new mutex();
		writeMutex = new mutex();

		*readingCount = 0;
		*isWriting = false;
	}
	RWMutex::~RWMutex()
	{
		delete readingCount;
		delete isWriting;

		delete readCV;
		delete writeCV;

		delete readMutex;
		delete writeMutex;
	}

	void RWMutex::readLock() const
	{
		unique_lock<mutex> uk(*readMutex);
		while (*isWriting)
			readCV->wait(uk);

		readingCount->operator++();
	};
	void RWMutex::readUnlock() const
	{
		//�б� ����� �ʹ� ���� ���� ���� ����Ͽ�
		if (readingCount->load() > 0)
			readingCount->operator--();

		//�б����� ��������Ը� �˷��� �ȴ�.
		writeCV->notify_all();
	};

	void RWMutex::writeLock()
	{
		unique_lock<mutex> uk(*writeMutex);

		//�̹� ������� �ɷ��ְų�, 
		//�б���� �ɷ��ִ� ���¿� ���
		while (*isWriting || *readingCount > 0)
			writeCV->wait(uk);

		*isWriting = true;
	};
	void RWMutex::writeUnlock()
	{
		*isWriting = false;

		//���� ����� �б�-������� ��� �˾ƾ� �� ����
		readCV->notify_all();
		writeCV->notify_all();
	};
}