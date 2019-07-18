#include <stdio.h>

#include "allegro.h"
#include "task.h"


BITMAP *bground; 
BITMAP *portiere;
BITMAP *palla;
BITMAP *porta;


void init_bground(void){
	BITMAP *tmp;
	int w = 750;
	int h = 520;

	tmp = load_bitmap("img/campo.bmp", NULL);
	if(tmp == NULL){
		printf("background file not found\n");
		exit(1);
	}

	bground = create_bitmap(w, h);
	stretch_sprite(bground, tmp, 0, 0, w, h);
	blit(bground, screen, 0, 0, SCREEN_W/2 - bground->w/2, 
								SCREEN_H - bground->h - 10, 
								SCREEN_W, SCREEN_H);


}

void init_portiere(void){
	// size di portiere_p : 100x100 

	portiere = load_bitmap("img/portiere_p.bmp", NULL);
	if(portiere == NULL){
		printf("portiere file not found\n");
		exit(1);
	}

	draw_sprite(screen, portiere, SCREEN_W/2 - portiere->w/2, 
								SCREEN_H - bground->h - 10);
}


void init_palla(void){
	// size di portiere_p : 100x100 

	palla = load_bitmap("img/palla_p.bmp", NULL);
	if(palla == NULL){
		printf("palla file not found\n");
		exit(1);
	}

	draw_sprite(screen, palla, SCREEN_W/2 - palla->w/2, SCREEN_H/2 + 120);
}


void init_porta(void){
	BITMAP *tmp;
	int w = 390;
	int h = 50;
	int bground_h = SCREEN_H - bground->h - 10;

	tmp = load_bitmap("img/porta.bmp", NULL);
	if(tmp == NULL){
		printf("porta file not found\n");
		exit(1);
	}

	porta = create_bitmap(w, h);
	stretch_sprite(porta, tmp, 0, 0, w, h);
	blit(porta, screen, 0, 0, SCREEN_W/2 - porta->w/2, 
								bground_h - porta->h, 
								SCREEN_W, SCREEN_H);


}


void init(void){
	int w = 800;	// width
	int h = 600;	// height

	allegro_init();
	install_keyboard();
	set_color_depth(desktop_color_depth());
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, w, h, 0, 0);
	
	init_bground();	
	init_portiere();
	init_palla();
	init_porta();
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
