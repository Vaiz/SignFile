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
	// Parameter:	size_t nBlockSize - ������ ����� � �������
	// Parameter:	size_t nMaxBlocksCount - ������������ ���������� ������
	// Description:	����������� �� ���������
	//************************************
	DataBlocksPool(size_t nBlockSize, size_t nMaxBlocksCount);

	//************************************
	// Access:		public
	// Description:	����������
	//************************************
	~DataBlocksPool();

	//************************************
	// Access:		public
	// Returns:		bool - ���������� true, ���� ��� ��������� 
	//				������ ��� ���� �������� ���������� bad_alloc
	//************************************
	bool IsBadAllocReceived() const;

	//************************************
	// Access:		public
	// Returns:		bool - ���������� true, ���� ���������� ������������ ���������� ������
	//************************************
	bool IsMaxBlocksCountReached() const;

	//************************************
	// Access:		public
	// Returns:		bool - ���������� true, ���� ���� ��������� �����
	//************************************
	bool HasFreeBlock() const;

	//************************************
	// Access:		public
	// Returns:		size_t - ���������� ���������� ��������� ������
	//************************************
	size_t BlocksCount() const;

	//************************************
	// Access:		public
	// Returns:		std::shared_ptr< DataBlock > - ���������� ��������� �� ��������� ����
	// Description:	������� ���� ������� ����� ����, ���� ����� ������� ��������� ���������� �����
	//************************************
	std::shared_ptr< DataBlock > Acquire();

	//************************************
	// Access:		public
	// Parameter:	std::shared_ptr< DataBlock > - ��������� �� ����
	// Description:	��������� ���� � ������ ���������
	//************************************
	void Release(std::shared_ptr< DataBlock > pDataBlock);

private:
	//************************************
	// Access:		public
	// Returns:		bool - ���������� true, ���� ������� �������� ������ ��� ����� ����
	//************************************
	bool CreateBlock();

private:
	std::list< std::shared_ptr< DataBlock > > m_availableBlocks;	// ��������� �����
	size_t m_nBlockSize;		// ������ �����
	size_t m_nBlocksCreated;	// ���������� ��������� ������
	size_t m_nMaxBlocksCount;	// ������������ ���������� ������, ������� ����� �������

	bool m_bBadAllocReceived;

	std::condition_variable m_condVar;	// �������� ����������, ��� �������� ��������� ���������� �����
	std::mutex m_mutex;					// �������, ���������� �� ������ � ����� ������
};

