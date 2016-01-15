#pragma once

#include <memory>		// shared_ptr				
#include <condition_variable>
#include <mutex>
#include <list>
#include "DataBlock.h"

class DataBlocksPool
{
public:
	//************************************
	// Access:		public
	// Parameter:	size_t nBlockSize - размер блока с данными
	// Parameter:	size_t nMaxBlocksCount - максимальное количество блоков
	// Description:	конструктор по умолчанию
	//************************************
	DataBlocksPool(size_t nBlockSize, size_t nMaxBlocksCount);

	//************************************
	// Access:		public
	// Description:	деструктор
	//************************************
	~DataBlocksPool();

	//************************************
	// Access:		public
	// Returns:		bool - возвращает true, если при выделении 
	//				памяти под блок возникло исключение bad_alloc
	//************************************
	bool IsBadAllocReceived() const;

	//************************************
	// Access:		public
	// Returns:		bool - возвращает true, если достигнуто максимальное количество блоков
	//************************************
	bool IsMaxBlocksCountReached() const;

	//************************************
	// Access:		public
	// Returns:		bool - возвращает true, если есть свободные блоки
	//************************************
	bool HasFreeBlock() const;

	//************************************
	// Access:		public
	// Returns:		size_t - возвращает количество созданных блоков
	//************************************
	size_t BlocksCount() const;

	//************************************
	// Access:		public
	// Returns:		std::shared_ptr< DataBlock > - возвращает указатель на свободный блок
	// Description:	Функция либо создаст новый блок, либо будет ожидать появления свободного блока
	//************************************
	std::shared_ptr< DataBlock > Acquire();

	//************************************
	// Access:		public
	// Parameter:	std::shared_ptr< DataBlock > - указатель на блок
	// Description:	Добавляет блок в список свободных
	//************************************
	void Release(std::shared_ptr< DataBlock > pDataBlock);

private:
	//************************************
	// Access:		public
	// Returns:		bool - возвращает true, если удалось выделить память под новый блок
	//************************************
	bool CreateBlock();

private:
	std::list< std::shared_ptr< DataBlock > > m_availableBlocks;	// свободные блоки
	size_t m_nBlockSize;		// размер блока
	size_t m_nBlocksCreated;	// количество созданных блоков
	size_t m_nMaxBlocksCount;	// Максимальное количество блоков, которое можно создать

	bool m_bBadAllocReceived;

	std::condition_variable m_condVar;	// условная переменная, для ожидания появления свободного блока
	std::mutex m_mutex;					// мьютекс, отвечающий за доступ к полям класса
};

