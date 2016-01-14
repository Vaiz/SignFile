#pragma once

#include <map>			// map
#include "DataQueue.h"
#include "HashQueue.h"

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
	// Parameter:	const char * pFileName - путь к файлу для хеширования
	// Description:	поток чтения файла, при заполнении очереди хеширует один блок
	// Exceptions:	logic_error
	//************************************
	static void ReadThread(FileSigner *pFileSigner, const char *pFileName);

	//************************************
	// Access:		private static
	// Parameter:	FileSigner * fileSigner
	// Parameter:	const char * signFileName - путь к файлу, в который будет записана сигнатура
	// Description:	поток записи хеша в файл
	// Exceptions:	logic_error
	//************************************
	static void WriteThread(FileSigner *pFileSigner, const char *pSignFileName);

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
	void HashBlock(DataBlock &);

private:
	size_t m_nThreads;			// количество потоков
	size_t m_nBlocksPerThread;	// количество блоков в очереди на один поток, используется для расчета максимального размера очереди
	size_t m_nBlockSize;		// размер одного блока
	
	DataQueue m_dataQueue;	// очередь блоков на хеширование
	HashQueue m_hashQueue;	// очередь хешей на запись в файл
};

