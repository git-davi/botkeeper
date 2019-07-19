#include <ptask.h>

#include "task.h"
#include "util.h"

#define SCALING_FACTOR 10
#define FRICTION_FACTOR 1

BITMAP *bground_b; 
BITMAP *portiere_b;
BITMAP *palla_b;
BITMAP *porta_b;

palla_t palla;
portiere_t portiere;
border_t border;


void *task_portiere(void *arg){
	// aggiorna la sua posizione in base alla 
	// traiettoria della palla
	fixed tmp_dir;	

	pthread_mutex_lock(&portiere.m);
	// trovo la direzione del portiere 
	// -> verrà usata come vettore velocità (scalandola)
	pthread_mutex_lock(&palla.m);
	tmp_dir = palla.x - portiere.x;
	pthread_mutex_unlock(&palla.m);

	// calcolo nuova posizione del portiere
	portiere.x = portiere.x + tmp_dir/SCALING_FACTOR * T;
	pthread_mutex_unlock(&portiere.m);

	return NULL;
}

void *task_palla(void *arg){
	// aggiorno posizione della palla
	pthread_mutex_lock(&palla.m);
	palla.x = palla.x + palla.v.x * T;
	palla.y = palla.y + palla.v.y * T;
	// attrito rallenta la palla
/*	
	if(palla.v.x != 0)
		palla.v.x -= FRICTION_FACTOR;
	if(palla.v.y != 0)
		palla.v.y -= FRICTION_FACTOR;
*/
	pthread_mutex_unlock(&palla.m);

	return NULL;
}

void *task_grafico(void *arg){

	return NULL;
}

void *task_user(void *arg){

	return NULL;
}

void init_tasks(void){

}

