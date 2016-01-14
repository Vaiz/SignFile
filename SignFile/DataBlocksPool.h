#pragma once

#include <memory>		// shared_ptr				
#include <condition_variable>
#include <mutex>
#include <list>
#include "DataBlock.h"

class DataBlocksPool
{
public:
	DataBlocksPool(size_t nBlockSize, size_t nMaxBlocksCount);
	~DataBlocksPool();

	bool IsBadAllocReceived() const;
	bool IsMaxBlocksCountReached() const;

	bool HasFreeBlock() const;

	size_t BlocksCount() const;

	std::shared_ptr< DataBlock > Acquire();

	void Release(std::shared_ptr< DataBlock > pDataBlock);

private:
	bool CreateBlock();

private:
	std::list< std::shared_ptr< DataBlock > > m_availableBlocks;	// ��������� �����
	size_t m_nBlockSize;
	size_t m_nBlocksCreated;	// ���������� ��������� ������
	size_t m_nMaxBlocksCount;	// ������������ ���������� ������, ������� ����� �������

	bool m_bBadAllocReceived;

	std::condition_variable m_condVar;	// �������� ����������, ��� �������� ��������� ���������� �����
	std::mutex m_mutex;					// �������, ���������� �� ������ � ����� ������
};

