#include <SDL3/SDL.h>

int main(void)
{
	SDL_Log("hello sdl");
	
	float window_w = 800;
	float window_h = 600;
	int target_framerate_ms = 1000 / 60;       // 16 milliseconds
	int target_framerate_ns = 1000000000 / 60; // 16666666 nanoseconds

	SDL_Window* window = SDL_CreateWindow("E00 - introduction", window_w, window_h, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	// increase the zoom to make debug text more legible
	// (ie, on the class projector, we will usually use 2)
	{
		float zoom = 1;
		window_w /= zoom;
		window_h /= zoom;
		SDL_SetRenderScale(renderer, zoom, zoom);
	}

	bool quit = false;

	SDL_Time walltime_frame_beg;
	SDL_Time walltime_work_end;
	SDL_Time walltime_frame_end = 0;
	SDL_Time time_elapsed_frame = 100000000;
	SDL_Time time_elapsed_work;

	int delay_type = 0;

	float player_size = 40;
	SDL_FRect player1_rect;
	player1_rect.w = player_size;
	player1_rect.h = player_size;
	player1_rect.x = window_w / 2 - player_size / 2;
	player1_rect.y = window_h / 2 - player_size / 2;

	SDL_FRect player2_rect;
	player2_rect.w = player_size;
	player2_rect.h = player_size;
	player2_rect.x = window_w / 2 - player_size / 2;
	player2_rect.y = window_h / 2 - player_size / 2;

	bool btn_pressed_w = false;
	bool btn_pressed_a = false;
	bool btn_pressed_d = false;
	bool btn_pressed_s = false;

	bool btn_pressed_left = false;
	bool btn_pressed_right = false;
	bool btn_pressed_up = false;
	bool btn_pressed_down = false;

	SDL_GetCurrentTime(&walltime_frame_beg);
	while(!quit)
	{
		bool mouse_down = false;

		// input
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_EVENT_QUIT:
					quit = true;
					break;
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
					mouse_down = true;
					break;
				case SDL_EVENT_KEY_DOWN:
					if(event.key.key >= SDLK_0 && event.key.key < SDLK_5)
						delay_type = event.key.key - SDLK_0;
					switch(event.key.key) {
						case SDLK_A:
							btn_pressed_a = true;
							break;
						case SDLK_W:
							btn_pressed_w = true;
							break;
						case SDLK_S:
							btn_pressed_s = true;
							break;
						case SDLK_D:
							btn_pressed_d = true;
							break;
						case SDLK_UP:
							btn_pressed_up = true;
							break;
						case SDLK_LEFT:
							btn_pressed_left = true;
							break;
						case SDLK_RIGHT:
							btn_pressed_right = true;
							break;
						case SDLK_DOWN:
							btn_pressed_down = true;
							break;
					}
					break;
				case SDL_EVENT_KEY_UP:
					switch(event.key.key) {
						case SDLK_A:
							btn_pressed_a = false;
							break;
						case SDLK_W:
							btn_pressed_w = false;
							break;
						case SDLK_S:
							btn_pressed_s = false;
							break;
						case SDLK_D:
							btn_pressed_d = false;
							break;
						case SDLK_UP:
							btn_pressed_up = false;
							break;
						case SDLK_LEFT:
							btn_pressed_left = false;
							break;
						case SDLK_RIGHT:
							btn_pressed_right = false;
							break;
						case SDLK_DOWN:
							btn_pressed_down = false;
							break;
					}
					break;
			}
		}

		float vx1 = 0;
		float vy1 = 0;
		float vx2 = 0;
		float vy2 = 0;
		float speed = time_elapsed_frame * 0.000001;

		if (btn_pressed_a)
			vx1 = -speed;
		if (btn_pressed_d)
			vx1 = speed;
		if (btn_pressed_w)
			vy1 = -speed;
		if (btn_pressed_s)
			vy1 = speed;

		if (btn_pressed_left)
			vx2 = -speed;
		if (btn_pressed_right)
			vx2 = speed;
		if (btn_pressed_up)
			vy2 = -speed;
		if (btn_pressed_down)
			vy2 = speed;

		player1_rect.x += vx1;
		player1_rect.y += vy1;

		player2_rect.x += vx2;
		player2_rect.y += vy2;

		if (player1_rect.x < 0)
			player1_rect.x = 0;
		
		if (player1_rect.x + player1_rect.w > window_w)
			player1_rect.x = window_w - player1_rect.w;
			
		if (player1_rect.y < 0)
			player1_rect.y = 0;
		
		if (player1_rect.y + player1_rect.h > window_h)
			player1_rect.y = window_h - player1_rect.h;

		if (player2_rect.x < 0)
			player2_rect.x = 0;
		
		if (player2_rect.x + player2_rect.w > window_w)
			player2_rect.x = window_w - player2_rect.w;
			
		if (player2_rect.y < 0)
			player2_rect.y = 0;
		
		if (player2_rect.y + player2_rect.h > window_h)
			player2_rect.y = window_h - player2_rect.h;

		float x, y;
		SDL_GetMouseState(&x, &y);

		if (mouse_down) {
			player1_rect.x = x - player1_rect.w / 2;
			player1_rect.y = y - player1_rect.h / 2;
		}
		
		// clear screen
		// NOTE: `0x` prefix means we are expressing the number in hexadecimal (base 16)
		//       `0b` is another useful prefix, expresses the number in binary
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0x3C, 0x63, 0xFF, 0XFF);
		SDL_RenderFillRect(renderer, &player1_rect);

		SDL_SetRenderDrawColor(renderer, 0x8C, 0x93, 0xFF, 0XFF);
		SDL_RenderFillRect(renderer, &player2_rect);

		SDL_GetCurrentTime(&walltime_work_end);
		time_elapsed_work = walltime_work_end - walltime_frame_beg;

		if(target_framerate_ns > time_elapsed_work)
		{
			switch(delay_type)
			{
				case 0:
				{
					// busy wait - very precise, but costly
					SDL_Time walltime_busywait = walltime_work_end;
					while(walltime_busywait - walltime_frame_beg < target_framerate_ns)
						SDL_GetCurrentTime(&walltime_busywait);
					break;
				}
				case 1:
				{
					// simple delay - too imprecise
					// NOTE: `SDL_Delay` gets milliseconds, but our timer gives us nanoseconds! We need to covert it manually
					SDL_Delay((target_framerate_ns - time_elapsed_work) / 1000000);
					break;
				}
				case 2:
				{
					// delay ns - also too imprecise
					SDL_DelayNS(target_framerate_ns - time_elapsed_work);
					break;
				}
				case 3:
				{
					// delay precise
					SDL_DelayPrecise(target_framerate_ns - time_elapsed_work);
					break;
				}
				case 4:
				{
					// custom delay - we use the sleeping delay with an arbitrary margin, then we busywait what's left
					SDL_DelayNS(target_framerate_ns - time_elapsed_work - 1000000);
					SDL_Time walltime_busywait = walltime_work_end;

					while(walltime_busywait - walltime_frame_beg < target_framerate_ns)
						SDL_GetCurrentTime(&walltime_busywait);
					break;
				}
			}
		}

		SDL_GetCurrentTime(&walltime_frame_end);
		time_elapsed_frame = walltime_frame_end - walltime_frame_beg;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 10.0f, "elapsed (frame): %9.6f ms", (float)time_elapsed_frame/(float)1000000);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 20.0f, "elapsed(work   : %9.6f ms", (float)time_elapsed_work/(float)1000000);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 30.0f, "delay type: %d (change with 0-4)", delay_type);


		// render
		SDL_RenderPresent(renderer);
		
		walltime_frame_beg = walltime_frame_end;
	}

	return 0;
};
