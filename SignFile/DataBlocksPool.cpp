#include "stdafx.h"
#include "DataBlocksPool.h"

using namespace std;

DataBlocksPool::DataBlocksPool(size_t nBlockSize, size_t nMaxBlocksCount)
	: m_nBlockSize(nBlockSize)
	, m_nBlocksCreated(0)
	, m_nMaxBlocksCount(nMaxBlocksCount)
	, m_bBadAllocReceived(false)
{
	CreateBlock();
}


DataBlocksPool::~DataBlocksPool()
{}

bool DataBlocksPool::IsBadAllocReceived() const
{
	return m_bBadAllocReceived;
}

bool DataBlocksPool::IsMaxBlocksCountReached() const
{
	return m_nBlocksCreated >= m_nMaxBlocksCount;
}

bool DataBlocksPool::HasFreeBlock() const
{
	return !m_availableBlocks.empty();
}

size_t DataBlocksPool::BlocksCount() const
{
	return m_nBlocksCreated;
}

bool DataBlocksPool::CreateBlock()
{
	if (m_bBadAllocReceived || m_nBlocksCreated >= m_nMaxBlocksCount)
		return false;

	shared_ptr< DataBlock > pDataBlock;
	try
	{
		pDataBlock.reset(new DataBlock(m_nBlockSize));
	}
	catch (bad_alloc e)
	{
		m_bBadAllocReceived = true;
		return false;
	}

	++m_nBlocksCreated;
	m_availableBlocks.push_back(pDataBlock);
	return true;
}

std::shared_ptr< DataBlock > DataBlocksPool::Acquire()
{
	std::unique_lock< mutex > lock(m_mutex);

	while (m_availableBlocks.empty())
	{
		if (!m_bBadAllocReceived)
			CreateBlock();
		else
			m_condVar.wait(lock);
	}

	std::shared_ptr< DataBlock > dataBlock = m_availableBlocks.front();
	m_availableBlocks.pop_front();

	return dataBlock;
}

void DataBlocksPool::Release(std::shared_ptr< DataBlock > pDataBlock)
{
	std::unique_lock< mutex > lock(m_mutex);

	m_availableBlocks.push_back(pDataBlock);
	m_condVar.notify_one();
}