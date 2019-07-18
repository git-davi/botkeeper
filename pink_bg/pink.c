#include <stdio.h>
#include "allegro.h"



int main(void){

	BITMAP *orig, *final;
	PALETTE pal;
	int x, y, c;
	int white, pink;
	
	allegro_init();
	set_color_depth(16);
//	set_color_depth(desktop_color_depth());

	white = makecol(255, 255, 255);
	pink = makecol(255, 0, 255);
	orig = load_bitmap("img/portiere.bmp", NULL);
	final = create_bitmap(orig->w, orig->h);
	
	for(x=0; x<orig->w; x++){
		for(y=0; y<orig->h; y++){
			
			c = getpixel(orig, x, y);
			if(c == white) c = pink;
			putpixel(final, x, y, c);
		}
	}

	get_palette(pal);
	save_bitmap("img/portiere_p.bmp", final, pal);

	allegro_exit();
	return 0;
}
