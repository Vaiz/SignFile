#pragma once

#include <memory>

class DataBlock
{
public:
	DataBlock();
	DataBlock(size_t nBlockSize);
	DataBlock(unsigned long long id,
		size_t nBlockSize);
	DataBlock(unsigned long long id,
		std::shared_ptr< char > pData,
		size_t nBlockSize);
	DataBlock(const DataBlock &);
	DataBlock &operator=(const DataBlock &);
	~DataBlock();

	//************************************
	// Access:		public 
	// Returns:		std::shared_ptr< char > - ���������� ����� ��������� �� ������
	//************************************
	std::shared_ptr< char > GetDataPtr();

	//************************************
	// Access:		public 
	// Returns:		unsigned long long - ���������� ���������� ����� �����
	//************************************
	unsigned long long GetId() const;

	//************************************
	// Access:		public
	// Parameter:	unsigned long long - id �����
	// Description:	������������� id �����
	//************************************
	void SetId(unsigned long long);

	//************************************
	// Access:    public 
	// Returns:   size_t - ���������� ������ ����� ������
	//************************************
	size_t GetBlockSize() const;

private:
	unsigned long long m_nId;			// ���������� ����� �����
	std::shared_ptr< char > m_pData;	// ����� ��������� �� ������ �����
	size_t m_nBlockSize;				// ������ ����� ������
};