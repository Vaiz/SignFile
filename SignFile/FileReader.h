#pragma once

#include <stdexcept>	// logic_error
#include <fstream>		// ifstream
#include <memory>		// shared_ptr

#include "DataBlock.h"

class FileReader : public std::ifstream
{
public:
	FileReader();
	~FileReader();

	//************************************
	// Returns:		friend FileReader & - ���������� ������ �� �����
	// Parameter:	FileReader & is - ������ �� �����
	// Parameter:	DataBlock & rDataBlock - ������ �� ���� � �������
	// Description:	������ ���� �� �����, ����������� ��� ������
	// Exceptions:	last_block_empty, logic_error
	//************************************
	friend FileReader &operator>>(FileReader &is, DataBlock &rDataBlock);

	//************************************
	// Access:    public 
	// Returns:   bool - ���������� true, ���� ��������� ����������� ���� ����� ����� 0
	//************************************
	bool IsLastBlockEmpty() const;

	//************************************
	// Access:    public 
	// Returns:   unsigned long long - ���������� id ���������� �����
	//************************************
	unsigned long long NextBolockId() const;

private:
	unsigned long long m_nNextBlockId;	// ������� ��� ���������� ������
	bool m_bLastBlockEmpty;				// ����, ����������� �� ��, ��� ��������� ����������� ���� ����� ������� �����
};

