#ifndef __TASK_H__
#define __TASK_H__

#include "dtypes.h"
#include "allegro.h"

#define T 10


extern BITMAP *bground_b; 
extern BITMAP *portiere_b;
extern BITMAP *palla_b;
extern BITMAP *porta_b;	

extern palla_t palla;
extern portiere_t portiere;
extern border_t border;

// funzione per l'avvio di tutti i task
void init_tasks(void);

#endif
