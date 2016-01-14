#include "stdafx.h"
#include "FileSigner.h"

#include <thread>
#include <boost\crc.hpp>

#include "FileReader.h"

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
	thread readThread = thread(ReadThread, this, pFileName);
	
	// ����������� ������ �����������
	vector< thread > threads;
	threads.resize(m_nThreads - 1);
	for (thread &thr : threads)
		thr = thread(HashThread, this);

	thread writeThread(WriteThread, this, pSignFileName);

	// ���� ���� �� ���������� ������ �����
	if (readThread.joinable())
		readThread.join();

	// ��������� ��� ��� ����� �����������
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
}

void FileSigner::ReadThread(FileSigner *pFileSigner, const char *pFileName)
{
	FileReader fileReader;
	fileReader.open(pFileName, ios::binary | ios::in);

	if (!fileReader.is_open())
		throw logic_error(string("Can't open file ") + pFileName);

	size_t nMaxBlockCount = pFileSigner->m_nThreads * pFileSigner->m_nBlocksPerThread;

	while (!fileReader.eof())
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
				if (fileReader.NextBolockId() == 0)
					throw e;

				// ��������� ������������ ���������� ������, ����� ��������� ����������� ����� ����������
				nMaxBlockCount = nQueueSize;

				// ���� �� ��� ���� ���� ������ ��� ��������, ������� ����� �������� ��������� �� ������ �����
				if (0 == nMaxBlockCount)
					nMaxBlockCount = 1;

				continue;
			}

			DataBlock dataBlock(-1, pData, pFileSigner->m_nBlockSize);
			fileReader >> dataBlock;
			
			if (fileReader.eof())
				if (fileReader.IsLastBlockEmpty())
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
	fileReader.close();
	pFileSigner->m_dataQueue.Stop();
}

void FileSigner::WriteThread(FileSigner *pFileSigner, const char *pFileName)
{
	ofstream signFile(pFileName, ios::binary | ios::out);

	if (!signFile.is_open())
		throw logic_error(string("Can't open file ") + pFileName);

	while (pFileSigner->m_hashQueue.WaitForNextHash())
		signFile << pFileSigner->m_hashQueue.Pop();
	
	signFile.close();
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