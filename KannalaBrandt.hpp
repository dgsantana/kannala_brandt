#include "judgeKB.h"

class JudgeKB
{
public:
	JudgeKB();
	~JudgeKB();
	int setk(double k2, double k3, double k4, double k5);
	int setk(char* path);

private:
	int m_width = 0;
	int m_height = 0;
	double m_k2 = 0;
	double m_k3 = 0;
	double m_k4 = 0;
	double m_k5 = 0;
	double m_mu = 0;
	double m_mv = 0;
	double m_u0 = 0;
	double m_v0 = 0;

	double m_inv_K13=0;
	double m_inv_K22=0;
	double m_inv_K23=0;
	double m_inv_K11=0;


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
