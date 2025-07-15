#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#ifdef _PS2
#include "iopcontrol.h"
#include "sifrpc-common.h"
#include "sbv_patches.h"
#include "ps2_filesystem_driver.h"
#include "debug.h"
#include "kernel.h"
#endif

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480 

int w, h;

static char total_memory_buffer[200];
static char old_total_memory_buffer[sizeof(total_memory_buffer)];

static char pc_buffer[50];
static char old_pc_buffer[sizeof(pc_buffer)];

static char ins_buffer[50];
static char old_ins_buffer[sizeof(ins_buffer)];

static char rom_buffer[256];
static char old_rom_buffer[sizeof(rom_buffer)];

static char gen_buffer[50];
static char old_gen_buffer[sizeof(gen_buffer)];

static char type_buffer[256];
static char old_type_buffer[sizeof(type_buffer)];

static char type2_buffer[300];
static char old_type2_buffer[sizeof(type2_buffer)];

//int w, h;
TTF_Font * pixelated;

SDL_Window* window;
SDL_Event event;
SDL_Renderer* renderer;

SDL_Texture * ins_texture = NULL;
SDL_Texture * pc_texture = NULL;
SDL_Texture * rom_texture = NULL;
SDL_Texture * gen_texture = NULL;
SDL_Texture * total_mem_texture = NULL;
SDL_Texture * mem_texture = NULL;
SDL_Texture * type_texture = NULL;
SDL_Texture * type2_texture = NULL;

SDL_Surface * ins_surface = NULL;
SDL_Surface * pc_surface = NULL;
SDL_Surface * rom_surface = NULL;
SDL_Surface * gen_surface = NULL;
SDL_Surface * total_mem_surface = NULL;
SDL_Surface * mem_surface = NULL;
SDL_Surface * type_surface = NULL;
SDL_Surface * type2_surface = NULL;


SDL_Color white = {
	255,
	255,
	255,
	255,
};

int tm_w, tm_h, t2_w, t2_h, g_w, g_h, p_w, p_h, a_w, a_h, i_w, i_h, m_w, m_h, r_w, r_h, t_w, t_h;

extern void debug_memory() {
	int rows = 10;
	int dist = 6;
	int tm_width;
	int tm_height;


	snprintf(pc_buffer, sizeof(pc_buffer), "PC: %i", pc);
	snprintf(ins_buffer, sizeof(ins_buffer), "INS: %X", instruction);
	snprintf(rom_buffer, sizeof(rom_buffer), "ROM: %s", rom_name);
	snprintf(gen_buffer, sizeof(gen_buffer), "A: %i \n X: %i \n Y: %i \n DP: %i \n", a, x, y, dp);
	snprintf(type_buffer, sizeof(type_buffer), "ROM_TYPE: %s", rom_type);
	snprintf(type2_buffer, sizeof(type2_buffer), "Z_FLAG: %i  \n C_FLAG: %i  \n X_FLAG: %i  \n N_FLAG: %i  \n D_FLAG: %i  \n", z_flag, c_flag, x_flag, n_flag, d_flag);
	if (strcmp(pc_buffer, old_pc_buffer) != 0) {
		strcpy(old_pc_buffer, pc_buffer);
		if (pc_texture) {
			SDL_DestroyTexture(pc_texture);
		}
		SDL_Surface * pc_surface = TTF_RenderText_Solid(pixelated, pc_buffer, white);
		pc_texture = SDL_CreateTextureFromSurface(renderer, pc_surface);
		SDL_FreeSurface(pc_surface);
	}
	if (strcmp(ins_buffer, old_ins_buffer) != 0) {
		strcpy(old_ins_buffer, ins_buffer);
		if (ins_texture) {
			SDL_DestroyTexture(ins_texture);
		}
		SDL_Surface * ins_surface = TTF_RenderText_Solid(pixelated, ins_buffer, white);
		ins_texture = SDL_CreateTextureFromSurface(renderer, ins_surface);
		SDL_FreeSurface(ins_surface);
	}
	if (strcmp(rom_buffer, old_rom_buffer) != 0) {
		strcpy(old_rom_buffer, rom_buffer);
		if (rom_texture) {
			SDL_DestroyTexture(rom_texture);
		}
		SDL_Surface * rom_surface = TTF_RenderText_Solid(pixelated, rom_buffer, white);
		rom_texture = SDL_CreateTextureFromSurface(renderer, rom_surface);
		SDL_FreeSurface(rom_surface);
	}
	if (strcmp(gen_buffer, old_gen_buffer) != 0) {
		strcpy(old_gen_buffer, gen_buffer);
		if (gen_texture) {
			SDL_DestroyTexture(gen_texture);
		}
		SDL_Surface * gen_surface = TTF_RenderText_Solid(pixelated, gen_buffer, white);
		gen_texture = SDL_CreateTextureFromSurface(renderer, gen_surface);
		SDL_FreeSurface(gen_surface);
	}
	if (strcmp(type2_buffer, old_type2_buffer) != 0) {
		strcpy(old_type2_buffer, type2_buffer);
		if (type2_texture) {
			SDL_DestroyTexture(type2_texture);
		}
		SDL_Surface * type2_surface = TTF_RenderText_Solid(pixelated, type2_buffer, white);
		type2_texture = SDL_CreateTextureFromSurface(renderer, type2_surface);
		SDL_FreeSurface(type2_surface);
	}
	if (strcmp(type_buffer, old_type_buffer) != 0) {
		strcpy(old_type_buffer, type_buffer);
		if (type_texture) {
			SDL_DestroyTexture(type_texture);
		}
		SDL_Surface * type_surface = TTF_RenderText_Solid(pixelated, type_buffer, white);
		type_texture = SDL_CreateTextureFromSurface(renderer, type_surface);
		SDL_FreeSurface(type_surface);
	}
	if (strcmp(total_memory_buffer, old_total_memory_buffer) != 0) {
		strcpy(old_total_memory_buffer, total_memory_buffer);
		if (total_mem_texture) {
			SDL_DestroyTexture(total_mem_texture);
		}
		total_mem_texture = SDL_CreateTextureFromSurface(renderer, total_mem_surface);
		SDL_FreeSurface(total_mem_surface);
	}

	SDL_QueryTexture(pc_texture, NULL, NULL, &p_w, &p_h);
	SDL_QueryTexture(ins_texture, NULL, NULL, &i_w, &i_h);
	SDL_QueryTexture(rom_texture, NULL, NULL, &r_w, &r_h);
	SDL_QueryTexture(gen_texture, NULL, NULL, &g_w, &g_h);
	SDL_QueryTexture(type_texture, NULL, NULL, &t_w, &t_h);
	SDL_QueryTexture(type2_texture, NULL, NULL, &t2_w, &t2_h);
	SDL_QueryTexture(total_mem_texture, NULL, NULL, &tm_w, &tm_h);

	
	SDL_Rect gen_rect = {
		0, 	128,	//size of my ass
		g_w,	g_h,	//size of my penis 
	};
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
	SDL_Rect type2_rect = {
		0, 	SCREEN_HEIGHT / 3,	//size of my ass
		t2_w,	t2_h,	//size of my penis 
	};
	SDL_Rect tot_mem_rect = {
		0, 	SCREEN_HEIGHT - 64,	//size of my ass
		t_w,	t_h,	//size of my penis 
	};
	SDL_Rect tm_rect = {
		0, 	SCREEN_HEIGHT / 2,	//size of my ass
		SCREEN_WIDTH / 2,	SCREEN_HEIGHT / 2,	//size of my penis 
	};
	SDL_RenderCopy(renderer, ins_texture, NULL, &ins_rect);
	SDL_RenderCopy(renderer, pc_texture, NULL, &pc_rect);
	SDL_RenderCopy(renderer, rom_texture, NULL, &rom_rect);
	SDL_RenderCopy(renderer, gen_texture, NULL, &gen_rect);
	SDL_RenderCopy(renderer, type_texture, NULL, &tot_mem_rect);
	SDL_RenderCopy(renderer, type2_texture, NULL, &type2_rect);
	SDL_RenderCopy(renderer, total_mem_texture, NULL, &tm_rect);
};

#ifdef _PS2
extern void reset_IOP() {
	SifInitRpc(0);
	while (!SifIopReset(NULL, 0)) {};
    	while (!SifIopSync()) {};
    	SifInitRpc(0);
    	sbv_patch_enable_lmb();
    	sbv_patch_disable_prefix_check();
}
#endif

extern void initGraphics() {
	#ifdef _PS2
	reset_IOP();
	//init_ps2_filesystem_driver();
	#endif
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error initializing video \n");
	
		#ifdef _PS2
		scr_printf("Error initializing video %s \n", SDL_GetError());
		#endif
		
		return;
	}

	TTF_Init();

pixelated = TTF_OpenFont("fonts/Pixellari.ttf", 17);	
	window = SDL_CreateWindow("penis", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (pixelated == NULL) {
		printf("Error opening the font! \n");
		
		printf("Error opening the font! \n");
	}	
};

extern void eventLoop() {
	SDL_RenderClear(renderer);
	debug_memory();
	SDL_RenderPresent(renderer);
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			TTF_Quit();
			SDL_Quit();
			cpu_waiting = 0;
			exit(0);
		}
	}
	SDL_Delay((u32) 1200);
}
