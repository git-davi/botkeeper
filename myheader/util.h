#ifndef __UTIL_H__
#define __UTIL_H__

#include "dtypes.h"
#include "allegro.h"

int center_x(BITMAP *bmp, int x);
int center_y(BITMAP *bmp, int y);
int decenter_x(BITMAP *bmp, int x);
int decenter_y(BITMAP *bmp, int y);
retta_t retta_due_punti(vector_t p1, vector_t p2);
double dist_punto_retta(vector_t p, retta_t r);
vector_t reflect_x(vector_t v);
vector_t reflect_y(vector_t v);
int sign(int x);
void set_velocita_palla(int x, int y);
void leggi_coordinate(	int *palla_x, int *palla_y, 
						int *portiere_x, int *portiere_y,
						int *barra_x, int *barra_y,
						int *indicatore_x, int *indicatore_y,
						int *freccia_x, int *freccia_y);
void collisione_portiere(void);
int bordo_collisione(retta_t r1, retta_t r2, retta_t r3, retta_t r4);
void calcolo_rette(retta_t *r1, retta_t *r2, 
					retta_t *r3, retta_t *r4);
void collisione_bordi(void);


#endif
