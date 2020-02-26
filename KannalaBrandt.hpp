#include "judgeKB.h"
#include <exception>
#ifdef USE_OpenCV
#include <opencv2/opencv.hpp>
#endif
class JudgeKB
{
public:
	JudgeKB();
	~JudgeKB();
	int setParameters(int image_width,
					  int image_height,
					  double k2,
					  double k3,
					  double k4,
					  double k5,
					  double mu,
					  double mv,
					  double u0,
					  double v0);
	int setParameters(const char *path);
	int setMaskRadius(double radius);
	int saveMaskMap(const char *path);

	bool jdThetaRegion();
	bool jdCoffRegion();
	bool jdConvergence();
	bool jdIteration();

private:
	int m_width;
	int m_height;
	double m_k2;
	double m_k3;
	double m_k4;
	double m_k5;
	double m_mu;
	double m_mv;
	double m_u0;
	double m_v0;

	double m_inv_K13;
	double m_inv_K22;
	double m_inv_K23;
	double m_inv_K11;

	double m_mask_radius;
	double m_circle_radius;
	double m_devices_radius[64];
	int m_device_type;
#ifdef USE_OpenCV
	cv::Mat m_mask_map;
#endif
	int setInv();
	double fai(double px, double py, double theta);
	double fai(double r, double theta);
	double backprojectSymmetric(double r);

	int readConfig(const char *path);
	int setDeviceType(int type);
};

JudgeKB kb;
const char *kbGetVersion()
{
	return "V0.0.2-20200226111244";
}

int kbSetThetaRadius(double radius)
{
	return kb.setMaskRadius(radius);
}

KB_STATUS testKB()
{
	try
	{
		if (kb.jdCoffRegion() == false)
		{
			return KB_CR;
		}
		if (kb.jdConvergence() == false)
		{
			return KB_CN;
		}
		if (kb.jdIteration() == false)
		{
			return KB_TH;
		}
	}
	catch (const std::exception &e)
	{
		return KB_ER;
	}
	return KB_OK;
}

KB_STATUS kbTestParameters(int image_width,
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
	if (kb.setParameters(image_width, image_height,
						 k2, k3, k4, k5,
						 mu, mv, u0, v0) == 0)
	{
		return testKB();
	}
	return KB_ER;
}

KB_STATUS kbTestFile(const char *file_path)
{
	if (kb.setParameters(file_path) == 0)
	{
		return testKB();
	}
	return KB_ER;
}

int kbGetThetaMap(const char *save_path)
{
	return kb.saveMaskMap(save_path);
}
