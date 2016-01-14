#pragma once

#include <map>			// map
#include <fstream>
#include <memory>		// unique_ptr
#include "FileReader.h"
#include "DataQueue.h"
#include "HashQueue.h"
#include "DataBlocksPool.h"

class FileSigner
{
public:
	FileSigner();
	~FileSigner();

	//************************************
	// Access:		public 
	// Parameter:	size_t nThreads - ���������� �������
	// Description:	������������� ���������� �������, ������������ ��� �����������
	// Exceptions:	invalid_argument
	//************************************
	void SetThreadCount(size_t);

	//************************************
	// Access:		public 
	// Parameter:	size_t nBlockSize - ������ ������ ����� ��� �����������
	// Description:	������������� ������ ����� ��� �����������
	// Exceptions:	invalid_argument
	//************************************
	void SetBlockSize(size_t);

	//************************************
	// Access:		public 
	// Parameter:	const char * pFileName - ���� � ����������� �����
	// Parameter:	const char * pSignFileName - ���� � ����� ���������
	// Description: ���������� ��������� ���������� �����, � ���������� �� � ����
	// Exceptions:	logic_error
	//************************************
	void SignFile(const char *pFileName, const char *pSignFileName);

private:

	//************************************
	// Access:		private static 
	// Parameter:	FileSigner * pFileSigner
	// Description:	����� ������ �����, ��� ���������� ������� �������� ���� ����
	// Exceptions:	logic_error
	//************************************
	static void ReadThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private static
	// Parameter:	FileSigner * fileSigner
	// Description:	����� ������ ���� � ����
	// Exceptions:	logic_error
	//************************************
	static void WriteThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private static
	// Parameter:	FileSigner * fileSigner
	// Description:	����� ���������� ����
	// Exceptions:	logic_error
	//************************************
	static void HashThread(FileSigner *pFileSigner);

	//************************************
	// Access:		private 
	// Parameter:	DataBlock & - ����, ��� ������� ���������� ��������� ���
	// Description:	��������� ��� ����� � ��������� ��� � m_hashQueue
	//************************************
	void HashBlock(std::shared_ptr< DataBlock >);

private:
	FileReader m_fileReader;	// ����� ������ �����
	std::ofstream m_signWriter;		// ����� ������ �������

	size_t m_nThreads;			// ���������� �������
	size_t m_nBlocksPerThread;	// ���������� ������ � ������� �� ���� �����, ������������ ��� ������� ������������� ������� �������
	size_t m_nBlockSize;		// ������ ������ �����
	
	DataQueue m_dataQueue;	// ������� ������ �� �����������
	HashQueue m_hashQueue;	// ������� ����� �� ������ � ����
	std::unique_ptr< DataBlocksPool > m_pDataBlocksPool;
};

