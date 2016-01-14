#include "stdafx.h"
#include "FileREader.h"

#include <algorithm>

using namespace std;

FileReader::FileReader()
	: ifstream()
	, m_nNextBlockId(0)
	, m_bLastBlockEmpty(false)
{
}

FileReader::~FileReader()
{
}

FileReader &operator>>(FileReader &is, DataBlock &rDataBlock)
{
	if (is.eof())
		throw logic_error("Can't read block. Reached end of file.");

	if (!is.is_open())
		throw logic_error("Can't read block. File not open.");

	char *pData = rDataBlock.GetDataPtr().get();
	if (is.read(pData, rDataBlock.GetBlockSize()).eof())
	{
		auto bytesReaded = is.gcount();

		if (0 == bytesReaded)
			is.m_bLastBlockEmpty = true;
		else // Обнуляем конец блока
		{
			char *begin = pData + bytesReaded;
			char *end = pData + rDataBlock.GetBlockSize();
			std::fill(begin, end, 0);
		}
	}

	rDataBlock.SetId(is.m_nNextBlockId++);
	return is;
}

bool FileReader::IsLastBlockEmpty() const
{
	return m_bLastBlockEmpty;
}

unsigned long long FileReader::NextBolockId() const
{
	return m_nNextBlockId;
}