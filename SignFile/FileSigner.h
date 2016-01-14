#pragma once

#include <map>			// map
#include <fstream>
#include <memory>		// unique_ptr
#include "FileReader.h"
#include "DataQueue.h"
#include "HashQueue.h"
#include "DataBlocksPool.h"

class FileSigner
{
public:
	FileSigner();
	~FileSigner();

	//************************************
	// Access:		public 
	// Parameter:	size_t nThreads - количество потоков
	// Description:	устанавливает количество потоков, используемое при хешировании
	// Exceptions:	invalid_argument
	//************************************
	void SetThreadCount(size_t);

	//************************************
	// Access:		public 
	// Parameter:	size_t nBlockSize - размер одного блока для хешировании
	// Description:	устанавливает размер блока для хеширования
	// Exceptions:	invalid_argument
	//************************************
	void SetBlockSize(size_t);

	//************************************
	// Access:		public 
	// Parameter:	const char * pFileName - путь к хешируемому файлу
	// Parameter:	const char * pSignFileName - путь к файлу сигнатуры
	// Description: генерирует сигнатуру указанного файла, и записывает ее в файл
	// Exceptions:	logic_error
	//************************************
	void SignFile(const char *pFileName, const char *pSignFileName);

private:

	//************************************
	// Access:		private static 
	// Parameter:	FileSigner * pFileSigner
	// Description:	поток чтения файла, при заполнении очереди хеширует один блок
	// Exceptions:	logic_error
	//************************************
	static void ReadThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private static
	// Parameter:	FileSigner * fileSigner
	// Description:	поток записи хеша в файл
	// Exceptions:	logic_error
	//************************************
	static void WriteThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private static
	// Parameter:	FileSigner * fileSigner
	// Description:	поток вычисления хеша
	// Exceptions:	logic_error
	//************************************
	static void HashThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private 
	// Parameter:	DataBlock & - блок, для кторого необходимо вычислить хеш
	// Description:	вычисляет хеш блока и добавляет его в m_hashQueue
	//************************************
	void HashBlock(std::shared_ptr< DataBlock >);

private:
	FileReader m_fileReader;	// поток чтения файла
	std::ofstream m_signWriter;		// поток записи подписи

	size_t m_nThreads;			// количество потоков
	size_t m_nBlocksPerThread;	// количество блоков в очереди на один поток, используется для расчета максимального размера очереди
	size_t m_nBlockSize;		// размер одного блока
	
	DataQueue m_dataQueue;	// очередь блоков на хеширование
	HashQueue m_hashQueue;	// очередь хешей на запись в файл
	std::unique_ptr< DataBlocksPool > m_pDataBlocksPool;
};

