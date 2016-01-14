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
	// Returns:		std::shared_ptr< char > - возвращает умный указатель на данные
	//************************************
	std::shared_ptr< char > GetDataPtr();

	//************************************
	// Access:		public 
	// Returns:		unsigned long long - возвращает порядковый номер блока
	//************************************
	unsigned long long GetId() const;

	//************************************
	// Access:		public
	// Parameter:	unsigned long long - id блока
	// Description:	устанавливает id блока
	//************************************
	void SetId(unsigned long long);

	//************************************
	// Access:    public 
	// Returns:   size_t - возвращает размер блока данных
	//************************************
	size_t GetBlockSize() const;

private:
	unsigned long long m_nId;			// порядковый номер блока
	std::shared_ptr< char > m_pData;	// умный указатель на данные блока
	size_t m_nBlockSize;				// размер блока данных
};