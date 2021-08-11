#include <SDL.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>
#include "Screen.h"
#include "Swarm.h"
#include "Target.h"
#include "Missile.h"

using namespace yf;

double Target::MAX_TFR = NULL;
int Swarm::SIM_FACTOR = NULL;
int Swarm::NTOTAL = NULL;
int circleInterval = NULL;
std::string Swarm::targetDetConfig = "";

int main(int argc, char* argv[]) {

	srand(time(NULL));
	Screen screen;

	do {
		std::cout << "Please enter the target detection configuration: ";
		std::cin >> Swarm::targetDetConfig;
	} while (Swarm::targetDetConfig != "A" && Swarm::targetDetConfig != "B");

	std::cout << "Please enter the range circle intervals (km): ";
	std::cin >> circleInterval;

	std::cout << "Please enter the Simulation Factor: ";
	std::cin >> Swarm::SIM_FACTOR;

	std::cout << std::endl;
	std::cout << "Please click on the window to begin" << std::endl;

	if (screen.init() != 0) {
		std::cout << "Error initialising SDL." << std::endl;
	}
	bool quit = false;

	while (true) {
		for (int x = 0; x < Screen::SCREEN_WIDTH; x++) {
			for (int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
				screen.setPixel(x, y, 0, 0, 0, 255);
			}
		}
		screen.drawShip(10);
		screen.drawRangeCircles(circleInterval);
		screen.update();

		if (screen.startAnimation() == true) {
			break;
		}

		if (screen.quitApplication() == true) {
			//std::cout << "here quit" << std::endl;
			screen.close();
			quit = true;
			break;
		}
		
	}

	int elapsed = SDL_GetTicks();
	Swarm swarm(elapsed);
	Swarm::startTime = elapsed;

	while (!quit) {

		elapsed = SDL_GetTicks();
		for (int x = 0; x < Screen::SCREEN_WIDTH; x++) {
			for (int y = 0; y < Screen::SCREEN_HEIGHT; y++) {
				screen.setPixel(x, y, 0, 0, 0, 255);
			}
		}
			
		Target* pTargets = swarm.getTargets();
		for (int i = 0; i < Swarm::NTOTAL; i++) {
			Target target = pTargets[i];

			int x = target.m_r * cos(target.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_X;
			int y = target.m_r * sin(target.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_Y;

			for (int j = 0; j <= 4; j++) {
				for (int k = 0; k <= 4; k++) {
					screen.setPixel(x+j, y+k, target.m_red, target.m_green, target.m_blue, 255);
				}
			}
		}

		Missile* pMissiles = swarm.getMissiles();
		for (int i = 0; i < Swarm::NTOTAL; i++) {
			Missile missile = pMissiles[i];

			if (Swarm::timeline >= missile.m_launch) {
				swarm.activateMissile(i);
			}

			if (missile.m_sleep) {
				continue;
			}
			//cout << target.m_theta << endl;
			int x = missile.m_r * cos(missile.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_X;
			int y = missile.m_r * sin(missile.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_Y;

			for (int j = 0; j <= 4; j++) {
				for (int k = 0; k <= 4; k++) {
					screen.setPixel(x + j, y + k, 0, 128, 255, 255);
				}
			}
		}

		// collision detection, activate and sleep
		/*for (int i = 0; i < Swarm::NTOTAL; i++) {
			Missile missile = pMissiles[i];
			Target target = pTargets[i];
			int Mx = missile.m_r * cos(missile.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_X;
			int My = missile.m_r * sin(missile.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_Y;
			int Tx = target.m_r * cos(target.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_X;
			int Ty = target.m_r * sin(target.m_theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_Y;
			
			if (abs(Tx - Mx) < 5 && abs(Ty - My) < 5) {
				// collision detected
				swarm.setSleep(i);
			}
		}*/

	
		swarm.update(elapsed);
		screen.trailBlur(pTargets,pMissiles);
		screen.drawShip(10);
		screen.drawRangeCircles(circleInterval);
		screen.update();

		if (screen.quitApplication() == true) {
			screen.close();
			break;
		}
	}

	return 0;
}