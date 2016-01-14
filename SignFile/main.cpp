#include "stdafx.h"

#include <iostream>
#include <thread>
#include "FileSigner.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3 && argc != 4)
	{
		cerr << "Usage: SignFile.exe file_name sign_file_name [block_size]" << endl;
		return 1;
	}

	FileSigner fileSigner;

	try
	{
		fileSigner.SetThreadCount(thread::hardware_concurrency());
	}
	catch (exception &e)
	{
		cerr << e.what() << endl;
	}

	try
	{
		if (argc == 4)
			fileSigner.SetBlockSize(atoi(argv[3]));

		cout << "Start time: ";
		system("time /t");

		fileSigner.SignFile(argv[1], argv[2]);
	}
	catch (exception &e)
	{
		cerr << e.what() << endl;
		return 2;
	}

	cout << "End time: ";
	system("time /t");
	return 0;
}

