#ifndef __DTYPES_H__
#define __DTYPES_H__


typedef struct portiere_t {
	int x, y;	// posizione del portiere
	float v;	// velocita' del portiere
	pthread_mutex_t m;
} portiere_t;

typedef struct vector_t {
	int x, y;
} vector_t;

typedef struct palla_t {
	int x, y;		// posizione della palla
	float v;		// velocità della palla
	vector_t dir;	// direzione della palla
	pthread_mutex_t m;
	pthread_cond_t ready;
} palla_t;

typedef struct border_t{
	int up_x, low_x;
	int up_y, low_y;
} border_t;

#endif
