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
	// Returns:		friend FileReader & - возвращает ссылку на поток
	// Parameter:	FileReader & is - ссылка на поток
	// Parameter:	DataBlock & rDataBlock - ссылка на блок с данными
	// Description:	читает блок из файла, присваивает ему индекс
	// Exceptions:	last_block_empty, logic_error
	//************************************
	friend FileReader &operator>>(FileReader &is, DataBlock &rDataBlock);

	//************************************
	// Access:    public 
	// Returns:   bool - возвращает true, если последний прочитанный блок имеет длину 0
	//************************************
	bool IsLastBlockEmpty() const;

	//************************************
	// Access:    public 
	// Returns:   unsigned long long - возвращает id следующего блока
	//************************************
	unsigned long long NextBolockId() const;

private:
	unsigned long long m_nNextBlockId;	// счетчик для индексации блоков
	bool m_bLastBlockEmpty;				// флаг, указывающий на то, что последний прочитанный блок имеет нулевую длину
};

