#pragma once

#include <queue>				
#include <condition_variable>
#include <mutex>
#include "DataBlock.h"

class DataQueue
{
public:
	DataQueue();
	~DataQueue();
		
	//************************************
	// Access:		public 
	// Parameter:	const DataBlock & dataBlock - ссылка на объект, который будет добавлен в очередь
	// Description:	добавляет блок с данными в очередь
	//************************************
	void Push(const DataBlock &dataBlock);

	//************************************
	// Access:		public 
	// Returns:		size_t - возвращает текущее количество блоков в очереди
	//************************************
	size_t Size() const;

	//************************************
	// Access:		public 
	// Returns:		bool - возвращает true, если в очереди есть хотя бы один блок
	// Description:	Останавливает поток, пока в очереди не появится хотя бы один блок, 
	//				или не будет вызвана функция Stop
	//************************************
	bool WaitForNextBlock();

	//************************************
	// Access:		public 
	// Returns:		bool - возвращает true, ели удалось получить блок
	// Parameter:	DataBlock & dataBlock - ссылка на объект, в который будет записан блок из очереди
	// Description:	забирает блок с данными из очереди
	//************************************
	bool Pop(DataBlock &dataBlock);

	//************************************
	// Access:		public 
	// Description:	сигнализирует всем ожидающим потокам, что данных больше не будет
	//************************************
	void Stop();

private:
	std::queue< DataBlock > m_queue;	// очередь, хранящая блоки с данными
	std::condition_variable m_condVar;	// условная переменная, для ожидания появления нового блока
	std::mutex m_mutex;					// мьютекс, отвечающий за доступ к полям класса
	bool m_bDone;						// флаг, сигнализирующий о том, что новых блоков не будет
};
