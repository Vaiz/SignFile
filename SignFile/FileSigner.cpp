#include "stdafx.h"
#include "FileSigner.h"

#include <thread>
#include <boost\crc.hpp>

#define MAX_ARRAY_SIZE	0x7fffffff		// ������������ ������ �������, ������� ����� �������� new
#define MEGABYTE		(1*1024*1024)	// �������� � ������
#define CRC8_POLYNOM	0x3a			// ������� ��� ������� crc

using namespace std;

FileSigner::FileSigner()
	: m_nThreads(2)
	, m_nBlocksPerThread(4)
	, m_nBlockSize(MEGABYTE)
{
}

FileSigner::~FileSigner()
{
}

void FileSigner::SetThreadCount(size_t nThreads)
{
	if (2 > nThreads)
		throw invalid_argument("Threads count can not be less then 2");

	m_nThreads = nThreads;
}

void FileSigner::SetBlockSize(size_t nBlockSize)
{
	if (nBlockSize > MAX_ARRAY_SIZE)
		throw invalid_argument("Block size can't be more then 0x7fffffff");
	if (0 == nBlockSize)
		throw invalid_argument("Block size can't be 0");

	m_nBlockSize = nBlockSize;
}

void FileSigner::SignFile(const char *pFileName,
	const char *pSignFileName)
{
	m_fileReader.open(pFileName, ios::binary | ios::in);
	if (!m_fileReader.is_open())
		throw logic_error(string("Can't open file ") + pFileName);

	m_signWriter.open(pSignFileName, ios::binary | ios::out);
	if(!m_signWriter.is_open())
		throw logic_error(string("Can't open file ") + pSignFileName);

	thread readThread = thread(ReadThread, this);
	
	// ����������� ������ �����������
	vector< thread > threads;
	threads.resize(m_nThreads - 1);
	for (thread &thr : threads)
		thr = thread(HashThread, this);

	thread writeThread(WriteThread, this);

	// ���� ���� �� ���������� ������ �����
	if (readThread.joinable())
		readThread.join();

	m_fileReader.close();
	m_dataQueue.Stop();

	// ��������� ��� ���� ����� �����������
	threads.push_back(thread(HashThread, this));

	// ������� ���������� ���� ������� �� �����������
	for (thread &thr : threads)
		if (thr.joinable())
			thr.join();

	// �������� ������� �����, ��� ������ ������ � ����� �� �����
	m_hashQueue.Stop();

	// ������ ���������� ������ ������
	if(writeThread.joinable())
		writeThread.join();

	m_signWriter.close();
}

void FileSigner::ReadThread(FileSigner *pFileSigner)
{
	size_t nMaxBlockCount = pFileSigner->m_nThreads * pFileSigner->m_nBlocksPerThread;

	while (!pFileSigner->m_fileReader.eof())
	{
		// ���� � ������� ���� �����, �� ������ ��� ���� ����
		size_t nQueueSize = pFileSigner->m_dataQueue.Size();
		if (nQueueSize < nMaxBlockCount)
		{
			shared_ptr< char > pData;
			try
			{
				pData.reset(new char[pFileSigner->m_nBlockSize], default_delete<char[]>());
			}
			catch (bad_alloc &e)
			{
				// �� ������� �������� ������ ���� ��� ������ ����
				if (pFileSigner->m_fileReader.NextBolockId() == 0)
					throw e;

				// ��������� ������������ ���������� ������, ����� ��������� ����������� ����� ����������
				nMaxBlockCount = nQueueSize;

				// ���� �� ��� ���� ���� ������ ��� ��������, ������� ����� �������� ��������� �� ������ �����
				if (0 == nMaxBlockCount)
					nMaxBlockCount = 1;

				continue;
			}

			DataBlock dataBlock(-1, pData, pFileSigner->m_nBlockSize);
			pFileSigner->m_fileReader >> dataBlock;
			
			if (pFileSigner->m_fileReader.eof())
				if (pFileSigner->m_fileReader.IsLastBlockEmpty())
					break;

			pFileSigner->m_dataQueue.Push(dataBlock);
		}
		else // ���� �� � ������� ��� ����, �� �������� ���� ����, ����� ����� �� ����������
		{
			DataBlock dataBlock;
			if (pFileSigner->m_dataQueue.Pop(dataBlock))
				pFileSigner->HashBlock(dataBlock);
		}
	}
}

void FileSigner::WriteThread(FileSigner *pFileSigner)
{
	while (pFileSigner->m_hashQueue.WaitForNextHash())
		pFileSigner->m_signWriter << pFileSigner->m_hashQueue.Pop();
}

void FileSigner::HashThread(FileSigner *pFileSigner)
{
	while (pFileSigner->m_dataQueue.WaitForNextBlock())
	{
		DataBlock dataBlock;
		if(pFileSigner->m_dataQueue.Pop(dataBlock))
			pFileSigner->HashBlock(dataBlock);
	}
}

void FileSigner::HashBlock(DataBlock &rDataBlock)
{
	boost::crc_basic<8> crc(CRC8_POLYNOM);
	crc.process_bytes(rDataBlock.GetDataPtr().get(), rDataBlock.GetBlockSize());

	char hashByte = crc.checksum();
	m_hashQueue.Push(make_pair(rDataBlock.GetId(), hashByte));
}
