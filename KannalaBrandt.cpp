#include "KannalaBrandt.hpp"
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
JudgeKB::JudgeKB()
{
}

JudgeKB::~JudgeKB()
{
}

int JudgeKB::setk(double k2, double k3, double k4, double k5)
{
	m_k2 = k2;
	m_k3 = k3;
	m_k4 = k4;
	m_k5 = k5;
	return 0;
}

int JudgeKB::setk(char* path)
{
	std::ifstream inFile;
	std::cout << path << std::endl;
	inFile.open(path);
	if (!inFile.is_open())
	{
		return -1;
	}
	std::string line;
	char cline[512];
	std::string::size_type position;
	while (!inFile.eof())
	{
		inFile.getline(cline, 512);
		line = std::string(cline);
		
		position = line.find("image_width:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position+1, line.size() - position));
			ins >> m_width;
		}
		
		position = line.find("image_height:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position +1, line.size() - position));
			ins >> m_height;
		}
		
		position = line.find("k2:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_k2;
		}
		position = line.find("k3:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_k3;
		}

		position = line.find("k4:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_k4;
		}

		position = line.find("k5:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_k5;
		}

		position = line.find("mu:");
		if (position != line.npos)
		{
			std::cout << line << std::endl;
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_mu;
		}

		position = line.find("mv:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_mv;
		}

		position = line.find("u0:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_u0;
		}

		position = line.find("v0:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position+3, line.size() - position));
			ins >> m_v0;
		}



	}
	std::cout << m_width << "\t" << m_height << std::endl;
	std::cout << m_k2 << "\t" << m_k3 << "\t" << m_k4 << "\t" << m_k5 << std::endl;
	std::cout << m_mu <<"\t" <<m_mv << "\t" << m_u0 << "\t" << m_v0 << std::endl;
	jdConvergence();
	return 0;
}

void JudgeKB::setInv()
{

	m_inv_K11 = 1.0 / m_mu;
	m_inv_K13 = -m_u0 / m_mu;
	m_inv_K22 = 1.0 / m_mv;
	m_inv_K23 = -m_v0 / m_mv;

}


bool JudgeKB::jdThetaRegion()
{
	return true;
}

bool JudgeKB::jdCoffRegion()
{
	double k2 = abs(m_k2);
	double k3 = abs(m_k3);
	double k4 = abs(m_k4);
	double k5 = abs(m_k5);
	
	double min_k23 = 0;
	double max_k45 = 0;

	if (k4 < 0.1 && k5 < 0.1)
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}

bool JudgeKB::jdConvergence()
{
	{

		double px = 0, py = 0;
		double x = m_inv_K11 * px + m_inv_K13;
		double y = m_inv_K22 * py + m_inv_K23;
		double r_min = sqrt(x*x + y * y);
		for (double i = 0; i < 1.57; i += 0.01)
		{
			for (double j = i+0.01; j < 1.31; j += 0.01)
			{
				if (abs(fai(r_min,i)-fai(r_min,j))/abs(i-j)>=1)
				{
					std::cout << i << "\t" << j << std::endl;
					//return false;
				}
			}
		}
	}

	{
		double px = m_width, py = m_height;
		double x = m_inv_K11 * px + m_inv_K13;
		double y = m_inv_K22 * py + m_inv_K23;
		double r_max = sqrt(x*x + y * y);
	}
	return true;
}

bool JudgeKB::jdIteration()
{
	return true;
}


double JudgeKB::fai_min(double x)
{
	return 0;
}

double JudgeKB::fai_max(double x)
{
	return 0;

}

double JudgeKB::fai(double px, double py, double theta)
{

	double x = m_inv_K11 * px + m_inv_K13;
	double y = m_inv_K22 * py + m_inv_K23;
	double r = sqrt(x*x + y * y);

	double theta2 = theta * theta, theta3 = theta2 * theta, theta5 = theta3 * theta2, theta7 = theta5 * theta2, theta9 = theta7 * theta2;
	theta = r - (m_k2 * theta3 + m_k3 * theta5 + m_k4 * theta7 + m_k5 * theta9);
	return theta;
}

double JudgeKB::fai(double r, double theta)
{
	double theta2 = theta * theta, theta3 = theta2 * theta, theta5 = theta3 * theta2, theta7 = theta5 * theta2, theta9 = theta7 * theta2;
	theta = r - (m_k2 * theta3 + m_k3 * theta5 + m_k4 * theta7 + m_k5 * theta9);
	return theta;
}
