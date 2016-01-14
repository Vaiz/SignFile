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
	// Parameter:	const std::pair<unsigned long long, char > & - ������ �� ������, ������� ����� �������� � �������
	// Description:	��������� ��� � �������
	//************************************
	void Push(const std::pair<unsigned long long, char > &);

	//************************************
	// Access:		public 
	// Description:	������������� ���� ��������� �������, ��� ����� ������ �� �����
	//************************************
	void Stop();

	//************************************
	// Access:		public 
	// Returns:		bool - ���������� true, ���� � ������� ���� ��� � ������ id
	// Description:	������������� �����, ���� � ������� �� ��������  ��� � ������ id 
	//				��� �� ����� ������� ������� Stop
	//************************************
	bool WaitForNextHash();

	//************************************
	// Access:		public 
	// Returns:		char - ���������� ��� � ������ id �� �������
	// Description:	����� ������� ���� ������� ������� �������� ��� � ������� ����� ����� ������ ���,
	//				��� ����� ���������� ������������ ������� WaitForNextHash
	//************************************
	char Pop();

private:
	std::map< unsigned long long, char > m_hashMap;	// ������, ��� �������� ����� � ������������ � �� ��������
	unsigned long long m_nextBlockIndex;			// ����� ����, ������� ������ ���� ����� �������� Pop

	std::condition_variable m_condVar;				// �������� ����������, ��� �������� ��������� ������ ����				
	std::mutex m_mutex;								// �������, ���������� �� ������ � ����� ������
	bool m_bDone;									// ����, ��������������� � ���, ��� ����� ����� �� �����
};

