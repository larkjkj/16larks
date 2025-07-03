#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arguments.h"
#include "rom.h"
#include "cpu.h"
#include "instructions.h"

#include "SDL.h"
#include "SDL_video.h"
#include "SDL_render.h"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "SDL_rect.h"

#ifdef D_PS2
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480 //implement wider
#else
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 448
#endif

int w, h;
char pc_buffer[20];
char add_buffer[20];
char ins_buffer[20];
char mem_buffer[20];
char rom_buffer[256];

//int w, h;
TTF_Font * pixelated;
SDL_Window* window;
SDL_Event event;
SDL_Renderer* renderer;

SDL_Color white = {
	255,
	255,
	255,
	255,
};

int p_w, p_h, a_w, a_h, i_w, i_h, m_w, m_h, r_w, r_h, t_w, t_h;

/*SDL_Surface* pc_surface;
SDL_Surface* add_surface;
SDL_Surface* ins_surface;
*/

extern void debug_memory() {
	snprintf(add_buffer, sizeof(add_buffer), "ADDRESS: %04X", address);
	snprintf(ins_buffer, sizeof(ins_buffer), "INSTRUCTION: %X", rom[pc]);
	snprintf(pc_buffer, sizeof(pc_buffer), "PC: %i", pc);
	snprintf(rom_buffer, sizeof(rom_buffer), "ROM: %s", rom_name);

	for(int i = 0; i < ((sizeof(total_memory)) / (sizeof(total_memory[0]))); i++) {
		snprintf(mem_buffer, sizeof(mem_buffer), "%04X", total_memory[i]);
	}
	

	/*add_surface = TTF_RenderText_Solid(pixelated, add_buffer, white);
	ins_surface = TTF_RenderText_Solid(pixelated, ins_buffer, white);
	pc_surface = TTF_RenderText_Solid(pixelated, pc_buffer, white);
	
	add_texture = SDL_CreateTextureFromSurface(renderer, add_surface);
	ins_texture = SDL_CreateTextureFromSurface(renderer, ins_surface);
	pc_texture = SDL_CreateTextureFromSurface(renderer, pc_surface);*/

	SDL_Texture * add_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, add_buffer, white));
	SDL_Texture * ins_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, ins_buffer, white));
	SDL_Texture * pc_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, pc_buffer, white));
	SDL_Texture * mem_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, mem_buffer, white));
	SDL_Texture * rom_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, rom_buffer, white));
	SDL_Texture * tot_mem_texture = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(pixelated, tot_mem_buffer, white));

	SDL_QueryTexture(pc_texture, NULL, NULL, &p_w, &p_h);
	SDL_QueryTexture(add_texture, NULL, NULL, &a_w, &a_h);
	SDL_QueryTexture(ins_texture, NULL, NULL, &i_w, &i_h);
	SDL_QueryTexture(mem_texture, NULL, NULL, &m_w, &m_h);
	SDL_QueryTexture(rom_texture, NULL, NULL, &r_w, &r_h);
	SDL_QueryTexture(tot_mem_texture, NULL, NULL, &t_w, &t_h);


	SDL_Rect pc_rect = {
		0, 	0,	//size of my ass
		p_w,	p_h,	//size of my penis 
	};
	SDL_Rect add_rect = {
		0, 	16,	//size of my ass
		a_w,	a_h,	//size of my penis 
	};
	SDL_Rect ins_rect = {
		0, 	32,	//size of my ass
		i_w,	i_h,	//size of my penis 
	};
	SDL_Rect mem_rect = {
		0, 	64,	//size of my ass
		m_w,	m_h,	//size of my penis 
	};
	SDL_Rect rom_rect = {
		0, 	SCREEN_HEIGHT - 32,	//size of my ass
		r_w,	r_h,	//size of my penis 
	};
	SDL_Rect tot_mem_rect = {
		0, 	SCREEN_HEIGHT - 32,	//size of my ass
		t_w,	t_h,	//size of my penis 
	};

	SDL_RenderCopy(renderer, add_texture, NULL, &add_rect);
	SDL_RenderCopy(renderer, ins_texture, NULL, &ins_rect);
	SDL_RenderCopy(renderer, pc_texture, NULL, &pc_rect);
	SDL_RenderCopy(renderer, mem_texture, NULL, &mem_rect);
	SDL_RenderCopy(renderer, rom_texture, NULL, &rom_rect);
	SDL_RenderCopy(renderer, tot_mem_texture, NULL, &tot_mem_rect);

/*	SDL_FreeSurface(add_surface);
	SDL_FreeSurface(ins_surface);
	SDL_FreeSurface(pc_surface);
*/
	SDL_DestroyTexture(add_texture);
	SDL_DestroyTexture(ins_texture);
	SDL_DestroyTexture(pc_texture);
	SDL_DestroyTexture(mem_texture);
	SDL_DestroyTexture(rom_texture);
	SDL_DestroyTexture(tot_mem_texture);
};

extern void initGraphics() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error initializing video \n");
		return;
	}
	TTF_Init();
	pixelated = TTF_OpenFont("fonts/retro.ttf", 15);	
	window = SDL_CreateWindow("penis", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (pixelated == NULL) {
		printf("Error opening the font! \n");
	}	
};

extern void eventLoop() {
	SDL_RenderClear(renderer);
	//debug_memory();
	SDL_RenderPresent(renderer);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			cpu_waiting = 0;
			exit(1);
		}
		if (event.type == SDL_QUIT) {
			cpu_waiting = 0;
			TTF_CloseFont(pixelated);
			SDL_Quit();
		}	
	}
	//SDL_Delay(16);
}
