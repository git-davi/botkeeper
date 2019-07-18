#include <pthread.h>
#include <unistd.h>
#include <ptask.h>

#include "task.h"


palla_t palla;
portiere_t portiere;


void init_attr(void){
	pthread_condattr_t ca;
	pthread_mutexattr_t ma;
	
	pthread_condattr_init(&palla.ready);
	pthread_mutexattr_init(&palla.m);
	pthread_mutexattr_init(&portiere.m);

	palla.x = 0;
	palla.y = 0;
	palla.v = 0;
	palla.dir.x = 0;
	palla.dir.y = 0;

	portiere.x = 0;
	portiere.y = 0;
	portiere.v = 0;
}

void *task_portiere(void *arg){

}

void *task_palla(void *arg){

}

void *task_grafico(void *arg){

}

void *task_user(void *arg){

}

void init_tasks(void){

}

