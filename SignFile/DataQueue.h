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
	// Parameter:	const DataBlock & dataBlock - ������ �� ������, ������� ����� �������� � �������
	// Description:	��������� ���� � ������� � �������
	//************************************
	void Push(const DataBlock &dataBlock);

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
	// Returns:		bool - ���������� true, ��� ������� �������� ����
	// Parameter:	DataBlock & dataBlock - ������ �� ������, � ������� ����� ������� ���� �� �������
	// Description:	�������� ���� � ������� �� �������
	//************************************
	bool Pop(DataBlock &dataBlock);

	//************************************
	// Access:		public 
	// Description:	������������� ���� ��������� �������, ��� ������ ������ �� �����
	//************************************
	void Stop();

private:
	std::queue< DataBlock > m_queue;	// �������, �������� ����� � �������
	std::condition_variable m_condVar;	// �������� ����������, ��� �������� ��������� ������ �����
	std::mutex m_mutex;					// �������, ���������� �� ������ � ����� ������
	bool m_bDone;						// ����, ��������������� � ���, ��� ����� ������ �� �����
};
