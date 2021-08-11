#pragma once
#include "Target.h"
#include "Missile.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

class Swarm {
private:
	Target* m_pTargets;
	Missile* m_pMissiles;
	int lastTime;

public:
	static int SIM_FACTOR;
	static std::string targetDetConfig;
	static int NTOTAL;
	static int startTime;
	static int timeline;


public:
	Swarm(int elapsed);
	virtual ~Swarm();
	Target* getTargets() { return m_pTargets; };
	Missile* getMissiles() { return m_pMissiles; };
	void update(int elapsed);
	void setSleep(int i);
	void activateMissile(int i);
};