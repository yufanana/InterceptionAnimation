#include "Swarm.h"

int Swarm::startTime;
int Swarm::timeline;

Swarm::Swarm(int elapsed): lastTime(elapsed) {

	// read from file
	std::ifstream myFile;
	std::string line;
	std::vector<std::vector<double>> data;   // vector containing NTOTAL vectors
	std::vector<double> targetData;
	myFile.open("data.csv");
	int count = 0;

	getline(myFile, line);	               // skip header line
	while (getline(myFile, line)) {		
		// each time the while condition is reached, the next line is read
		// cout << line << endl;
		std::stringstream ss(line);        // convert the line into a stream
		std::string value;	    	       // helper variable to store each value
		targetData.clear();			       // vector to store one target's data

		while (getline(ss, value, ',')) {
			// syntax: getline(<stream>, <string var>, <delimiter>)
			// cout << value << endl;
			double temp = stod(value);      // convert string var to double var
			targetData.push_back(temp);     // add this value to the back of the vector
		}
		data.push_back(targetData);         // add this target vector into overall data vector
		count++;
	}
	NTOTAL = count;

	// create new arrays to contain Target and Missile objects
	m_pTargets = new Target[NTOTAL];
	m_pMissiles = new Missile[NTOTAL];
	Target::SIM_FACTOR = SIM_FACTOR;
	Missile::SIM_FACTOR = SIM_FACTOR;

	double MAX_TFR = 0;
	for (int i = 0; i < NTOTAL; i++) {
		double TFR = data[i][1];
		if (TFR > MAX_TFR) {
			MAX_TFR = TFR;
		}
	}
	Target::MAX_TFR = MAX_TFR;

	for (int i = 0; i < NTOTAL; i++) {
		Target target = m_pTargets[i];
		m_pTargets[i].setData(i, NTOTAL, data, targetDetConfig);
	}

	for (int i = 0; i < NTOTAL; i++) {
		Missile missile = m_pMissiles[i];
		m_pMissiles[i].setData(i, NTOTAL, data);
	}

	myFile.close();
}

Swarm::~Swarm() {
	// delete and free up resources

	delete[] m_pTargets;
	delete[] m_pMissiles;
}

void Swarm::update(int elapsed) {
	// update the position of the targets and missiles

	int interval = elapsed - lastTime;
	lastTime = elapsed;
	timeline = elapsed - startTime;
	for (int i = 0; i < NTOTAL; i++) {
		m_pTargets[i].update(interval, timeline);
		m_pMissiles[i].update(interval, timeline);
	}
}

void Swarm::setSleep(int i) {
	// stops updating the specified target and missile
	m_pTargets[i].setSleep();
	m_pMissiles[i].setSleep();
}

void Swarm::activateMissile(int i) {
	// starts updating the specified missile and drawing it
	m_pMissiles[i].activate();
}

