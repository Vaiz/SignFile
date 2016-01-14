#include "stdafx.h"
#include "DataQueue.h"

using namespace std;

DataQueue::DataQueue() 
	: m_bDone(false) 
{
}

DataQueue::~DataQueue()
{
	Stop();
}

void DataQueue::Push(const std::shared_ptr< DataBlock > dataBlock)
{
	std::unique_lock< mutex > lock(m_mutex);
	m_queue.push(dataBlock);
	m_condVar.notify_one();
}

size_t DataQueue::Size() const
{
	return m_queue.size();
}

bool DataQueue::WaitForNextBlock()
{
	std::unique_lock< mutex > lock(m_mutex);

	while (m_queue.empty() && !m_bDone)
		m_condVar.wait(lock);

	if (m_queue.empty())
		return false;

	return true;
}

std::shared_ptr< DataBlock > DataQueue::Pop()
{
	std::unique_lock< mutex > lock(m_mutex);
	std::shared_ptr< DataBlock > dataBlock;

	if (!m_queue.empty())
	{
		dataBlock = m_queue.front();
		m_queue.pop();
	}

	return dataBlock;
}

void DataQueue::Stop()
{
	std::unique_lock< mutex > lock(m_mutex);
	m_bDone = true;
	m_condVar.notify_all();
}