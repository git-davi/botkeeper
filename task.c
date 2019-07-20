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
campo_t campo;


void *task_portiere(void *arg){
	// aggiorna la sua posizione in base alla 
	// traiettoria della palla
	int tmp_dir;	

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
	if(palla.v.x==0 && palla.v.y==0){
		pthread_cond_signal(&palla.ready);
	}

	pthread_mutex_unlock(&palla.m);

	return NULL;
}

void *task_grafico(void *arg){
	int palla_x;
	int palla_y;
	int portiere_x;
        int portiere_y;
	int outl;
	int outr;
	int outt;
	int outb;

	//ottengo le coordinate della palla
	pthread_mutex_lock(&palla.m);
	palla_x=palla.x;
	palla_y=palla.y;
	pthread_mutex_unlock(&palla.m);

	//ottengo le coordinate del portiere
	pthread_mutex_lock(&portiere.m);
        portiere_x=portiere.x;
        portiere_y=portiere.y;
        pthread_mutex_unlock(&portiere.m);

	//decentro palla e portiere
	palla_x = decenter_x(palla_b, palla_x);
	palla_y = decenter_y(palla_b, palla_y);
	portiere_x = decenter_x(portiere_b, portiere_x);
	portiere_y = decenter_y(portiere_b, portiere_y);

	//disegno campo,portiere,palla
	blit(bground_b, screen, 0, 0, campo.border_x.low, campo.border_y.low,
		SCREEN_W, SCREEN_H);
	draw_sprite(screen, portiere_b, portiere_x, portiere_y);
	draw_sprite(screen, palla_b, palla_x, palla_y);

	//calcolo le collisioni contro i bordi
	outl = (palla.x <= border.x.low);
	outr = (palla.x >= border.x.up);
	outt = (palla.y <= border.y.low);
	outb = (palla.y >= border.y.up);
	
	if(outl){
		palla.x = border.x.low;
	}
	if(outr){
                palla.x = border.x.up;
        }
	if(outl || outr){
		palla.v.x = -palla.v.x;
	}
	if(outt){
                palla.y = border.x.low;
        }
	if(outb){
                palla.y = border.x.up;
        }
	if(outt || outb){
                palla.v.y = -palla.v.y;
        }

	outl = (palla.x >= portiere_x);
        outr = (palla.x >= portiere_x + portiere_b->w);
        outt = (palla.y <= portiere_y);
        outb = (palla.y >= portiere_y + portiere_b->h);
 
	
	return NULL;
}

void *task_user(void *arg){

	while(1){
		while(palla.v.x!=0 || palla.v.y!=0){
			pthread_cond_wait(&palla.ready, &palla.m);
		}
		//init freccia e potenza
		while(!key[KEY_SPACE]){
			//aggiornamento rotazione freccia
		}	
		//leggi rotazione e assegna a palla
		while(!key[KEY_SPACE]){
			//aggiornamento potenza
		}
		//leggi potenza e assegna a palla

	}
	
	return NULL;
}

void init_tasks(void){

}

