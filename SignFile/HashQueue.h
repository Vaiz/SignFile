#pragma once

#include <queue>				
#include <condition_variable>
#include <mutex>
#include <map>

class HashQueue
{
public:
	HashQueue();
	~HashQueue();

	//************************************
	// Access:		public 
	// Parameter:	const std::pair<unsigned long long, char > & - ссылка на объект, который будет добавлен в очередь
	// Description:	добавляет хеш в очередь
	//************************************
	void Push(const std::pair<unsigned long long, char > &);

	//************************************
	// Access:		public 
	// Description:	сигнализирует всем ожидающим потокам, что хешей больше не будет
	//************************************
	void Stop();

	//************************************
	// Access:		public 
	// Returns:		bool - возвращает true, если в очереди есть хеш с нужным id
	// Description:	Останавливает поток, пока в очереди не появится  хеш с нужным id 
	//				или не будет вызвана функция Stop
	//************************************
	bool WaitForNextHash();

	//************************************
	// Access:		public 
	// Returns:		char - возвращает хеш с нужным id из очереди
	// Description:	перед вызовом этой функции следует убедится что в очереди точно ессть нужный хеш,
	//				для этого необходимо использовать функцию WaitForNextHash
	//************************************
	char Pop();

private:
	std::map< unsigned long long, char > m_hashMap;	// дерево, для хранения хешей в соответствии с их номерами
	unsigned long long m_nextBlockIndex;			// номер хеша, который должен быть выдан функцией Pop

	std::condition_variable m_condVar;				// условная переменная, для ожидания появления нового хеша				
	std::mutex m_mutex;								// мьютекс, отвечающий за доступ к полям класса
	bool m_bDone;									// флаг, сигнализирующий о том, что новых хешей не будет
};

