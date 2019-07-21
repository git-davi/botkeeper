// file per le funzioni ausialiarie
#include <math.h>
#include "util.h"

#define N 2

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



vector_t reflect_x(vector_t v){
	v.y = - v.y;
	return v;
}

vector_t reflect_y(vector_t v){
	v.x = - v.x;
	return v;
}

double dist_punto_retta(vector_t p, retta_t r){
	double num, den;
	
	num = abs(r.a * p.x + r.b * p.y + r.c);
	den = sqrt(pow(r.a, 2) + pow(r.b, 2));

	return num/den;
}

retta_t retta_due_punti(vector_t p1, vector_t p2){
	retta_t r;

	r.a = p2.y - p1.y;
	r.b = -(p2.x - p1.x);
	r.c = (- p1.x) * (p2.y - p1.y) + p1.y * (p2.x - p1.x);

	return r;
}

