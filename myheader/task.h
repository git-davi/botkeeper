#ifndef __TASK_H__
#define __TASK_H__

#include "dtypes.h"
#include "allegro.h"
#include <math.h>

#define T 40	// period of tasks
#define C 5		// capacity of tasks
#define FRAME_RATE 40
#define FRAME_C 15

#define POR_SCALING_FACTOR 5
#define POR_ANTICIPATION_MS 100
#define BALL_SCALING_FACTOR 500
#define FRICTION_FACTOR 0.99

#define SPEED_INDICATOR 10
#define POWER_SCALER 7
#define SPEED_ARROW 0.1
#define RAGGIO_FRECCIA 10

#define M 1000000000000000000
#define PRIO 99

#define L1 1	 	//			L1	
#define L2 2		//		---------
#define L3 3		//	L3	|       | L2
#define L4 4		//		|       |
#define LATI 4		//		---------
					//			L4


extern BITMAP *bground_b; 
extern BITMAP *portiere_b;
extern BITMAP *palla_b;
extern BITMAP *porta_b;
extern BITMAP *indicatore_b;
extern BITMAP *barra_b;
extern BITMAP *freccia_b;

extern palla_t palla;
extern portiere_t portiere;
extern campo_t campo;
extern freccia_t freccia;
extern potenza_t potenza;
extern porta_t porta;

extern int low_x, up_x, low_y, up_y;

// funzione per l'avvio di tutti i task
void init_tasks(void);

#endif
