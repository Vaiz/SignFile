#pragma once

#include <queue>				
#include <condition_variable>
#include <mutex>
#include <memory>	// shared_ptr
#include "DataBlock.h"

class DataQueue
{
public:
	DataQueue();
	~DataQueue();
		
	//************************************
	// Access:		public 
	// Parameter:	const std::shared_ptr< DataBlock > dataBlock - указатель на блок, 
	//				который будет добавлен в очередь
	// Description:	добавляет блок с данными в очередь
	//************************************
	void Push(const std::shared_ptr< DataBlock > dataBlock);

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
	// Returns:		std::shared_ptr< DataBlock > - указатель на следующий блок из очереди,
	//				если очередь пуста, то указатель будет нулевым
	//************************************
	std::shared_ptr< DataBlock > Pop();

	//************************************
	// Access:		public 
	// Description:	сигнализирует всем ожидающим потокам, что данных больше не будет
	//************************************
	void Stop();

private:
	std::queue< std::shared_ptr< DataBlock > > m_queue;	// очередь, хранящая блоки с данными
	std::condition_variable m_condVar;	// условная переменная, для ожидания появления нового блока
	std::mutex m_mutex;					// мьютекс, отвечающий за доступ к полям класса
	bool m_bDone;						// флаг, сигнализирующий о том, что новых блоков не будет
};
