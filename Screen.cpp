#include "Screen.h"

namespace yf {
	Screen::Screen() :
		m_window(NULL), m_renderer(NULL), m_texture(NULL), 
		m_buffer1(NULL), m_buffer2(NULL), m_message(NULL),m_rect(NULL),
		m_surface(NULL), m_textColor({ 0,0,0,0 }) {
	}

	int Screen::init() {
		// create window, renderer, texture, buffer memory
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "SDL could not initialise! SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		}
		m_window = SDL_CreateWindow("Interception Animation",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (m_window == NULL) {
			SDL_Quit();
			std::cout << "Could not create window: " << SDL_GetError() << std::endl;
			return 2;
		}
		
		m_renderer = SDL_CreateRenderer(m_window, -1,
			SDL_RENDERER_PRESENTVSYNC);
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
		SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

		if (m_renderer == NULL) {
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
			return 3;
		}

		if (m_texture == NULL) {
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			std::cout << "Could not create texture: " << SDL_GetError() << std::endl;
			return 4;
		}

		// buffer is used to store RGBA values of each pixel
		// each element corresponds to one pixel
		m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];

		// set all elements to 0
		memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

		if (TTF_Init() < 0) {
			std::cout << "TTF could not initialise! TTF Error: " << TTF_GetError() << std::endl;
			return 1;
		}

		m_textColor = {255, 255, 255, 0};
		
		return 0;
	}

	void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
		// set the RGBA value at the specified x-y position in the m_buffer1

		if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
			return;
		}

		Uint32 color = 0;

		color += red;
		color <<= 8;
		color += green;
		color <<= 8;
		color += blue;
		color <<= 8;
		color += alpha; //alpha

		m_buffer1[y * SCREEN_WIDTH + x] = color;
	}

	int Screen::update() {
		// update texture and renderer, draw number labels

		TTF_Font* font = TTF_OpenFont("calibrib.ttf", 20);
		if (font == NULL) {
			std::cout << "Failed to load font. SDL_ttf Error: " << TTF_GetError() << std::endl;
		}

		SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);

		for (int i = 0; i < Swarm::NTOTAL; i++) {

			std::string text = std::to_string(i+1);
			m_surface = TTF_RenderText_Solid(font, text.c_str(), m_textColor);
			m_message = SDL_CreateTextureFromSurface(m_renderer, m_surface);

			if (m_surface == NULL) {
				SDL_FreeSurface(m_surface);
				SDL_DestroyRenderer(m_renderer);
				SDL_DestroyWindow(m_window);
				SDL_Quit();
				std::cout << "Could not create surface: " << SDL_GetError() << std::endl;
				return 5;
			}

			if (m_message == NULL) {
				SDL_DestroyTexture(m_message);
				SDL_FreeSurface(m_surface);
				SDL_DestroyRenderer(m_renderer);
				SDL_DestroyWindow(m_window);
				SDL_Quit();
				std::cout << "Could not create texture: " << SDL_GetError() << std::endl;
				return 6;
			}

			double theta = (double)i * (360 / (Swarm::NTOTAL)) + 5;
			SDL_Rect m_rect;
			m_rect.x = 380 * cos(theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_X;
			m_rect.y = 380 * sin(theta / 180 * M_PI) + Screen::SCREEN_ORIGIN_Y;
			m_rect.w = 10 * text.length();
			m_rect.h = 20;


			SDL_FreeSurface(m_surface);
			SDL_RenderCopy(m_renderer, m_message, NULL, &m_rect);
			SDL_DestroyTexture(m_message);
		}
		
		SDL_RenderPresent(m_renderer);
		
		return 0;
	}

	void Screen::trailBlur(Target* pTargets, Missile* pMissiles) {
		// create a trail blur behind the targets and missiles by reducing the alpha values

		//Swap buffers, pixel info stored in m_buffer2
		Uint32* temp = m_buffer1; // to be used to draw
		m_buffer1 = m_buffer2;
		m_buffer2 = temp; // stores pixel values without blur

		for (int i = 0; i < Swarm::NTOTAL; i++) {
			Missile missile = pMissiles[i];
			Target target = pTargets[i];

			int Tx = target.m_r * cos(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_X;
			int Ty = target.m_r * sin(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_Y;
			int Mx = missile.m_r * cos(missile.m_theta / 180 * M_PI) + SCREEN_ORIGIN_X;
			int My = missile.m_r * sin(missile.m_theta / 180 * M_PI) + SCREEN_ORIGIN_Y;

			for (int frame = 1; frame < 10; frame++) {
				int TxBlur = (target.m_r + 2*frame) * cos(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_X;
				int TyBlur = (target.m_r + 2*frame) * sin(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_Y;
				int MxBlur = (missile.m_r - 2*frame) * cos(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_X;
				int MyBlur = (missile.m_r - 2*frame) * sin(target.m_theta / 180 * M_PI) + SCREEN_ORIGIN_Y;

				for (int j = 0; j <= 4; j++) {
					for (int k = 0; k <= 4; k++) {
						setPixel(TxBlur + j, TyBlur + k, target.m_red, target.m_green, target.m_blue, 255 - frame*25);
						if (missile.m_sleep == false) {
							setPixel(MxBlur + j, MyBlur + k, 0, 128, 255, 255 - frame * 25);
						}						
					}
				}
			}
		}
	}

	bool Screen::quitApplication() {
		// listens for user to click the exit button to exit the application
		
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return true;	// terminate game loop
			}
		}
		return false;
	}

	bool Screen::startAnimation() {
		// listens for user to press a button to start the animation

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_MOUSEBUTTONUP) {
				return true;	// exit pause loop
			}
		}
		return false;
	}

	void Screen::close() {
		// destroy stuff and free up memory resources

		delete[] m_buffer1;
		delete[] m_buffer2;
		SDL_DestroyTexture(m_texture);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	void Screen::clear() {
		memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	}

	void Screen::drawRangeCircles(int intervalKM) {
		// plots cirles at specified intervals (units: km)

		float intervalPixel = SCREEN_HEIGHT / 2 / Target::MAX_TFR * intervalKM;
		for (double i = intervalPixel; i <= 400; i += intervalPixel) {
			drawCircle(i);
		}
		
	}

	void Screen::drawCircle(int radius) {
		// plots a circle at with the specified radius (units: pixels)

		const double PI = M_PI;
		const double increment = (double)radius / 3000;
		for (double i = 0; i < 360; i += increment) {
			double x = (double)(radius * cos(i / 180 * PI));
			double y = (double)(radius * sin(i / 180 * PI));
			int x_pos = SCREEN_ORIGIN_X + x;
			int y_pos = SCREEN_ORIGIN_Y + y;
			setPixel(x_pos, y_pos, 255, 255, 255, 255);
		}
	}

	void Screen::drawShip(int size) {
		// draws the rectangular ship at the origin

		for (int i = -size; i <= size; i++) {
			for (int j = -size; j <= size; j += 2 * size) {
				int x = SCREEN_ORIGIN_X + j;
				int y = SCREEN_ORIGIN_Y + i;
				setPixel(x, y, 255, 255, 255, 255);
			}
		}

		for (int i = -size; i <= size; i++) {
			for (int j = -size; j <= size; j += 2 * size) {
				int x = SCREEN_ORIGIN_X + i;
				int y = SCREEN_ORIGIN_Y + j;
				setPixel(x, y, 255, 255, 255, 255);
			}
		}
	}

}