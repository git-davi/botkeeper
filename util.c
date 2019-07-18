// file per le funzioni ausialiarie
#include "util.h"

// trovare il centro della bitmap
int center_x(BITMAP *bmp, int x){
	return x + bmp->w/2;
} 

int center_y(BITMAP *bmp, int y){
	return y + bmp->h/2;
}

// passare dal centro al primo punto 
// (usato per disegnare la bitmap)
int decenter_x(BITMAP *bmp, int x){
	return x - bmp->w/2;
} 

int decenter_y(BITMAP *bmp, int y){
	return y - bmp->h/2;
}
