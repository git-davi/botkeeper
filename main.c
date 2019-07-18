#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "allegro.h"
#include "dtypes.h"
#include "task.h"
#include "util.h"

#define W 800
#define H 600


BITMAP *bground_b; 
BITMAP *portiere_b;
BITMAP *palla_b;
BITMAP *porta_b;


void init_attr(void){
	pthread_condattr_t ca;
	pthread_mutexattr_t ma;
	
	pthread_condattr_init(&ca);
	pthread_mutexattr_init(&ma);

	pthread_cond_init(&palla.ready, &ca);
	pthread_mutex_init(&palla.m, &ma);
	pthread_mutex_init(&portiere.m, &ma);
	
}


void init_bground(void){
	BITMAP *tmp;
	int w = 750;
	int h = 520;
	int x, y;

	tmp = load_bitmap("img/campo.bmp", NULL);
	if(tmp == NULL){
		printf("background file not found\n");
		exit(1);
	}

	x = SCREEN_W/2 - bground_b->w/2;
	y = SCREEN_H - bground_b->h - 10;
	bground_b = create_bitmap(w, h);
	stretch_sprite(bground_b, tmp, 0, 0, w, h);
	blit(bground_b, screen, 0, 0, x, y, SCREEN_W, SCREEN_H);

	border.up_x = x;
	border.low_x = border.up_x + bground_b->w;
	border.up_y = y;
	border.low_y = border.up_y + bground_b->h;
}

void init_portiere(void){
	// size di portiere_p : 100x100 
	int x, y;

	portiere_b = load_bitmap("img/portiere_p.bmp", NULL);
	if(portiere_b == NULL){
		printf("portiere file not found\n");
		exit(1);
	}

	x = SCREEN_W/2 - portiere_b->w/2;
	y = SCREEN_H - bground_b->h - 10;
	draw_sprite(screen, portiere_b, x, y);

	
	portiere.x = center_x(portiere_b, x);
	portiere.y = center_y(portiere_b, y);
	portiere.v = 0;
}


void init_palla(void){
	// size di portiere_p : 100x100 
	int x, y;

	palla_b = load_bitmap("img/palla_p.bmp", NULL);
	if(palla_b == NULL){
		printf("palla file not found\n");
		exit(1);
	}

	x = SCREEN_W/2 - palla_b->w/2;
	y = SCREEN_H/2 + 120;
	draw_sprite(screen, palla_b, x, y);

	palla.x = center_x(palla_b, x);
	palla.y = center_y(palla_b, y);
	palla.v = 0;
	palla.dir.x = 0;
	palla.dir.y = 0;

}


void init_porta(void){
	BITMAP *tmp;
	int w = 390;
	int h = 50;
	int bground_h = SCREEN_H - bground_b->h - 10;

	tmp = load_bitmap("img/porta.bmp", NULL);
	if(tmp == NULL){
		printf("porta file not found\n");
		exit(1);
	}

	porta_b = create_bitmap(w, h);
	stretch_sprite(porta_b, tmp, 0, 0, w, h);
	blit(porta_b, screen, 0, 0, SCREEN_W/2 - porta_b->w/2, 
								bground_h - porta_b->h, 
								SCREEN_W, SCREEN_H);

}


void init(void){

	allegro_init();
	install_keyboard();
	set_color_depth(desktop_color_depth());
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, W, H, 0, 0);
	
	init_bground();	
	init_portiere();
	init_palla();
	init_porta();

	init_attr();	//init mutex e cond variables
}


void stop(void){
	//destroy_bitmap();	// automatic action
	allegro_exit();
}


int main(void){
	init();

	readkey();
		
	stop();
	return 0;
}
