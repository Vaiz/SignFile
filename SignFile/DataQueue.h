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
	// Parameter:	const std::shared_ptr< DataBlock > dataBlock - ��������� �� ����, 
	//				������� ����� �������� � �������
	// Description:	��������� ���� � ������� � �������
	//************************************
	void Push(const std::shared_ptr< DataBlock > dataBlock);

	//************************************
	// Access:		public 
	// Returns:		size_t - ���������� ������� ���������� ������ � �������
	//************************************
	size_t Size() const;

	//************************************
	// Access:		public 
	// Returns:		bool - ���������� true, ���� � ������� ���� ���� �� ���� ����
	// Description:	������������� �����, ���� � ������� �� �������� ���� �� ���� ����, 
	//				��� �� ����� ������� ������� Stop
	//************************************
	bool WaitForNextBlock();

	//************************************
	// Access:		public 
	// Returns:		std::shared_ptr< DataBlock > - ��������� �� ��������� ���� �� �������,
	//				���� ������� �����, �� ��������� ����� �������
	//************************************
	std::shared_ptr< DataBlock > Pop();

	//************************************
	// Access:		public 
	// Description:	������������� ���� ��������� �������, ��� ������ ������ �� �����
	//************************************
	void Stop();

private:
	std::queue< std::shared_ptr< DataBlock > > m_queue;	// �������, �������� ����� � �������
	std::condition_variable m_condVar;	// �������� ����������, ��� �������� ��������� ������ �����
	std::mutex m_mutex;					// �������, ���������� �� ������ � ����� ������
	bool m_bDone;						// ����, ��������������� � ���, ��� ����� ������ �� �����
};
