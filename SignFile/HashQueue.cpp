#include "stdafx.h"
#include "HashQueue.h"

using namespace std;

HashQueue::HashQueue()
	: m_nextBlockIndex(0)
	, m_bDone(false)
{
}

HashQueue::~HashQueue()
{
	Stop();
}

void HashQueue::Push(const std::pair<unsigned long long, char > &crHash)
{
	std::unique_lock< mutex > lock(m_mutex);

	m_hashMap.insert(crHash);

	if(m_nextBlockIndex == crHash.first)
		m_condVar.notify_one();
}

bool HashQueue::WaitForNextHash()
{
	std::unique_lock< mutex > lock(m_mutex);

	while (m_hashMap.find(m_nextBlockIndex) == m_hashMap.end() && !m_bDone)
		m_condVar.wait(lock);

	if (m_hashMap.find(m_nextBlockIndex) == m_hashMap.end())
		return false;

	return true;
}

char HashQueue::Pop()
{
	std::unique_lock< mutex > lock(m_mutex);
	
	auto it = m_hashMap.find(m_nextBlockIndex);
	if (it == m_hashMap.end())
		return false;

	char hash = it->second;
	m_hashMap.erase(it);
	++m_nextBlockIndex;
	return hash;
}

void HashQueue::Stop()
{
	std::unique_lock< mutex > lock(m_mutex);

	m_bDone = true;
	m_condVar.notify_all();
}
