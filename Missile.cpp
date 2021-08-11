#include "Missile.h"

int Missile::SIM_FACTOR;

Missile::Missile() {
	init();
}

Missile::~Missile() {

}

void Missile::init() {
	m_sleep = true;
	m_speed = 0.04;
	m_r = 0.0;
}

void Missile::update(int interval, int timeline) {
	if (m_sleep == true) {
		return;
	}
	if (m_r >= m_intercept) {
		// reached the inteception point
		m_sleep = true;
	}
	m_r += m_speed * interval;
}

void Missile::setSleep() {
	m_sleep = true;
}

void Missile::activate() {
	m_sleep = false;
}

void Missile::setData(int i, int NTOTAL, const std::vector<std::vector<double>> &data) {
	// initialise the missile parameters

	m_theta = (double)i * (360 / (NTOTAL));
	double launchTime = data[i][4];
	double intTime = data[i][5];
	double intRange = data[i][6];
	double flightTime = intTime - launchTime;

	m_launch = launchTime * 1000 / SIM_FACTOR;
	m_intercept = intRange / Target::MAX_TFR * Target::r0;
	m_speed = m_intercept / (flightTime * 1000 / SIM_FACTOR);
}