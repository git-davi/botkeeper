#ifndef __DTYPES_H__
#define __DTYPES_H__

// fixed è un numero decimale in virgola fissata

typedef struct border_t{
	int up, low;
} border_t;

typedef struct portiere_t {
	int x, y;	// posizione del portiere
	int dir;	// direzione/velocita' del portiere
	border_t border;
	pthread_mutex_t m;	
} portiere_t;

typedef struct vector_t {
	int x, y;
} vector_t;

typedef struct palla_t {
	int x, y;		// posizione della palla
	vector_t v;		// direzione della palla
	pthread_mutex_t m;
	pthread_cond_t ready;
	float angle;
} palla_t;

typedef struct campo_t {
        border_t border_x;
	border_t border_y;
} campo_t;


#endif