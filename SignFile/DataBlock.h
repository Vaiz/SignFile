#pragma once

#include <memory>

class DataBlock
{
public:
	//************************************
	// Access:		public
	// Description:	конструктор по умолчанию
	//************************************
	DataBlock();

	//************************************
	// Access:		public
	// Parameter:	size_t nBlockSize - размер блока с данными
	// Description:	выделяет память под блок указанного размера
	//************************************
	DataBlock(size_t nBlockSize);

	//************************************
	// Access:		public
	// Parameter:	unsigned long long - id блока
	// Parameter:	size_t nBlockSize - размер блока с данными
	// Description:	устанавливает id и выделяет память под блок указанного размера
	//************************************
	DataBlock(unsigned long long id,
		size_t nBlockSize);

	//************************************
	// Access:		public
	// Parameter:	unsigned long long - id блока
	// Parameter:	std::shared_ptr< char > pData - умный указатель на данные
	// Parameter:	size_t nBlockSize - размер блока с данными
	// Description:	устанавливает id и копирует указатель на блок с данными
	//************************************
	DataBlock(unsigned long long id,
		std::shared_ptr< char > pData,
		size_t nBlockSize);

	//************************************
	// Access:		public
	// Parameter:	const DataBlock & - ссылка на другой блок
	// Description:	конструктор копирования
	//************************************
	DataBlock(const DataBlock &);

	//************************************
	// Access:		public
	// Parameter:	const DataBlock & - ссылка на другой блок
	// Description:	оператор присваивания
	//************************************
	DataBlock &operator=(const DataBlock &);

	//************************************
	// Access:		public
	// Description:	деструктор
	//************************************
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