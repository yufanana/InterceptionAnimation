#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>

class Target
{
private:
	double m_TFR;
	double m_speed;
	double m_detTime;
	double m_lTime;
	double m_intTime;
	double m_intRange;

	double m_rDetect;

public:
	static int SIM_FACTOR;
	static double MAX_TFR;
	const static int r0 = 400;

	double m_intercept;
	bool m_sleep;
	double m_r;
	double m_theta;
	int m_red;
	int m_green;
	int m_blue;

private:
	void init();

public:
	Target();
	virtual ~Target();
	void update(int interval, int timeline);
	void setSleep();
	void setData(int i, int NTOTAL, const std::vector<std::vector<double>>& data, std::string targetDetConfig);
	// pass data vector byRef to save memory
};