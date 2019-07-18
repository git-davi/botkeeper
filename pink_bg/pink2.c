#include <stdio.h>
#include "allegro.h"



int main(void){

	BITMAP *orig, *final, *tmp;
	PALETTE pal;
	int x, y, c;
	int white, pink;
	int new_w = 40;
	int new_h = 40;

	
	allegro_init();
	set_color_depth(16);
//	set_color_depth(desktop_color_depth());

	white = makecol(255, 255, 255);
	pink = makecol(255, 0, 255);

	// creo gli sprite
	orig = load_bitmap("img/palla.bmp", NULL);
	tmp = create_bitmap(new_w, new_h);
	final = create_bitmap(new_w, new_h);

	// ridimensiono lo sprite originale
	stretch_sprite(tmp, orig, 0, 0, new_w, new_h);
	
	// converto pixel rosa
	for(x=0; x<tmp->w; x++){
		for(y=0; y<tmp->h; y++){
			
			c = getpixel(tmp, x, y);
			if(c == white) c = pink;
			putpixel(final, x, y, c);
		}
	}

	get_palette(pal);
	save_bitmap("img/palla_p.bmp", final, pal);

	allegro_exit();
	return 0;
}
