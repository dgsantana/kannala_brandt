#include "judgeKB.h"
#include <iostream>

int main(int argc, char **argv)
{
	char *path = argv[1];

	// std::cout<<kbGetVersion()<<std::endl;

	switch (kbTestFile(path))
	{
	case KB_ER:
		std::cout << "unknow error\n";
		break;
	case KB_OK:
		std::cout << "params ok\n";
		break;
	case KB_CR:
		std::cout << "params out range error\n";
		break;
	case KB_CN:
		std::cout << "params not convergence error\n";
		break;
	case KB_TH:
		std::cout << "params theta error\n";
		break;
	default:
		break;
	}

	return 0;
}