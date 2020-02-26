#include "KannalaBrandt.hpp"
#include "MiniCircl.hpp"
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <eigen3/Eigen/Dense>

JudgeKB::JudgeKB()
{
	m_width = 0;
	m_height = 0;
	m_k2 = 0;
	m_k3 = 0;
	m_k4 = 0;
	m_k5 = 0;
	m_mu = 0;
	m_mv = 0;
	m_u0 = 0;
	m_v0 = 0;

	m_inv_K13 = 0;
	m_inv_K22 = 0;
	m_inv_K23 = 0;
	m_inv_K11 = 0;

	m_mask_radius = 500;
	for (size_t i = 0; i < 64; i++)
	{
		m_devices_radius[i] = 0;
	}
}

JudgeKB::~JudgeKB()
{
}

int JudgeKB::setParameters(int image_width,
						   int image_height,
						   double k2,
						   double k3,
						   double k4,
						   double k5,
						   double mu,
						   double mv,
						   double u0,
						   double v0)
{
	m_width = image_width;
	m_height = image_height;

	m_k2 = k2;
	m_k3 = k3;
	m_k4 = k4;
	m_k5 = k5;

	m_mu = mu;
	m_mv = mv;
	m_u0 = u0;
	m_v0 = v0;

	return setInv();
}

int JudgeKB::setParameters(const char *path)
{
	std::ifstream inFile;
	// std::cout << path << std::endl;
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
			std::istringstream ins(line.substr(position + 1, line.size() - position));
			ins >> m_width;
		}

		position = line.find("image_height:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position + 1, line.size() - position));
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
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_k3;
		}

		position = line.find("k4:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_k4;
		}

		position = line.find("k5:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_k5;
		}

		position = line.find("mu:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_mu;
		}

		position = line.find("mv:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_mv;
		}

		position = line.find("u0:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_u0;
		}

		position = line.find("v0:");
		if (position != line.npos)
		{
			std::istringstream ins(line.substr(position + 3, line.size() - position));
			ins >> m_v0;
		}
	}

	return setInv();
}

int JudgeKB::setInv()
{
	if (m_mu < 1.0e-3 || m_mv < 1.0e-3)
		return -1;

	m_inv_K11 = 1.0 / m_mu;
	m_inv_K13 = -m_u0 / m_mu;
	m_inv_K22 = 1.0 / m_mv;
	m_inv_K23 = -m_v0 / m_mv;

	return 0;
}

int JudgeKB::readConfig(const char *path)
{
	std::ifstream inFile;
	// std::cout << path << std::endl;
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

		position = line.find("cvmodule:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position + 1, line.size() - position));
			ins >> m_devices_radius[0];
		}

		position = line.find("cv2:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position + 1, line.size() - position));
			ins >> m_devices_radius[1];
		}

		position = line.find("G2:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position + 1, line.size() - position));
			ins >> m_devices_radius[2];
		}

		position = line.find("newG2:");
		if (position != line.npos)
		{
			position = line.find(":");
			std::istringstream ins(line.substr(position + 1, line.size() - position));
			ins >> m_devices_radius[3];
		}
	}

	return 0;
}

int JudgeKB::setDeviceType(int type)
{
	if (m_devices_radius[type] == 0)
	{
		return -1;
	}

	m_mask_radius = m_devices_radius[type];
	return 0;
}

int JudgeKB::setMaskRadius(double radius)
{
	m_mask_radius = radius;
	return 0;
}

int JudgeKB::saveMaskMap(const char *path)
{
	std::string s(path);
	char p[512];
	if (m_circle_radius == -1)
	{
		return -1;
	}

	if (path[s.length() - 1] == '/' || path[s.length() - 1] == '\\')
	{
		if (m_circle_radius == 0)
		{
			sprintf(p, "%smask_ok.png", path);
		}
		else
			sprintf(p, "%smask_%0.3lf.png", path, m_circle_radius);
	}
	else
	{
		if (m_circle_radius == 0)
		{
			sprintf(p, "%s/mask_ok.png", path);
		}
		else
			sprintf(p, "%s/mask_%0.3lf.png", path, m_circle_radius);
	}

#ifdef USE_OpenCV

	cv::imwrite(p, m_mask_map);
#else
	std::ofstream ofile;
	ofile.open(p);
	ofile.close();
#endif
}

bool JudgeKB::jdThetaRegion()
{
	return true;
}

bool JudgeKB::jdCoffRegion()
{
	m_circle_radius = -1;
	double k2 = std::abs(m_k2);
	double k3 = std::abs(m_k3);
	double k4 = std::abs(m_k4);
	double k5 = std::abs(m_k5);

	double min_k23 = 0;
	double max_k45 = 0;

	if (k4 > 0.2 || k5 > 0.2)
	{
		return false;
	}

	return true;
}

bool JudgeKB::jdConvergence()
{

	double px = 0, py = 0;
	double x = m_inv_K11 * px + m_inv_K13;
	double y = m_inv_K22 * py + m_inv_K23;
	double r_min = sqrt(x * x + y * y);
	double max_theta = 150 / 180 * 3.1415926 / 2;
	for (double i = 0; i < max_theta; i += 0.01)
	{
		for (double j = i + 0.01; j < max_theta; j += 0.01)
		{
			if (std::abs(fai(r_min, i) - fai(r_min, j)) / std::abs(i - j) >= 1)
			{
				// std::cout << i << "\t" << j << std::endl;
				return false;
			}
		}
	}

	return true;
}

double JudgeKB::backprojectSymmetric(double r)
{
	double tol = 1e-10;
	double p_u_norm = r;
	double theta;

	int npow = 9;
	if (m_k5 == 0.0)
	{
		npow -= 2;
	}
	if (m_k4 == 0.0)
	{
		npow -= 2;
	}
	if (m_k3 == 0.0)
	{
		npow -= 2;
	}
	if (m_k2 == 0.0)
	{
		npow -= 2;
	}

	Eigen::MatrixXd coeffs(npow + 1, 1);
	coeffs.setZero();
	coeffs(0) = -p_u_norm;
	coeffs(1) = 1.0;

	if (npow >= 3)
	{
		coeffs(3) = m_k2;
	}
	if (npow >= 5)
	{
		coeffs(5) = m_k3;
	}
	if (npow >= 7)
	{
		coeffs(7) = m_k4;
	}
	if (npow >= 9)
	{
		coeffs(9) = m_k5;
	}

	if (npow == 1)
	{
		theta = p_u_norm;
	}
	else
	{
		// Get eigenvalues of companion matrix corresponding to polynomial.
		// Eigenvalues correspond to roots of polynomial.
		Eigen::MatrixXd A(npow, npow);
		A.setZero();
		A.block(1, 0, npow - 1, npow - 1).setIdentity();
		A.col(npow - 1) = -coeffs.block(0, 0, npow, 1) / coeffs(npow);

		Eigen::EigenSolver<Eigen::MatrixXd> es(A);
		Eigen::MatrixXcd eigval = es.eigenvalues();

		std::vector<double> thetas;
		for (int i = 0; i < eigval.rows(); ++i)
		{
			if (fabs(eigval(i).imag()) > tol)
			{
				continue;
			}

			double t = eigval(i).real();

			if (t < -tol)
			{
				continue;
			}
			else if (t < 0.0)
			{
				t = 0.0;
			}

			thetas.push_back(t);
		}

		if (thetas.empty())
		{
			theta = p_u_norm;
			std::cout << "aaa\t";
		}
		else
		{
			theta = *std::min_element(thetas.begin(), thetas.end());
		}
	}
	return theta;
}
bool JudgeKB::jdIteration()
{
	m_circle_radius = -1;

#ifdef USE_OpenCV

	cv::Mat mask(m_height, m_width, CV_8UC1, cv::Scalar(255));
#endif
	MiniCircl cc;
	int count = 0;
	for (size_t px = 0; px < m_width; px++)
	{
		std::vector<int> flag(m_height, 1);
		for (size_t py = 0; py < m_height; py++)
		{
			double x = m_inv_K11 * px + m_inv_K13;
			double y = m_inv_K22 * py + m_inv_K23;
			double r = sqrt(x * x + y * y);
			double theta = r;
			int inter = 0;
			for (int j = 0; j < 20; j++)
			{
				double xk = theta;
				theta = fai(r, theta);
				double yk = theta;
				if (std::fabs(yk - xk) < 1e-10)
				{
					break;
				}

				theta = fai(r, theta);

				double zk = theta;
				if (std::fabs(yk - zk) < 1e-10)
				{
					break;
				}

				theta = xk - (yk - xk) * (yk - xk) / (zk - yk - yk + xk);
				if (std::fabs(zk - theta) < 1e-10)
				{
					break;
				}
				inter = j;
			}
			if (inter > 10 || theta > 1.57 || theta < 0)
			{
				;
#ifdef USE_OpenCV

				mask.at<uchar>(py, px) = 0;
#endif
			}
			else
			{
				flag[py] = 0;
			}
		}
		for (size_t i = 0; i < flag.size() - 1; i++)
		{
			if (flag[i] != flag[i + 1])
			{
				count++;
				cc.addPoint(px, i);
			}
		}
	}

#ifdef USE_OpenCV
	// cv::imwrite("mask.png", mask);
	m_mask_map = mask;
#endif

	if (count > 100)
	{
		double R = 0;
		if (cc.getRadius(R) == 1 && R != -1)
		{
			m_circle_radius = R;
			double x = 0, y = 0;
			cc.getCenter(x, y);
			if (x < m_u0 * 1.5 && x > m_u0 * 0.5)
			{
				if (y < m_v0 * 1.5 && y > m_v0 * 0.5)
				{
					if (R < m_mask_radius)
					{
						return false;
					}
				}
			}
		}
	}
	else
	{
		m_circle_radius = 0.0;
	}

	return true;
}

double JudgeKB::fai(double px, double py, double theta)
{
	double x = m_inv_K11 * px + m_inv_K13;
	double y = m_inv_K22 * py + m_inv_K23;
	double r = sqrt(x * x + y * y);

	return fai(r, theta);
}

double JudgeKB::fai(double r, double theta)
{
	double theta2 = theta * theta, theta3 = theta2 * theta, theta5 = theta3 * theta2, theta7 = theta5 * theta2, theta9 = theta7 * theta2;
	theta = r - (m_k2 * theta3 + m_k3 * theta5 + m_k4 * theta7 + m_k5 * theta9);
	return theta;
}
