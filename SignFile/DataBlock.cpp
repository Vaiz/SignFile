#include "stdafx.h"
#include "DataBlock.h"

using namespace std;

DataBlock::DataBlock()
	: m_nId(-1)
	, m_nBlockSize(0)
{
}

DataBlock::DataBlock(size_t nBlockSize)
	: m_nId(-1)
	, m_pData(new char[nBlockSize], default_delete<char[]>())
	, m_nBlockSize(nBlockSize)
{
}

DataBlock::DataBlock(unsigned long long id,
	shared_ptr< char > pData,
	size_t nBlockSize)
	: m_nId(id)
	, m_pData(pData)
	, m_nBlockSize(nBlockSize)
{
}

DataBlock::DataBlock(unsigned long long id, size_t nBlockSize)
	: m_nId(id)
	, m_pData(new char[nBlockSize], default_delete<char[]>())
	, m_nBlockSize(nBlockSize)
{
}

DataBlock::DataBlock(const DataBlock &other)
	: m_nId(other.m_nId)
	, m_pData(other.m_pData)
	, m_nBlockSize(other.m_nBlockSize)
{
}

DataBlock &DataBlock::operator=(const DataBlock &other)
{
	m_nId = other.m_nId;
	m_pData = other.m_pData;
	m_nBlockSize = other.m_nBlockSize;

	return *this;
}

DataBlock::~DataBlock()
{
}

shared_ptr< char > DataBlock::GetDataPtr()
{
	return m_pData;
}

unsigned long long DataBlock::GetId() const
{
	return m_nId;
}

void DataBlock::SetId(unsigned long long nId)
{
	m_nId = nId;
}

size_t DataBlock::GetBlockSize() const
{
	return m_nBlockSize;
}
