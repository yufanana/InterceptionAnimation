#pragma once
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Target.h"

class Missile
{
private:
	double m_speed;
	double m_intercept;

public:
	static int SIM_FACTOR;
	bool m_sleep;
	double m_r;
	double m_theta;
	double m_launch;

private:
	void init();

public:
	Missile();
	virtual ~Missile();
	void update(int interval, int timeline);
	void setSleep();
	void activate();
	void setData(int i, int NTOTAL, const std::vector<std::vector<double>>& data);
	// pass data vector byRef to save memory
};

