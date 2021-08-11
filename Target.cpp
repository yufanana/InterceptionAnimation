#include "Target.h"

int Target::SIM_FACTOR;

Target::Target() {
	init();
}

Target::~Target() {

}

void Target::init() {
	m_red = 255;
	m_green = 255;
	m_blue = 0;
	m_sleep = false;
}

void Target::update(int interval, int timeline) {
	if (m_sleep == true) {
		return;
	}
	if (m_r <= m_intercept) {
		// reached the interception point
		m_sleep = true;
	}
	if (m_r <= m_rDetect) {
		// target detected
		m_green = 0; //change to yellow
	}

	m_r -= m_speed*interval;
}

void Target::setSleep() {
	m_sleep = true;
}

void Target::setData(int i, int NTOTAL, const std::vector<std::vector<double>>& data,std::string targetDetConfig) {
	// initialise the target parameters

	m_theta = (double)i * (360 / (NTOTAL));
	double TFR = data[i][1];
	double tSpeed = data[i][2];
	double detTime = data[i][3];
	double intRange = data[i][6];

	if (targetDetConfig == "A") {
		m_r = r0;
	}
	else {
		m_r = TFR / MAX_TFR * r0;
	}

	// MAX_TFR = r0 pixels (220km = 400 pixels)
	double flightTime = MAX_TFR * 1000 / tSpeed;

	// for SIM_FACTOR = 10, scale down 222s real flight time --> 2220ms simulated flight time
	// for SIM_FACTOR = 20, scale down 222s real flight time --> 4440ms simulated flight time
	m_speed = r0 / (flightTime * 1000 / SIM_FACTOR);	// units: pixels / ms

	if (detTime == 0) {
		m_rDetect = r0;
	}
	else
	{
		m_rDetect = TFR / MAX_TFR * r0;
	}
	m_intercept = intRange / MAX_TFR * r0;
}