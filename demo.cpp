#include "judgeKB.h"
#include "KannalaBrandt.hpp"
#include <iostream>

int main()
{
	char* path = "D:/workspace/SLAM-OB/config/sunflower/cam0_cvmodule-sdm845.yaml";
	JudgeKB kb;
	kb.setk(path);
	char c;
	std::cin>>c;
	return 0;
}