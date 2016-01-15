#include "stdafx.h"
#include "FileSigner.h"

#include <thread>
#include <boost\crc.hpp>

#define MEGABYTE		(1*1024*1024)	// мегабайт в байтах
#define CRC8_POLYNOM	0x3a			// полином для расчета crc

using namespace std;

FileSigner::FileSigner()
	: m_nThreads(2)
	, m_nBlocksPerThread(2)
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
		throw invalid_argument("Block size can't be more then 2147483647");
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

	m_pDataBlocksPool.reset(new DataBlocksPool(m_nBlockSize, m_nBlocksPerThread * m_nThreads));
	if(m_pDataBlocksPool->IsBadAllocReceived())
		throw logic_error("Can't allocate even single block");

	thread readThread = thread(ReadThread, this);
	
	// Запускаются потоки хеширования
	vector< thread > threads;
	threads.resize(m_nThreads - 1);
	for (thread &thr : threads)
		thr = thread(HashThread, this);

	thread writeThread(WriteThread, this);

	// Ждем пока не закончится чтение файла
	if (readThread.joinable())
		readThread.join();

	m_fileReader.close();
	m_dataQueue.Stop();

	// Добавляем еще один поток хеширования
	threads.push_back(thread(HashThread, this));

	// Ожидаем завершения всех потоков на хеширование
	for (thread &thr : threads)
		if (thr.joinable())
			thr.join();

	// Сообщаем очереди хешей, что больше блоков с хешем не будет
	m_hashQueue.Stop();

	// Ождаем завершения потока записи
	if(writeThread.joinable())
		writeThread.join();

	m_signWriter.close();
}

void FileSigner::ReadThread(FileSigner *pFileSigner)
{
	while (!pFileSigner->m_fileReader.eof())
	{
		bool bReadBlock = pFileSigner->m_pDataBlocksPool->HasFreeBlock()
			|| (!pFileSigner->m_pDataBlocksPool->IsBadAllocReceived() && !pFileSigner->m_pDataBlocksPool->IsMaxBlocksCountReached())
			|| pFileSigner->m_dataQueue.Size() == 0
			|| pFileSigner->m_pDataBlocksPool->BlocksCount() <= pFileSigner->m_nThreads;

		if(bReadBlock)
		{
			shared_ptr<	DataBlock > dataBlock = pFileSigner->m_pDataBlocksPool->Acquire();
			
			pFileSigner->m_fileReader >> *dataBlock;
			if (pFileSigner->m_fileReader.eof())
				if (pFileSigner->m_fileReader.IsLastBlockEmpty())
					break;

			pFileSigner->m_dataQueue.Push(dataBlock);
		}
		else // если же мы не можем прочитать блок, то хешируем один блок, чтобы поток не простаивал
		{
			shared_ptr<	DataBlock > dataBlock = pFileSigner->m_dataQueue.Pop();
			if (dataBlock)
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
		pFileSigner->HashBlock(pFileSigner->m_dataQueue.Pop());
}

void FileSigner::HashBlock(std::shared_ptr< DataBlock > pDataBlock)
{
	if (!pDataBlock)
		return;

	boost::crc_basic<8> crc(CRC8_POLYNOM);
	crc.process_bytes(pDataBlock->GetDataPtr().get(), pDataBlock->GetBlockSize());

	char hashByte = crc.checksum();
	m_hashQueue.Push(make_pair(pDataBlock->GetId(), hashByte));

	m_pDataBlocksPool->Release(pDataBlock);
}
