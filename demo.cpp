#include "judgeKB.h"
#include <iostream>

int main(int argc, char **argv)
{
	char *param_file = argv[1];

	std::cout << kbGetVersion() << std::endl;

	if (kbSetThetaRadius(300) < 0)
	{
		std::cout << "radius error\n";
		return 0;
	}

	switch (kbTestFile(param_file))
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

	if (kbGetThetaMap("./") < 0)
	{
		std::cout << "can not get map\n";
	}
	return 0;
}