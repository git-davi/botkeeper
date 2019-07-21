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

#endif
