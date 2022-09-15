#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#define POS_X 100
#define POS_Y 100
#define WIDTH 640
#define HEIGHT 480

struct atom {

	int x;
	int y;
	double vx;
	double vy;

} atom;


void position_update(struct atom *a, struct atom *b, double g)
{

	double fx = 0;
	double fy = 0;
	int dx = a->x - b->x;
	int dy = a->y - b->y;
	double d = (dx*dx + dy*dy); d = powf(d, 0.5);
	if(d > 0 && d < 80) {
		double F = g/d;
		fx += F*dx;
		fy += F*dy;
	}
	a->vx = (a->vx + fx)*0.5;
	a->vy = (a->vy + fy)*0.5;
	a->x += (int) rintf(a->vx);
	a->y += (int) rintf(a->vy);
	if(a->x <= 0 || a->x >= WIDTH) {
		a->vx *= -1;
		if(a->x >= WIDTH) { a->x = WIDTH-1; }
		if(a->x <= 0) { a->x = 1; }
	}
	if(a->y <= 0 || a->y >= HEIGHT) {
		a->vy *= -1;
		if(a->y >= HEIGHT) { a->y = HEIGHT-1; }
		if(a->y <= 0) { a->y = 1; }
	}	
}

unsigned int random_uint(unsigned int range)
{

	unsigned int x;
	FILE *fp = fopen("/dev/urandom", "r");
	fread(&x, sizeof x, 1, fp);
	fclose(fp);
	unsigned int return_value = x % range;
	return return_value;

}

int main(void)
{

	// Setting Up Window Pointers
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Surface *surface = NULL;

	// Setting up number of particles.
	const int particles = 100;
	SDL_Rect src_rect1[particles];
	SDL_Rect dest_rect1[particles];
	SDL_Rect src_rect2[particles];
	SDL_Rect dest_rect2[particles];
	
	// Color Background
	surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));

	// struct atom initialization of blue particles
	struct atom blue[particles];
	for (int i=0; i<particles; i++) {
		
		blue[i].x = random_uint(WIDTH);
		blue[i].y = random_uint(HEIGHT);
		blue[i].vx = 0;
		blue[i].vy = 0;

	}

	// struct atom initialization of red particles
	struct atom red[particles];
	for (int i=0; i<particles; i++) {
		
		red[i].x = random_uint(WIDTH);
		red[i].y = random_uint(HEIGHT);
		red[i].vx = 0;
		red[i].vy = 0;

	}

	// This sets up the rectangle array.
	for (int i=0; i<particles; i++) {

		// Create rectangle in iteration i of array
		src_rect1[i].x = blue[i].x;
		src_rect1[i].y = blue[i].y;
		src_rect1[i].w = 3;
		src_rect1[i].h = 3;

		// Render rectangle
		SDL_RenderFillRect(renderer, &src_rect1[i]);
		SDL_FillRect(surface, &src_rect1[i], SDL_MapRGB(surface->format, 0, 0, 255));
	
		// Cleans destination array to source value
		dest_rect1[i].x = src_rect1[i].x;
		dest_rect1[i].y = src_rect1[i].y;
		dest_rect1[i].w = src_rect1[i].w;
		dest_rect1[i].h = src_rect1[i].h;
	}
	
	for (int i=0; i<particles; i++) {

		// Create rectangle in iteration i of array
		src_rect2[i].x = red[i].x;
		src_rect2[i].y = red[i].y;
		src_rect2[i].w = 3;
		src_rect2[i].h = 3;

		// Render rectangle
		SDL_RenderFillRect(renderer, &src_rect2[i]);
		SDL_FillRect(surface, &src_rect2[i], SDL_MapRGB(surface->format, 255, 0, 0));
	
		// Cleans destination array to source value
		dest_rect2[i].x = src_rect2[i].x;
		dest_rect2[i].y = src_rect2[i].y;
		dest_rect2[i].w = src_rect2[i].w;
		dest_rect2[i].h = src_rect2[i].h;
	}

	// Create Window
	win = SDL_CreateWindow("Movement", POS_X, POS_Y, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Init(SDL_INIT_VIDEO);

	// Setting up loop
	SDL_bool loopShouldStop = SDL_FALSE;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	// Main render loop
	while(!loopShouldStop) {
		
			
		// Position updater
		SDL_RenderClear(renderer);

		// Blue particle
		for(int i=0; i<particles; i++) {

			for(int j=0; j<particles; j++) {
				position_update(&blue[i], &blue[j], 1.25);
				position_update(&blue[i], &red[j], -2.25);
			}
			dest_rect1[i].x = blue[i].x;
			dest_rect1[i].y = blue[i].y;

			// Rendering of the new position
			SDL_RenderCopy(renderer, texture, &src_rect1[i], &dest_rect1[i]);
		}
		
		// Red particle
		for(int i=0; i<particles; i++) {

			for(int j=0; j<particles; j++) {
				position_update(&red[i], &blue[j], 2.25);
				position_update(&red[i], &red[j], -1.25);
			}
			dest_rect2[i].x = red[i].x;
			dest_rect2[i].y = red[i].y;

			// Rendering of the new position
			SDL_RenderCopy(renderer, texture, &src_rect2[i], &dest_rect2[i]);
		}
		SDL_RenderPresent(renderer);

		// Quit event listener
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch (event.type){
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
				case SDLK_q:
					loopShouldStop = SDL_TRUE;
					break;
				}
				break;
			}
		}

		// 60fps
		SDL_Delay(1000/60);

	}

	// Clean Up
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
