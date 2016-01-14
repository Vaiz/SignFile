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
	std::list< std::shared_ptr< DataBlock > > m_availableBlocks;	// свободные блоки
	size_t m_nBlockSize;
	size_t m_nBlocksCreated;	// количество созданных блоков
	size_t m_nMaxBlocksCount;	// Максимальное количество блоков, которое можно создать

	bool m_bBadAllocReceived;

	std::condition_variable m_condVar;	// условная переменная, для ожидания появления свободного блока
	std::mutex m_mutex;					// мьютекс, отвечающий за доступ к полям класса
};

