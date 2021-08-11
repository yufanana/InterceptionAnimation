#pragma once
#include <SDL.h>
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Target.h"
#include "Missile.h"
#include "Swarm.h"

namespace yf {

	class Screen {
	public:
		const static int SCREEN_WIDTH = 800;
		const static int SCREEN_HEIGHT = 800;
		const static int SCREEN_ORIGIN_X = SCREEN_WIDTH/2;
		const static int SCREEN_ORIGIN_Y = SCREEN_HEIGHT/2;
	private:
		SDL_Window* m_window;
		SDL_Surface* m_surface;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		SDL_Texture* m_message;
		SDL_Color m_textColor;
		SDL_Rect* m_rect;
		Uint32* m_buffer1;
		Uint32* m_buffer2;

	public:
		Screen();
		int init();
		void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
		int update();
		void trailBlur(Target* pTargets, Missile* pMissiles);
		bool quitApplication();
		bool startAnimation();
		void close();
		void clear();
		void drawRangeCircles(int intervalKM);
		void drawCircle(int radius);
		void drawShip(int size);
	};

} /* namespace fyf */
