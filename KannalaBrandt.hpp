#include "judgeKB.h"

class JudgeKB
{
public:
	JudgeKB();
	~JudgeKB();
	int setk(double k2, double k3, double k4, double k5);
	int setk(char *path);

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

	bool jdThetaRegion();
	bool jdCoffRegion();
	bool jdConvergence();
	bool jdIteration();

	void setInv();
	double fai_min(double x);
	double fai_max(double x);
	double fai(double px, double py, double theta);
	double fai(double r, double theta);
};
